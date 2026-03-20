// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/types/segmented_vector.hpp"
#include "gl/types/types.hpp"
#include "hgl/directional_tags.hpp"
#include "hgl/impl/layout_tags.hpp"
#include "hgl/types.hpp"

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <ranges>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_flat_incidence_list;
} // namespace hgl_testing
#endif

namespace hgl::impl {

namespace detail {

using common_storage_type = gl::types::segmented_vector<gl::types::id_type>;

[[nodiscard]] inline bool contains(
    const common_storage_type::const_segment_type& segment, const gl::types::id_type minor_id
) noexcept {
    const auto minor_it = std::ranges::lower_bound(segment, minor_id);
    return minor_it != segment.end() and *minor_it == minor_id;
}

inline void unique_insert(
    common_storage_type& storage,
    const gl::types::id_type major_id,
    const gl::types::id_type minor_id
) noexcept {
    const auto segment = storage[major_id];
    const auto insert_it = std::ranges::lower_bound(segment, minor_id);
    if (insert_it == segment.end() or *insert_it != minor_id) {
        const auto pos =
            static_cast<gl::types::size_type>(std::distance(segment.begin(), insert_it));
        storage.insert(major_id, pos, minor_id);
    }
}

inline void remove_minor(common_storage_type& storage, const gl::types::id_type id) noexcept {
    auto& data = storage.data_storage();
    auto& offsets = storage.offsets_storage();

    auto write_idx = 0uz;
    const auto n_segments = storage.size();

    for (auto seg_idx = 0uz; seg_idx < n_segments; ++seg_idx) {
        const auto orig_start = offsets[seg_idx];
        const auto orig_end = offsets[seg_idx + 1uz];

        offsets[seg_idx] = write_idx;

        for (auto i = orig_start; i < orig_end; ++i) {
            auto val = data[i];
            if (val == id)
                continue;
            if (val > id)
                val--;
            data[write_idx++] = val;
        }
    }
    offsets.back() = write_idx;
    data.resize(write_idx);
}

} // namespace detail

template <
    type_traits::c_hypergraph_directional_tag DirectionalTag,
    type_traits::c_hypergraph_layout_tag LayoutTag>
class flat_incidence_list;

template <type_traits::c_hypergraph_asymmetric_layout_tag LayoutTag>
class flat_incidence_list<hgl::undirected_t, LayoutTag> final {
public:
    using directional_tag = hgl::undirected_t;
    using layout_tag = LayoutTag;

    flat_incidence_list(const flat_incidence_list&) = delete;
    flat_incidence_list& operator=(const flat_incidence_list&) = delete;

    flat_incidence_list() = default;

    flat_incidence_list(const types::size_type n_vertices, const types::size_type n_hyperedges)
    : _storage{layout_tag::major(n_vertices, n_hyperedges)} {}

    flat_incidence_list(flat_incidence_list&&) = default;
    flat_incidence_list& operator=(flat_incidence_list&&) = default;

