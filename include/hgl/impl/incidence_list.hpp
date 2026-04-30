// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/constants.hpp"
#include "hgl/decl/repr_tags.hpp"
#include "hgl/directional_tags.hpp"
#include "hgl/repr/layout_tags.hpp"
#include "hgl/types.hpp"
#include "hgl/util.hpp"

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <ranges>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_incidence_list;
} // namespace hgl_testing
#endif

namespace hgl {

namespace detail {

template <traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph_repr_tag SourceReprTag>
struct to_impl;

} // namespace detail

namespace impl {

template <traits::c_hypergraph_directional_tag DirectionalTag, traits::c_hypergraph_list_repr ReprTag>
class incidence_list;

template <traits::c_hypergraph_list_repr ReprTag>
requires traits::c_hypergraph_asymmetric_layout_tag<typename ReprTag::layout_tag>
class incidence_list<hgl::undirected_t, ReprTag> final {
public:
    using directional_tag = hgl::undirected_t;
    using representation_tag = ReprTag;
    using layout_tag = typename representation_tag::layout_tag;
    using id_type = typename representation_tag::id_type;

    incidence_list() = default;

    incidence_list(const size_type n_vertices, const size_type n_hyperedges)
    : _major_storage{layout_tag::major(n_vertices, n_hyperedges)} {}

    incidence_list(const incidence_list&) = default;
    incidence_list& operator=(const incidence_list&) = default;

    incidence_list(incidence_list&&) noexcept = default;
    incidence_list& operator=(incidence_list&&) noexcept = default;

    ~incidence_list() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const size_type n) noexcept {
        this->_add<vertex_t>(n);
    }

    gl_attr_force_inline void remove_vertex(const id_type vertex_id) noexcept {
        this->_remove<vertex_t>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const id_type vertex_id
    ) const noexcept {
        return this->_incident_with<vertex_t>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type degree(const id_type vertex_id) const noexcept {
        return this->_size<vertex_t>(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map(const size_type n_vertices
    ) const noexcept {
        return this->_size_map<vertex_t>(n_vertices);
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const size_type n) noexcept {
        this->_add<hyperedge_t>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) noexcept {
        this->_remove<hyperedge_t>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_incident_with<hyperedge_t>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type hyperedge_size(const id_type hyperedge_id
    ) const noexcept {
        return this->_size<hyperedge_t>(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> hyperedge_size_map(
        const size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<hyperedge_t>(n_hyperedges);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        auto& minor_storage = this->_major_storage[to_idx(major_id)];

        // insert the id at the correct position to keep the minor-id collection sorted
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        if (minor_it == minor_storage.end() or *minor_it != minor_id)
            minor_storage.insert(minor_it, minor_id);
    }

    gl_attr_force_inline void unbind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        auto& minor_storage = this->_major_storage[to_idx(major_id)];
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        if (minor_it != minor_storage.end() and *minor_it == minor_id)
            minor_storage.erase(minor_it);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        return this->_contains(
            this->_major_storage[to_idx(layout_tag::major(vertex_id, hyperedge_id))],
            layout_tag::minor(vertex_id, hyperedge_id)
        );
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const incidence_list&, const incidence_list&) = default;

    // --- friend declarations ---

    template <
        traits::c_hypergraph_repr_tag TargetReprTag,
        traits::c_hypergraph_repr_tag SourceReprTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_list;
#endif

private:
    using minor_element_type = id_type;
    using minor_storage_type = std::vector<minor_element_type>;
    using major_element_type = minor_storage_type;
    using major_storage_type = std::vector<major_element_type>;

    template <traits::c_hypergraph_element_tag Element>
    void _add(const size_type n) noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) // add major
            this->_major_storage.resize(this->_major_storage.size() + n);
    }

    template <traits::c_hypergraph_element_tag Element>
    void _remove(const id_type id) noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // remove major
            this->_major_storage.erase(this->_major_storage.begin() + to_diff(id));
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

    template <traits::c_hypergraph_element_tag Element>
    [[nodiscard]] gl_attr_force_inline auto _incident_with(const id_type id) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // incident with major
            return std::views::all(this->_major_storage[to_idx(id)]);
        }
        else { // incident with minor
            return std::views::iota(initial_id_v<id_type>, this->_major_storage.size())
                 | std::views::filter([this, minor_id = id](id_type major_id) {
                       return this->_contains(this->_major_storage[to_idx(major_id)], minor_id);
                   });
        }
    }

    template <traits::c_hypergraph_element_tag Element>
    [[nodiscard]] size_type _size(const id_type id) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // size major
            return this->_major_storage[to_idx(id)].size();
        }
        else { // size minor
            size_type size = 0uz;
            for (const auto& major_el : this->_major_storage)
                if (this->_contains(major_el, id))
                    ++size;
            return size;
        }
    }

