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
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            this->_remove_major(vertex_id);
        else
            this->_remove_minor(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(
        const types::id_type vertex_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            return this->_incident_with_major(vertex_id);
        else
            return this->_incident_with_minor(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(
        const types::id_type vertex_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            return this->_major_size(vertex_id);
        else
            return this->_minor_size(vertex_id);
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            this->_major_storage.resize(this->_major_storage.size() + n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            this->_remove_major(hyperedge_id);
        else
            this->_remove_minor(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(
        const types::id_type hyperedge_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            return this->_incident_with_major(hyperedge_id);
        else
            return this->_incident_with_minor(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type hyperedge_size(
        const types::id_type hyperedge_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            return this->_major_size(hyperedge_id);
        else
            return this->_minor_size(hyperedge_id);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        this->_bind_impl(
            layout_tag::major(vertex_id, hyperedge_id), layout_tag::minor(vertex_id, hyperedge_id)
        );
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        this->_unbind_impl(
            layout_tag::major(vertex_id, hyperedge_id), layout_tag::minor(vertex_id, hyperedge_id)
        );
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        return this->_are_bound_impl(
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

    gl_attr_force_inline void _remove_major(const types::id_type major_id) noexcept {
        this->_major_storage.erase(
            this->_major_storage.begin() + static_cast<std::ptrdiff_t>(major_id)
        );
    }

    void _remove_minor(const types::id_type minor_id) noexcept {
        for (auto& minor_storage : this->_major_storage) {
            auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
            if (minor_it != minor_storage.end() and *minor_it == minor_id)
                minor_it = minor_storage.erase(minor_it); // unbind the element
            while (minor_it != minor_storage.end())
                --(*minor_it++); // decrement ids > minor_id
        }
    }

    [[nodiscard]] gl_attr_force_inline auto _incident_with_major(
        const types::id_type major_id
    ) const noexcept {
        return std::views::all(this->_major_storage[major_id]);
    }

    [[nodiscard]] gl_attr_force_inline auto _incident_with_minor(
        const types::id_type minor_id
    ) const noexcept {
        return std::views::iota(0uz, this->_major_storage.size())
             | std::views::filter([this, minor_id](types::id_type major_id) {
                   return this->_are_bound_impl(this->_major_storage[major_id], minor_id);
               });
    }

    [[nodiscard]] gl_attr_force_inline types::size_type _major_size(
        const types::id_type major_id
    ) const noexcept {
        return this->_major_storage[major_id].size();
    }

    [[nodiscard]] types::size_type _minor_size(const types::id_type minor_id) const noexcept {
        types::size_type size = 0uz;
        for (const auto& major_el : this->_major_storage)
            if (this->_are_bound_impl(major_el, minor_id))
                ++size;
        return size;
    }

    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    void _bind_impl(const types::id_type major_id, const types::id_type minor_id) noexcept {
        auto& minor_storage = this->_major_storage[major_id];

        // insert the id at the correct position to keep the minor-id collection sorted
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        if (minor_it == minor_storage.end() or *minor_it != minor_id)
            minor_storage.insert(minor_it, minor_id);
    }

    gl_attr_force_inline void _unbind_impl(
        // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        const types::id_type major_id,
        const types::id_type minor_id
    ) noexcept {
        auto& minor_storage = this->_major_storage[major_id];
        const auto minor_it = std::ranges::lower_bound(minor_storage, minor_id);
        if (minor_it != minor_storage.end() and *minor_it == minor_id)
            minor_storage.erase(minor_it);
    }

    [[nodiscard]] bool _are_bound_impl(
        const major_element_type& major_el, const types::id_type minor_id
    ) const noexcept {
        const auto minor_it = std::ranges::lower_bound(major_el, minor_id);
        return minor_it != major_el.end() and *minor_it == minor_id;
    }

    major_storage_type _major_storage;
};

} // namespace hgl::impl