    ~flat_incidence_list() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        this->_add<impl::element_type::vertex>(n);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        this->_remove<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_incident_with<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const types::id_type vertex_id
    ) const noexcept {
        return this->_size<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> degree_map(
        const types::size_type n_vertices
    ) const noexcept {
        return this->_size_map<impl::element_type::vertex>(n_vertices);
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        this->_add<impl::element_type::hyperedge>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        this->_remove<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_incident_with<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type hyperedge_size(
        const types::id_type hyperedge_id
    ) const noexcept {
        return this->_size<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> hyperedge_size_map(
        const types::size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<impl::element_type::hyperedge>(n_hyperedges);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        detail::unique_insert(this->_storage, major_id, minor_id);
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);

        const auto segment = this->_storage[major_id];
        const auto minor_it = std::ranges::lower_bound(segment, minor_id);
        if (minor_it != segment.end() and *minor_it == minor_id) {
            const auto pos =
                static_cast<types::size_type>(std::distance(segment.begin(), minor_it));
            this->_storage.erase(major_id, pos);
        }
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        return detail::contains(
            this->_storage[layout_tag::major(vertex_id, hyperedge_id)],
            layout_tag::minor(vertex_id, hyperedge_id)
        );
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_flat_incidence_list;
#endif

private:
    using element_type = types::id_type;
    using storage_type = types::segmented_vector<element_type>;
    using storage_segment_type = typename storage_type::segment_type;
    using storage_const_segment_type = typename storage_type::const_segment_type;

    template <impl::element_type Element>
    void _add(const types::size_type n) noexcept {
        if constexpr (Element == layout_tag::major_element) // add major
            this->_storage.resize(this->_storage.size() + n);
    }

    template <impl::element_type Element>
    void _remove(const types::id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) // remove major
            this->_storage.erase(id);
        else // remove minor
            detail::remove_minor(this->_storage, id);
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _incident_with(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // incident with major
            return this->_storage[id];
        }
        else { // incident with minor
            return std::views::iota(0uz, this->_storage.size())
                 | std::views::filter([this, minor_id = id](const types::id_type major_id) {
                       return detail::contains(this->_storage[major_id], minor_id);
                   });
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] types::size_type _size(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // size major
            return this->_storage[id].size();
        }
        else { // size minor
            types::size_type size = 0uz;
            for (const auto segment : this->_storage)
                if (detail::contains(segment, id))
                    ++size;
            return size;
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] std::vector<types::size_type> _size_map(const types::size_type n_elements
    ) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // size major
            std::vector<types::size_type> size_map(n_elements, 0ull);

            const std::size_t n_segments = this->_storage.size();
            const auto offsets = this->_storage.offsets_view();

            for (std::size_t i = 0uz; i < n_segments; ++i)
                size_map[i] = offsets[i + 1uz] - offsets[i];
            return size_map;
        }
        else { // size minor
            std::vector<types::size_type> size_map(n_elements, 0ull);
            for (const auto minor_id : this->_storage.data_view())
                ++size_map[static_cast<std::size_t>(minor_id)];
            return size_map;
        }
    }

    storage_type _storage;
};

template <type_traits::c_hypergraph_asymmetric_layout_tag LayoutTag>
class flat_incidence_list<hgl::bf_directed_t, LayoutTag> final {
public:
    using directional_tag = hgl::bf_directed_t;
    using layout_tag = LayoutTag;

    flat_incidence_list(const flat_incidence_list&) = delete;
    flat_incidence_list& operator=(const flat_incidence_list&) = delete;

    flat_incidence_list() = default;

    flat_incidence_list(const types::size_type n_vertices, const types::size_type n_hyperedges)
    : _tail_storage{layout_tag::major(n_vertices, n_hyperedges)},
      _head_storage{layout_tag::major(n_vertices, n_hyperedges)} {}

    flat_incidence_list(flat_incidence_list&&) = default;
    flat_incidence_list& operator=(flat_incidence_list&&) = default;

    ~flat_incidence_list() = default;

    // --- vertex methods : general ---

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        this->_add<impl::element_type::vertex>(n);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        this->_remove<impl::element_type::vertex>(vertex_id);
    }