    template <traits::c_hypergraph_element_tag Element>
    [[nodiscard]] std::vector<size_type> _size_map(const size_type n_elements) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // size major
            auto size_map = this->_major_storage | std::views::transform(&major_element_type::size)
                          | std::ranges::to<std::vector<size_type>>();
            size_map.resize(n_elements, 0uz);
            return size_map;
        }
        else { // size minor
            std::vector<size_type> size_map(n_elements, 0uz);
            for (const auto& major_entry : this->_major_storage)
                for (const auto minor_id : major_entry)
                    ++size_map[to_idx(minor_id)];
            return size_map;
        }
    }

    [[nodiscard]] bool _contains(const minor_storage_type& minor_storage, const id_type minor_id)
        const noexcept {
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        return minor_it != minor_storage.end() and *minor_it == minor_id;
    }

    major_storage_type _major_storage;
};

template <traits::c_hypergraph_list_repr ReprTag>
requires traits::c_hypergraph_asymmetric_layout_tag<typename ReprTag::layout_tag>
class incidence_list<hgl::bf_directed_t, ReprTag> final {
public:
    using directional_tag = hgl::bf_directed_t;
    using representation_tag = ReprTag;
    using layout_tag = typename representation_tag::layout_tag;
    using id_type = typename representation_tag::id_type;


    incidence_list() = default;

    incidence_list(const size_type n_vertices, const size_type n_hyperedges)
    : _tail_storage{layout_tag::major(n_vertices, n_hyperedges)},
      _head_storage{layout_tag::major(n_vertices, n_hyperedges)} {}

    incidence_list(const incidence_list&) = default;
    incidence_list& operator=(const incidence_list&) = default;

    incidence_list(incidence_list&&) noexcept = default;
    incidence_list& operator=(incidence_list&&) noexcept = default;

    ~incidence_list() = default;

    // --- vertex methods : general ---

    gl_attr_force_inline void add_vertices(const size_type n) noexcept {
        this->_add<vertex_t>(n);
    }

    gl_attr_force_inline void remove_vertex(const id_type vertex_id) noexcept {
        this->_remove<vertex_t>(vertex_id);
    }

