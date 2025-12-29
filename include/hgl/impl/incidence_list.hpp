// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/directional_tags.hpp"
#include "hgl/impl/layout_tags.hpp"
#include "hgl/types/types.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_incidence_list;
} // namespace hgl_testing
#endif

namespace hgl::impl {

template <
    type_traits::c_hypergraph_directional_tag DirectionalTag,
    type_traits::c_hypergraph_layout_tag LayoutTag>
class incidence_list;

template <type_traits::c_hypergraph_layout_tag LayoutTag>
class incidence_list<hgl::undirected_t, LayoutTag> final {
public:
    using directional_tag = hgl::undirected_t;
    using layout_tag = LayoutTag;

    incidence_list(const incidence_list&) = delete;
    incidence_list& operator=(const incidence_list&) = delete;

    incidence_list() = default;

    incidence_list(const types::size_type n_vertices, const types::size_type n_hyperedges)
    : _major_storage{layout_tag::major(n_vertices, n_hyperedges)} {}

    incidence_list(incidence_list&&) = default;
    incidence_list& operator=(incidence_list&&) = default;

    ~incidence_list() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            this->_major_storage.resize(this->_major_storage.size() + n);
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

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            this->_major_storage.resize(this->_major_storage.size() + n);
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

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        auto& minor_storage = this->_major_storage[major_id];

        // insert the id at the correct position to keep the minor-id collection sorted
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        if (minor_it == minor_storage.end() or *minor_it != minor_id)
            minor_storage.insert(minor_it, minor_id);
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        auto& minor_storage = this->_major_storage[major_id];
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        if (minor_it != minor_storage.end() and *minor_it == minor_id)
            minor_storage.erase(minor_it);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        return this->_contains(
            this->_major_storage[layout_tag::major(vertex_id, hyperedge_id)],
            layout_tag::minor(vertex_id, hyperedge_id)
        );
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_list;
#endif

private:
    using minor_element_type = types::id_type;
    using minor_storage_type = std::vector<minor_element_type>;
    using major_element_type = minor_storage_type;
    using major_storage_type = std::vector<major_element_type>;

    template <impl::element_type Element>
    void _remove(const types::id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) { // remove major
            this->_major_storage.erase(
                this->_major_storage.begin() + static_cast<std::ptrdiff_t>(id)
            );
        }
        else { // remove minor
            for (auto& minor_storage : this->_major_storage) {
                auto minor_it = std::ranges::lower_bound(minor_storage, id);
                if (minor_it != minor_storage.end() and *minor_it == id)
                    minor_it = minor_storage.erase(minor_it); // unbind the element
                while (minor_it != minor_storage.end())
                    --(*minor_it++); // decrement ids > id (minor)
            }
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _incident_with(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // incident with major
            return std::views::all(this->_major_storage[id]);
        }
        else { // incident with minor
            return std::views::iota(0uz, this->_major_storage.size())
                 | std::views::filter([this, minor_id = id](types::id_type major_id) {
                       return this->_contains(this->_major_storage[major_id], minor_id);
                   });
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] types::size_type _size(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // size major
            return this->_major_storage[id].size();
        }
        else { // size minor
            types::size_type size = 0uz;
            for (const auto& major_el : this->_major_storage)
                if (this->_contains(major_el, id))
                    ++size;
            return size;
        }
    }

    [[nodiscard]] bool _contains(
        const minor_storage_type& minor_storage, const types::id_type minor_id
    ) const noexcept {
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        return minor_it != minor_storage.end() and *minor_it == minor_id;
    }

    major_storage_type _major_storage;
};

template <type_traits::c_hypergraph_layout_tag LayoutTag>
class incidence_list<hgl::bf_directed_t, LayoutTag> final {
public:
    using directional_tag = hgl::bf_directed_t;
    using layout_tag = LayoutTag;

    incidence_list(const incidence_list&) = delete;
    incidence_list& operator=(const incidence_list&) = delete;

    incidence_list() = default;

    incidence_list(const types::size_type n_vertices, const types::size_type n_hyperedges)
    : _major_storage{layout_tag::major(n_vertices, n_hyperedges)} {}

    incidence_list(incidence_list&&) = default;
    incidence_list& operator=(incidence_list&&) = default;

    ~incidence_list() = default;

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

    [[nodiscard]] gl_attr_force_inline auto outgoing_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_get_tail<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] types::size_type out_degree(const types::id_type vertex_id) const noexcept {
        return this->_tail_size<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incoming_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return this->_get_head<impl::element_type::vertex>(vertex_id);
    }

    [[nodiscard]] types::size_type in_degree(const types::id_type vertex_id) const noexcept {
        return this->_head_size<impl::element_type::vertex>(vertex_id);
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

    [[nodiscard]] gl_attr_force_inline auto tail_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_get_tail<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] types::size_type tail_size(const types::id_type hyperedge_id) const noexcept {
        return this->_tail_size<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto head_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return this->_get_head<impl::element_type::hyperedge>(hyperedge_id);
    }