    // --- vertex methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_get<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        return this->_size<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] std::vector<types::size_type> degree_map(const types::size_type n_vertices
    ) const noexcept {
        return this->_size_map<impl::element_type::vertex>(n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_get<impl::element_type::vertex>(
            vertex_id, &flat_incidence_list::_tail_storage
        );
    }

    [[nodiscard]] types::size_type out_degree(const types::id_type vertex_id) const noexcept {
        return this->_size<impl::element_type::vertex>(
            vertex_id, &flat_incidence_list::_tail_storage
        );
    }

    [[nodiscard]] std::vector<types::size_type> out_degree_map(const types::size_type n_vertices
    ) const noexcept {
        return this->_size_map<impl::element_type::vertex>(
            n_vertices, &flat_incidence_list::_tail_storage
        );
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_get<impl::element_type::vertex>(
            vertex_id, &flat_incidence_list::_head_storage
        );
    }

    [[nodiscard]] types::size_type in_degree(const types::id_type vertex_id) const noexcept {
        return this->_size<impl::element_type::vertex>(
            vertex_id, &flat_incidence_list::_head_storage
        );
    }

    [[nodiscard]] std::vector<types::size_type> in_degree_map(const types::size_type n_vertices
    ) const noexcept {
        return this->_size_map<impl::element_type::vertex>(
            n_vertices, &flat_incidence_list::_head_storage
        );
    }

    // --- hyperedge methods : general ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        this->_add<impl::element_type::hyperedge>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        this->_remove<impl::element_type::hyperedge>(hyperedge_id);
    }

    // --- hyperedge methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_get<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_size<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] std::vector<types::size_type> hyperedge_size_map(
        const types::size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<impl::element_type::hyperedge>(n_hyperedges);
    }

    [[nodiscard]] gl_attr_force_inline auto tail_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_get<impl::element_type::hyperedge>(
            hyperedge_id, &flat_incidence_list::_tail_storage
        );
    }

    [[nodiscard]] types::size_type tail_size(const types::id_type hyperedge_id) const noexcept {
        return this->_size<impl::element_type::hyperedge>(
            hyperedge_id, &flat_incidence_list::_tail_storage
        );
    }

    [[nodiscard]] std::vector<types::size_type> tail_size_map(const types::size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<impl::element_type::hyperedge>(
            n_hyperedges, &flat_incidence_list::_tail_storage
        );
    }

    [[nodiscard]] gl_attr_force_inline auto head_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_get<impl::element_type::hyperedge>(
            hyperedge_id, &flat_incidence_list::_head_storage
        );
    }

    [[nodiscard]] types::size_type head_size(const types::id_type hyperedge_id) const noexcept {
        return this->_size<impl::element_type::hyperedge>(
            hyperedge_id, &flat_incidence_list::_head_storage
        );
    }

    [[nodiscard]] std::vector<types::size_type> head_size_map(const types::size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<impl::element_type::hyperedge>(
            n_hyperedges, &flat_incidence_list::_head_storage
        );
    }

    // --- binding methods ---

    gl_attr_force_inline void bind_tail(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_remove_no_align(this->_head_storage, major_id, minor_id);
        detail::unique_insert(this->_tail_storage, major_id, minor_id);
    }

    gl_attr_force_inline void bind_head(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_remove_no_align(this->_tail_storage, major_id, minor_id);
        detail::unique_insert(this->_head_storage, major_id, minor_id);
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_remove_no_align(this->_tail_storage, major_id, minor_id);
        this->_remove_no_align(this->_head_storage, major_id, minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return detail::contains(this->_tail_storage[major_id], minor_id)
            or detail::contains(this->_head_storage[major_id], minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return detail::contains(this->_tail_storage[major_id], minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return detail::contains(this->_head_storage[major_id], minor_id);
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_flat_incidence_list;
#endif

private:
    using element_type = types::id_type;
    using storage_type = types::segmented_vector<element_type>;
    using storage_segment_type = typename storage_type::segment_type;
    using storage_const_segment_type = typename storage_type::const_segment_type;

    template <typename Projection>
    [[nodiscard]] gl_attr_force_inline auto _storage_view(
        const types::id_type id, const Projection storage_proj
    ) const noexcept {
        if constexpr (std::same_as<Projection, std::identity>) {
            // TODO: use std::views::concat (C++26)
            // NOTE: This is safe because the range operator | creates an owning view over the array
            return std::array<storage_const_segment_type, 2>{
                       this->_tail_storage[id], this->_head_storage[id]
                   }
                 | std::views::join;
        }
        else {
            return std::invoke(storage_proj, this)[id];
        }
    }

    template <impl::element_type Element>
    void _add(const types::size_type n) noexcept {
        if constexpr (Element == layout_tag::major_element) {
            this->_tail_storage.resize(this->_tail_storage.size() + n);
            this->_head_storage.resize(this->_head_storage.size() + n);
        }
    }

    template <impl::element_type Element>
    void _remove(const types::id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) {
            this->_tail_storage.erase(id);
            this->_head_storage.erase(id);
        }
        else {
            detail::remove_minor(this->_tail_storage, id);
            detail::remove_minor(this->_head_storage, id);
        }
    }

    void _remove_no_align(
        storage_type& storage, const types::id_type major_id, const types::id_type minor_id
    ) noexcept {
        const auto segment = storage[major_id];
        const auto minor_it = std::ranges::lower_bound(segment, minor_id);
        if (minor_it != segment.end() and *minor_it == minor_id) {
            const auto pos =
                static_cast<types::size_type>(std::distance(segment.begin(), minor_it));
            storage.erase(major_id, pos);
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _get(const types::id_type id) const noexcept {
        // TODO
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _get(const types::id_type id, const auto&& storage_proj)
        const noexcept {
        // TODO
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline types::size_type _size(const types::id_type id
    ) const noexcept {
        // TODO
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline types::size_type _size(
        const types::id_type id, const auto&& storage_proj
    ) const noexcept {
        // TODO
    }

    template <impl::element_type Element>
    [[nodiscard]] std::vector<types::size_type> _size_map(const types::size_type n_elements
    ) const noexcept {
        // TODO
    }

    template <impl::element_type Element>
    [[nodiscard]] std::vector<types::size_type> _size_map(
        const types::size_type n_elements, const auto&& storage_proj
    ) const noexcept {
        // TODO
    }

    storage_type _tail_storage;
    storage_type _head_storage;
};

// template <type_traits::c_hypergraph_directional_tag DirectionalTag>
// class flat_incidence_list<DirectionalTag, bidirectional_t> final {
// public:
//     using directional_tag = DirectionalTag;
//     using layout_tag = bidirectional_t;

//     flat_incidence_list(const flat_incidence_list&) = delete;
//     flat_incidence_list& operator=(const flat_incidence_list&) = delete;

//     flat_incidence_list() = default;

//     flat_incidence_list(const types::size_type n_vertices, const types::size_type n_hyperedges)
//     : _v_list{n_vertices, n_hyperedges}, _e_list{n_vertices, n_hyperedges} {}

//     flat_incidence_list(flat_incidence_list&&) = default;
//     flat_incidence_list& operator=(flat_incidence_list&&) = default;

//     ~flat_incidence_list() = default;

//     // --- vertex methods : general ---

//     gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
//         this->_v_list.add_vertices(n);
//         this->_e_list.add_vertices(n);
//     }

//     gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
//         this->_v_list.remove_vertex(vertex_id);
//         this->_e_list.remove_vertex(vertex_id);
//     }

//     // --- vertex methods : incidence queries ---

//     [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
//     ) const noexcept {
//         return this->_v_list.incident_hyperedges(vertex_id);
//     }

//     [[nodiscard]] gl_attr_force_inline types::size_type degree(const types::id_type vertex_id
//     ) const noexcept {
//         return this->_v_list.degree(vertex_id);
//     }

//     [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> degree_map(
//         const types::size_type n_vertices
//     ) const noexcept {
//         return this->_v_list.degree_map(n_vertices);
//     }

//     [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const types::id_type vertex_id
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_v_list.out_hyperedges(vertex_id);
//     }

//     [[nodiscard]] types::size_type out_degree(const types::id_type vertex_id) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_v_list.out_degree(vertex_id);
//     }

//     [[nodiscard]] std::vector<types::size_type> out_degree_map(const types::size_type n_vertices
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_v_list.out_degree_map(n_vertices);
//     }

//     [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const types::id_type vertex_id
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_v_list.in_hyperedges(vertex_id);
//     }

//     [[nodiscard]] types::size_type in_degree(const types::id_type vertex_id) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_v_list.in_degree(vertex_id);
//     }

//     [[nodiscard]] std::vector<types::size_type> in_degree_map(const types::size_type n_vertices
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_v_list.in_degree_map(n_vertices);
//     }

//     // --- hyperedge methods : general ---

//     gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
//         this->_v_list.add_hyperedges(n);
//         this->_e_list.add_hyperedges(n);
//     }

//     gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
//         this->_v_list.remove_hyperedge(hyperedge_id);
//         this->_e_list.remove_hyperedge(hyperedge_id);
//     }

//     // --- hyperedge methods : incidence queries ---

//     [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
//     ) const noexcept {
//         return this->_e_list.incident_vertices(hyperedge_id);
//     }

//     [[nodiscard]] gl_attr_force_inline types::size_type hyperedge_size(
//         const types::id_type hyperedge_id
//     ) const noexcept {
//         return this->_e_list.hyperedge_size(hyperedge_id);
//     }

//     [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> hyperedge_size_map(
//         const types::size_type n_hyperedges
//     ) const noexcept {
//         return this->_e_list.hyperedge_size_map(n_hyperedges);
//     }

//     [[nodiscard]] gl_attr_force_inline auto tail_vertices(const types::id_type hyperedge_id
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_e_list.tail_vertices(hyperedge_id);
//     }

//     [[nodiscard]] types::size_type tail_size(const types::id_type hyperedge_id) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_e_list.tail_size(hyperedge_id);
//     }

//     [[nodiscard]] std::vector<types::size_type> tail_size_map(const types::size_type n_hyperedges
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_e_list.tail_size_map(n_hyperedges);
//     }

//     [[nodiscard]] gl_attr_force_inline auto head_vertices(const types::id_type hyperedge_id
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_e_list.head_vertices(hyperedge_id);
//     }

//     [[nodiscard]] types::size_type head_size(const types::id_type hyperedge_id) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_e_list.head_size(hyperedge_id);
//     }

//     [[nodiscard]] std::vector<types::size_type> head_size_map(const types::size_type n_hyperedges
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         return this->_e_list.head_size_map(n_hyperedges);
//     }

//     // --- binding methods ---

//     gl_attr_force_inline void bind(
//         const types::id_type vertex_id, const types::id_type hyperedge_id
//     ) noexcept
//     requires std::same_as<DirectionalTag, hgl::undirected_t>
//     {
//         this->_v_list.bind(vertex_id, hyperedge_id);
//         this->_e_list.bind(vertex_id, hyperedge_id);
//     }

//     gl_attr_force_inline void unbind(
//         const types::id_type vertex_id, const types::id_type hyperedge_id
//     ) noexcept {
//         this->_v_list.unbind(vertex_id, hyperedge_id);
//         this->_e_list.unbind(vertex_id, hyperedge_id);
//     }

//     [[nodiscard]] gl_attr_force_inline bool are_bound(
//         const types::id_type vertex_id, const types::id_type hyperedge_id
//     ) const noexcept {
//         if (this->degree(vertex_id) <= this->hyperedge_size(hyperedge_id))
//             return this->_v_list.are_bound(vertex_id, hyperedge_id);
//         else
//             return this->_e_list.are_bound(vertex_id, hyperedge_id);
//     }

//     gl_attr_force_inline void bind_tail(
//         const types::id_type vertex_id, const types::id_type hyperedge_id
//     ) noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         this->_v_list.bind_tail(vertex_id, hyperedge_id);
//         this->_e_list.bind_tail(vertex_id, hyperedge_id);
//     }

//     gl_attr_force_inline void bind_head(
//         const types::id_type vertex_id, const types::id_type hyperedge_id
//     ) noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         this->_v_list.bind_head(vertex_id, hyperedge_id);
//         this->_e_list.bind_head(vertex_id, hyperedge_id);
//     }

//     [[nodiscard]] gl_attr_force_inline bool is_tail(
//         const types::id_type vertex_id, const types::id_type hyperedge_id
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         if (this->out_degree(vertex_id) <= this->tail_size(hyperedge_id))
//             return this->_v_list.is_tail(vertex_id, hyperedge_id);
//         else
//             return this->_e_list.is_tail(vertex_id, hyperedge_id);
//     }

//     [[nodiscard]] gl_attr_force_inline bool is_head(
//         const types::id_type vertex_id, const types::id_type hyperedge_id
//     ) const noexcept
//     requires std::same_as<DirectionalTag, hgl::bf_directed_t>
//     {
//         if (this->in_degree(vertex_id) <= this->head_size(hyperedge_id))
//             return this->_v_list.is_head(vertex_id, hyperedge_id);
//         else
//             return this->_e_list.is_head(vertex_id, hyperedge_id);
//     }

// #ifdef HGL_TESTING
//     friend struct hgl_testing::test_flat_incidence_list;
// #endif

// private:
//     using vertex_major_list = flat_incidence_list<DirectionalTag, vertex_major_t>;
//     using hyperedge_major_list = flat_incidence_list<DirectionalTag, hyperedge_major_t>;

//     vertex_major_list _v_list;
//     hyperedge_major_list _e_list;
// };

} // namespace hgl::impl