    // --- vertex methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const id_type vertex_id
    ) const noexcept {
        return this->_get<vertex_t>(vertex_id);
    }

    [[nodiscard]] size_type degree(const id_type vertex_id) const noexcept {
        return this->_size<vertex_t>(vertex_id);
    }

    [[nodiscard]] std::vector<size_type> degree_map(const size_type n_vertices) const noexcept {
        return this->_size_map<vertex_t>(n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const id_type vertex_id) const noexcept {
        return this->_get<vertex_t>(vertex_id, &incidence_list::_tail_storage);
    }

    [[nodiscard]] size_type out_degree(const id_type vertex_id) const noexcept {
        return this->_size<vertex_t>(vertex_id, &incidence_list::_tail_storage);
    }

    [[nodiscard]] std::vector<size_type> out_degree_map(const size_type n_vertices) const noexcept {
        return this->_size_map<vertex_t>(n_vertices, &incidence_list::_tail_storage);
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const id_type vertex_id) const noexcept {
        return this->_get<vertex_t>(vertex_id, &incidence_list::_head_storage);
    }

    [[nodiscard]] size_type in_degree(const id_type vertex_id) const noexcept {
        return this->_size<vertex_t>(vertex_id, &incidence_list::_head_storage);
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(const size_type n_vertices) const noexcept {
        return this->_size_map<vertex_t>(n_vertices, &incidence_list::_head_storage);
    }

    // --- hyperedge methods : general ---

    gl_attr_force_inline void add_hyperedges(const size_type n) noexcept {
        this->_add<hyperedge_t>(n);
    }

    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) noexcept {
        this->_remove<hyperedge_t>(hyperedge_id);
    }

    // --- hyperedge methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_get<hyperedge_t>(hyperedge_id);
    }

    [[nodiscard]] size_type hyperedge_size(const id_type hyperedge_id) const noexcept {
        return this->_size<hyperedge_t>(hyperedge_id);
    }

    [[nodiscard]] std::vector<size_type> hyperedge_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<hyperedge_t>(n_hyperedges);
    }

    [[nodiscard]] gl_attr_force_inline auto tail(const id_type hyperedge_id) const noexcept {
        return this->_get<hyperedge_t>(hyperedge_id, &incidence_list::_tail_storage);
    }

    [[nodiscard]] size_type tail_size(const id_type hyperedge_id) const noexcept {
        return this->_size<hyperedge_t>(hyperedge_id, &incidence_list::_tail_storage);
    }

    [[nodiscard]] std::vector<size_type> tail_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<hyperedge_t>(n_hyperedges, &incidence_list::_tail_storage);
    }

    [[nodiscard]] gl_attr_force_inline auto head(const id_type hyperedge_id) const noexcept {
        return this->_get<hyperedge_t>(hyperedge_id, &incidence_list::_head_storage);
    }

    [[nodiscard]] size_type head_size(const id_type hyperedge_id) const noexcept {
        return this->_size<hyperedge_t>(hyperedge_id, &incidence_list::_head_storage);
    }

    [[nodiscard]] std::vector<size_type> head_size_map(const size_type n_hyperedges
    ) const noexcept {
        return this->_size_map<hyperedge_t>(n_hyperedges, &incidence_list::_head_storage);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind_tail(
        const id_type vertex_id, const id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        const auto major_idx = to_idx(major_id);
        this->_remove_no_align(this->_head_storage[major_idx], minor_id);
        this->_unique_insert(this->_tail_storage[major_idx], minor_id);
    }

    gl_attr_force_inline void bind_head(
        const id_type vertex_id, const id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        const auto major_idx = to_idx(major_id);
        this->_remove_no_align(this->_tail_storage[major_idx], minor_id);
        this->_unique_insert(this->_head_storage[major_idx], minor_id);
    }

    gl_attr_force_inline void unbind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        const auto major_idx = to_idx(major_id);
        this->_remove_no_align(this->_tail_storage[major_idx], minor_id);
        this->_remove_no_align(this->_head_storage[major_idx], minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        const auto major_idx = to_idx(major_id);
        return this->_contains(this->_tail_storage[major_idx], minor_id)
            or this->_contains(this->_head_storage[major_idx], minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_contains(this->_tail_storage[to_idx(major_id)], minor_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_contains(this->_head_storage[to_idx(major_id)], minor_id);
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const incidence_list&, const incidence_list&) = default;

    // --- friend declarations ---

    template <
        traits::c_hypergraph_repr_tag TargetReprTag,
        traits::c_hypergraph_repr_tag SourceReprTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_list;
#endif

private:
    using minor_element_type = id_type;
    using minor_storage_type = std::vector<minor_element_type>;
    using major_storage_type = std::vector<minor_storage_type>;

    template <traits::c_hypergraph_element_tag Element>
    void _add(const size_type n) noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // add major
            this->_tail_storage.resize(this->_tail_storage.size() + n);
            this->_head_storage.resize(this->_head_storage.size() + n);
        }
    }

    template <traits::c_hypergraph_element_tag Element>
    void _remove(const id_type id) noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // remove major
            this->_tail_storage.erase(this->_tail_storage.begin() + to_diff(id));
            this->_head_storage.erase(this->_head_storage.begin() + to_diff(id));
        }
        else { // remove minor
            for (auto& minor_storage : this->_tail_storage)
                this->_remove_minor(minor_storage, id);
            for (auto& minor_storage : this->_head_storage)
                this->_remove_minor(minor_storage, id);
        }
    }

    void _remove_minor(minor_storage_type& minor_storage, const id_type id) noexcept {
        auto minor_it = this->_remove_no_align(minor_storage, id);
        while (minor_it != minor_storage.end())
            --(*minor_it++); // decrement ids > id
    }

    auto _remove_no_align(minor_storage_type& minor_storage, const id_type id) noexcept {
        auto minor_it = std::ranges::lower_bound(minor_storage, id);
        if (minor_it != minor_storage.end() and *minor_it == id)
            minor_it = minor_storage.erase(minor_it); // unbind the element
        return minor_it;
    }

    template <traits::c_hypergraph_element_tag Element>
    [[nodiscard]] gl_attr_force_inline auto _get(const id_type id) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // get major
            const auto idx = to_idx(id);
            return util::concat(this->_tail_storage[idx], this->_head_storage[idx]);
        }
        else { // get minor
            return std::views::iota(initial_id_v<id_type>, this->_tail_storage.size())
                 | std::views::filter([this, minor_id = id](id_type major_id) {
                       return this->_contains(this->_tail_storage[to_idx(major_id)], minor_id)
                           or this->_contains(this->_head_storage[to_idx(major_id)], minor_id);
                   });
        }
    }

    template <traits::c_hypergraph_element_tag Element, typename Projection = std::identity>
    [[nodiscard]] gl_attr_force_inline auto _get(const id_type id, const Projection storage_proj)
        const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // get major
            return std::views::all((this->*storage_proj)[to_idx(id)]);
        }
        else { // get minor
            return std::views::iota(initial_id_v<id_type>, this->_tail_storage.size())
                 | std::views::filter(
                       [this, &storage = this->*storage_proj, minor_id = id](id_type major_id) {
                           return this->_contains(storage[to_idx(major_id)], minor_id);
                       }
                 );
        }
    }

    template <traits::c_hypergraph_element_tag Element>
    [[nodiscard]] gl_attr_force_inline size_type _size(const id_type id) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // size major
            const auto idx = to_idx(id);
            return this->_tail_storage[idx].size() + this->_head_storage[idx].size();
        }
        else { // size minor
            size_type size = 0uz;
            for (const auto [ts, hs] : std::views::zip(this->_tail_storage, this->_head_storage)) {
                if (this->_contains(ts, id))
                    ++size;
                if (this->_contains(hs, id))
                    ++size;
            }
            return size;
        }
    }

    template <traits::c_hypergraph_element_tag Element, typename Projection = std::identity>
    [[nodiscard]] gl_attr_force_inline size_type
    _size(const id_type id, const Projection storage_proj) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // size major
            return (this->*storage_proj)[to_idx(id)].size();
        }
        else { // size minor
            size_type size = 0uz;
            for (const auto& minor_storage : this->*storage_proj)
                if (this->_contains(minor_storage, id))
                    ++size;
            return size;
        }
    }

    template <traits::c_hypergraph_element_tag Element>
    [[nodiscard]] std::vector<size_type> _size_map(const size_type n_elements) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // size major
            std::vector<size_type> size_map(n_elements, 0uz);
            const auto n_segments = this->_tail_storage.size();
            for (auto i = 0uz; i < n_segments; ++i)
                size_map[i] = this->_tail_storage[i].size() + this->_head_storage[i].size();
            return size_map;
        }
        else { // size minor
            std::vector<size_type> size_map(n_elements, 0uz);
            for (const auto& minor_storage : this->_tail_storage)
                for (const auto minor_id : minor_storage)
                    ++size_map[to_idx(minor_id)];
            for (const auto& minor_storage : this->_head_storage)
                for (const auto minor_id : minor_storage)
                    ++size_map[to_idx(minor_id)];
            return size_map;
        }
    }

    template <traits::c_hypergraph_element_tag Element, typename Projection = std::identity>
    [[nodiscard]] std::vector<size_type> _size_map(
        const size_type n_elements, const Projection storage_proj
    ) const noexcept {
        if constexpr (std::same_as<Element, major_element_t<layout_tag>>) { // size major
            std::vector<size_type> size_map(n_elements, 0uz);
            const auto& storage = this->*storage_proj;
            const auto n_segments = storage.size();
            for (auto i = 0uz; i < n_segments; ++i)
                size_map[i] = storage[i].size();
            return size_map;
        }
        else { // size minor
            std::vector<size_type> size_map(n_elements, 0uz);
            for (const auto& minor_storage : this->*storage_proj)
                for (const auto minor_id : minor_storage)
                    ++size_map[to_idx(minor_id)];
            return size_map;
        }
    }

    void _unique_insert(minor_storage_type& minor_storage, const id_type id) noexcept {
        const auto minor_it = std::ranges::lower_bound(minor_storage, id);
        if (minor_it == minor_storage.end() or *minor_it != id)
            minor_storage.insert(minor_it, id);
    }

    [[nodiscard]] bool _contains(const minor_storage_type& minor_storage, const id_type id)
        const noexcept {
        const auto minor_it = std::ranges::lower_bound(minor_storage, id);
        return minor_it != minor_storage.end() and *minor_it == id;
    }

    major_storage_type _tail_storage;
    major_storage_type _head_storage;
};