    [[nodiscard]] types::size_type head_size(const types::id_type hyperedge_id) const noexcept {
        return this->_head_size<impl::element_type::hyperedge>(hyperedge_id);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind_tail(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        // TODO: validate if minor_id is not in head
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_unique_insert(this->_major_storage[major_id].tail, minor_id);
    }

    gl_attr_force_inline void bind_head(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        // TODO: validate if minor_id is not in tail
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_unique_insert(this->_major_storage[major_id].head, minor_id);
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        auto& entry = this->_major_storage[major_id];
        this->_remove_no_align(entry.tail, minor_id);
        this->_remove_no_align(entry.head, minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_contains(this->_major_storage[major_id], minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_contains(this->_major_storage[major_id].tail, minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_contains(this->_major_storage[major_id].head, minor_id);
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_list;
#endif

private:
    using minor_element_type = types::id_type;
    using minor_storage_type = std::vector<minor_element_type>;

    /**
     * For hyperedge-major representation: tail = T(e), head = H(e)
     * For vertex-major representation:
     * - tail = {e in E(G) : v in T(e)}
     * - head = {e in E(G) : v in H(e)}
     * where G is the hypergraph and v is the vertex entry in the outer list
     */
    struct major_element_type {
        minor_storage_type tail;
        minor_storage_type head;
    };

    using major_storage_type = std::vector<major_element_type>;

    template <impl::element_type Element>
    void _add(const types::size_type n) noexcept {
        if constexpr (Element == layout_tag::major_element) // add major
            this->_major_storage.resize(this->_major_storage.size() + n);
    }

    template <impl::element_type Element>
    void _remove(const types::id_type id) noexcept {
        if constexpr (Element == layout_tag::major_element) { // remove major
            this->_major_storage.erase(
                this->_major_storage.begin() + static_cast<std::ptrdiff_t>(id)
            );
        }
        else { // remove minor
            for (auto& minor_storage : this->_major_storage) {
                this->_remove(minor_storage.tail, id);
                this->_remove(minor_storage.head, id);
            }
        }
    }

    void _remove(minor_storage_type& minor_storage, const types::id_type id) noexcept {
        auto minor_it = this->_remove_no_align(minor_storage, id);
        while (minor_it != minor_storage.end())
            --(*minor_it++); // decrement ids > id
    }

    auto _remove_no_align(minor_storage_type& minor_storage, const types::id_type id) noexcept {
        auto minor_it = std::ranges::lower_bound(minor_storage, id);
        if (minor_it != minor_storage.end() and *minor_it == id)
            minor_it = minor_storage.erase(minor_it); // unbind the element
        return minor_it;
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _get(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // get major
            const auto& entry = this->_major_storage[id];
            // TODO: use std::views::concat (C++26)
            // NOTE: This is safe because the range operator | creates an owning view over the array
            return std::array<std::span<const minor_element_type>, 2>{entry.tail, entry.head}
                 | std::views::join;
        }
        else { // get minor
            return std::views::iota(0uz, this->_major_storage.size())
                 | std::views::filter([this, minor_id = id](types::id_type major_id) {
                       return this->_contains(this->_major_storage[major_id], minor_id);
                   });
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _get_tail(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // get major
            return std::views::all(this->_major_storage[id].tail);
        }
        else { // get minor
            return std::views::iota(0uz, this->_major_storage.size())
                 | std::views::filter([this, minor_id = id](types::id_type major_id) {
                       return this->_contains(this->_major_storage[major_id].tail, minor_id);
                   });
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline auto _get_head(const types::id_type id) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // get major
            return std::views::all(this->_major_storage[id].head);
        }
        else { // get minor
            return std::views::iota(0uz, this->_major_storage.size())
                 | std::views::filter([this, minor_id = id](types::id_type major_id) {
                       return this->_contains(this->_major_storage[major_id].head, minor_id);
                   });
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline types::size_type _size(const types::id_type id
    ) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // size major
            const auto& entry = this->_major_storage[id];
            return entry.tail.size() + entry.head.size();
        }
        else { // size minor
            types::size_type size = 0uz;
            for (const auto& major_el : this->_major_storage)
                if (this->_contains(major_el, id))
                    ++size;
            return size;
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline types::size_type _tail_size(const types::id_type id
    ) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // size major
            return this->_major_storage[id].tail.size();
        }
        else { // size minor
            types::size_type size = 0uz;
            for (const auto& major_el : this->_major_storage)
                if (this->_contains(major_el.tail, id))
                    ++size;
            return size;
        }
    }

    template <impl::element_type Element>
    [[nodiscard]] gl_attr_force_inline types::size_type _head_size(const types::id_type id
    ) const noexcept {
        if constexpr (Element == layout_tag::major_element) { // size major
            return this->_major_storage[id].head.size();
        }
        else { // size minor
            types::size_type size = 0uz;
            for (const auto& major_el : this->_major_storage)
                if (this->_contains(major_el.head, id))
                    ++size;
            return size;
        }
    }

    void _unique_insert(minor_storage_type& minor_storage, const types::id_type id) noexcept {
        const auto minor_it = std::ranges::lower_bound(minor_storage, id);
        if (minor_it == minor_storage.end() or *minor_it != id)
            minor_storage.insert(minor_it, id);
    }

    [[nodiscard]] gl_attr_force_inline bool _contains(
        const major_element_type& major_el, const types::id_type id
    ) const noexcept {
        return this->_contains(major_el.tail, id) or this->_contains(major_el.head, id);
    }

    [[nodiscard]] bool _contains(const minor_storage_type& minor_storage, const types::id_type id)
        const noexcept {
        const auto minor_it = std::ranges::lower_bound(minor_storage, id);
        return minor_it != minor_storage.end() and *minor_it == id;
    }

    major_storage_type _major_storage;
};

} // namespace hgl::impl