template <traits::c_hypergraph_directional_tag DirectionalTag, traits::c_hypergraph_list_repr ReprTag>
requires std::same_as<typename ReprTag::layout_tag, repr::bidirectional_t>
class incidence_list<DirectionalTag, ReprTag> final {
public:
    using directional_tag = DirectionalTag;
    using representation_tag = ReprTag;
    using layout_tag = typename representation_tag::layout_tag;
    using id_type = typename representation_tag::id_type;


    incidence_list() = default;

    incidence_list(const size_type n_vertices, const size_type n_hyperedges)
    : _v_list{n_vertices, n_hyperedges}, _e_list{n_vertices, n_hyperedges} {}

    incidence_list(const incidence_list&) = default;
    incidence_list& operator=(const incidence_list&) = default;

    incidence_list(incidence_list&&) noexcept = default;
    incidence_list& operator=(incidence_list&&) noexcept = default;

    ~incidence_list() = default;

    // --- vertex methods : general ---

    gl_attr_force_inline void add_vertices(const size_type n) noexcept {
        this->_v_list.add_vertices(n);
        this->_e_list.add_vertices(n);
    }

    gl_attr_force_inline void remove_vertex(const id_type vertex_id) noexcept {
        this->_v_list.remove_vertex(vertex_id);
        this->_e_list.remove_vertex(vertex_id);
    }

    // --- vertex methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const id_type vertex_id
    ) const noexcept {
        return this->_v_list.incident_hyperedges(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type degree(const id_type vertex_id) const noexcept {
        return this->_v_list.degree(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> degree_map(const size_type n_vertices
    ) const noexcept {
        return this->_v_list.degree_map(n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto out_hyperedges(const id_type vertex_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_v_list.out_hyperedges(vertex_id);
    }

    [[nodiscard]] size_type out_degree(const id_type vertex_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_v_list.out_degree(vertex_id);
    }

    [[nodiscard]] std::vector<size_type> out_degree_map(const size_type n_vertices) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_v_list.out_degree_map(n_vertices);
    }

    [[nodiscard]] gl_attr_force_inline auto in_hyperedges(const id_type vertex_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_v_list.in_hyperedges(vertex_id);
    }

    [[nodiscard]] size_type in_degree(const id_type vertex_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_v_list.in_degree(vertex_id);
    }

    [[nodiscard]] std::vector<size_type> in_degree_map(const size_type n_vertices) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_v_list.in_degree_map(n_vertices);
    }

    // --- hyperedge methods : general ---

    gl_attr_force_inline void add_hyperedges(const size_type n) noexcept {
        this->_v_list.add_hyperedges(n);
        this->_e_list.add_hyperedges(n);
    }

    gl_attr_force_inline void remove_hyperedge(const id_type hyperedge_id) noexcept {
        this->_v_list.remove_hyperedge(hyperedge_id);
        this->_e_list.remove_hyperedge(hyperedge_id);
    }

    // --- hyperedge methods : incidence queries ---

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const id_type hyperedge_id
    ) const noexcept {
        return this->_e_list.incident_vertices(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline size_type hyperedge_size(const id_type hyperedge_id
    ) const noexcept {
        return this->_e_list.hyperedge_size(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<size_type> hyperedge_size_map(
        const size_type n_hyperedges
    ) const noexcept {
        return this->_e_list.hyperedge_size_map(n_hyperedges);
    }

    [[nodiscard]] gl_attr_force_inline auto tail(const id_type hyperedge_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_e_list.tail(hyperedge_id);
    }

    [[nodiscard]] size_type tail_size(const id_type hyperedge_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_e_list.tail_size(hyperedge_id);
    }

    [[nodiscard]] std::vector<size_type> tail_size_map(const size_type n_hyperedges) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_e_list.tail_size_map(n_hyperedges);
    }

    [[nodiscard]] gl_attr_force_inline auto head(const id_type hyperedge_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_e_list.head(hyperedge_id);
    }

    [[nodiscard]] size_type head_size(const id_type hyperedge_id) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_e_list.head_size(hyperedge_id);
    }

    [[nodiscard]] std::vector<size_type> head_size_map(const size_type n_hyperedges) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        return this->_e_list.head_size_map(n_hyperedges);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(const id_type vertex_id, const id_type hyperedge_id) noexcept
    requires std::same_as<DirectionalTag, hgl::undirected_t>
    {
        this->_v_list.bind(vertex_id, hyperedge_id);
        this->_e_list.bind(vertex_id, hyperedge_id);
    }

    gl_attr_force_inline void unbind(const id_type vertex_id, const id_type hyperedge_id) noexcept {
        this->_v_list.unbind(vertex_id, hyperedge_id);
        this->_e_list.unbind(vertex_id, hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept {
        if (this->degree(vertex_id) <= this->hyperedge_size(hyperedge_id))
            return this->_v_list.are_bound(vertex_id, hyperedge_id);
        else
            return this->_e_list.are_bound(vertex_id, hyperedge_id);
    }

    gl_attr_force_inline void bind_tail(const id_type vertex_id, const id_type hyperedge_id) noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        this->_v_list.bind_tail(vertex_id, hyperedge_id);
        this->_e_list.bind_tail(vertex_id, hyperedge_id);
    }

    gl_attr_force_inline void bind_head(const id_type vertex_id, const id_type hyperedge_id) noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        this->_v_list.bind_head(vertex_id, hyperedge_id);
        this->_e_list.bind_head(vertex_id, hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_tail(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        if (this->out_degree(vertex_id) <= this->tail_size(hyperedge_id))
            return this->_v_list.is_tail(vertex_id, hyperedge_id);
        else
            return this->_e_list.is_tail(vertex_id, hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_head(
        const id_type vertex_id, const id_type hyperedge_id
    ) const noexcept
    requires std::same_as<DirectionalTag, hgl::bf_directed_t>
    {
        if (this->in_degree(vertex_id) <= this->head_size(hyperedge_id))
            return this->_v_list.is_head(vertex_id, hyperedge_id);
        else
            return this->_e_list.is_head(vertex_id, hyperedge_id);
    }

    // --- comparison ---

    [[nodiscard]] friend bool operator==(const incidence_list&, const incidence_list&) = default;

    // --- friend declarations ---

    template <
        traits::c_hypergraph_repr_tag TargetReprTag,
        traits::c_hypergraph_repr_tag SourceReprTag>
    friend struct hgl::detail::to_impl;

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_list;
#endif

private:
    using vertex_major_list =
        incidence_list<DirectionalTag, repr::list_t<repr::vertex_major_t, id_type>>;
    using hyperedge_major_list =
        incidence_list<DirectionalTag, repr::list_t<repr::hyperedge_major_t, id_type>>;

    vertex_major_list _v_list;
    hyperedge_major_list _e_list;
};

} // namespace impl
} // namespace hgl
