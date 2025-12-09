// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/directional_tags.hpp"
#include "hgl/impl/layout_tags.hpp"
#include "hgl/types/types.hpp"

#include <algorithm>
#include <ranges>
#include <vector>
#include <cstdint>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_incidence_matrix;
} // namespace hgl_testing
#endif

namespace hgl::impl {

template <
    type_traits::c_hypergraph_directional_tag DirectionalTag,
    type_traits::c_hypergraph_layout_tag LayoutTag>
class incidence_matrix;

template <type_traits::c_hypergraph_layout_tag LayoutTag>
class incidence_matrix<hgl::undirected_t, LayoutTag> final {
public:
    using layout_tag = LayoutTag;

    incidence_matrix(const incidence_matrix&) = delete;
    incidence_matrix& operator=(const incidence_matrix&) = delete;

    incidence_matrix() = default;

    incidence_matrix(const types::size_type n_vertices, const types::size_type n_hyperedges)
    : _matrix_row_size{layout_tag::minor(n_vertices, n_hyperedges)},
      _matrix(
          layout_tag::major(n_vertices, n_hyperedges), matrix_row_type(_matrix_row_size, false)
      ) {}

    incidence_matrix(incidence_matrix&&) = default;
    incidence_matrix& operator=(incidence_matrix&&) = default;

    ~incidence_matrix() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            this->_add_major(n);
        else
            this->_add_minor(n);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            this->_remove_major(vertex_id);
        else
            this->_remove_minor(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            return this->_incident_with_major(vertex_id);
        else
            return this->_incident_with_minor(vertex_id);
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            return this->_count_major(vertex_id);
        else
            return this->_count_minor(vertex_id);
    }

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            this->_add_major(n);
        else
            this->_add_minor(n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            this->_remove_major(hyperedge_id);
        else
            this->_remove_minor(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            return this->_incident_with_major(hyperedge_id);
        else
            return this->_incident_with_minor(hyperedge_id);
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            return this->_count_major(hyperedge_id);
        else
            return this->_count_minor(hyperedge_id);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = true;
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = false;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[major_id][minor_id];
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_matrix;
#endif

private:
    using matrix_row_type = std::vector<bool>;
    using hypergraph_storage_type = std::vector<matrix_row_type>;

    gl_attr_force_inline void _add_major(const types::size_type n) noexcept {
        this->_matrix.resize(
            this->_matrix.size() + n, matrix_row_type(this->_matrix_row_size, false)
        );
    }

    void _add_minor(const types::size_type n) noexcept {
        this->_matrix_row_size += n;
        for (auto& row : this->_matrix)
            row.resize(this->_matrix_row_size, false);
    }

    gl_attr_force_inline void _remove_major(const types::id_type major_id) noexcept {
        this->_matrix.erase(this->_matrix.begin() + major_id);
    }

    gl_attr_force_inline void _remove_minor(const types::id_type minor_id) noexcept {
        if (this->_matrix_row_size == 0)
            return;
        this->_matrix_row_size--;
        for (auto& row : this->_matrix) {
            row.erase(row.begin() + minor_id);
        }
    }

    [[nodiscard]] auto _incident_with_major(const types::id_type major_id) const noexcept {
        return std::views::iota(0uz, this->_matrix_row_size)
             | std::views::filter([&row = this->_matrix[major_id]](const types::id_type minor_id) {
                   return row[minor_id];
               });
    }

    [[nodiscard]] auto _incident_with_minor(const types::id_type minor_id) const noexcept {
        return std::views::iota(0uz, this->_matrix.size())
             | std::views::filter([this, minor_id](const types::id_type major_id) {
                   return this->_matrix[major_id][minor_id];
               });
    }

    [[nodiscard]] types::size_type _count_major(const types::id_type major_id) const noexcept {
        types::size_type count = 0;
        for (const bool bit : this->_matrix[major_id])
            count += static_cast<types::size_type>(bit);
        return count;
    }

    [[nodiscard]] types::size_type _count_minor(const types::id_type minor_id) const noexcept {
        types::size_type count = 0;
        for (const auto& row : this->_matrix)
            count += static_cast<types::size_type>(row[minor_id]);
        return count;
    }

    types::size_type _matrix_row_size = 0uz;
    hypergraph_storage_type _matrix;
};

template <type_traits::c_hypergraph_layout_tag LayoutTag>
class incidence_matrix<hgl::bf_directed_t, LayoutTag> final {
public:
    using layout_tag = LayoutTag;

    incidence_matrix(const incidence_matrix&) = delete;
    incidence_matrix& operator=(const incidence_matrix&) = delete;

    incidence_matrix() = default;

    incidence_matrix(const types::size_type n_vertices, const types::size_type n_hyperedges)
    : _matrix_row_size{layout_tag::minor(n_vertices, n_hyperedges)},
      _matrix(
          layout_tag::major(n_vertices, n_hyperedges), matrix_row_type(_matrix_row_size, incidence_type::none)
      ) {}

    incidence_matrix(incidence_matrix&&) = default;
    incidence_matrix& operator=(incidence_matrix&&) = default;

    ~incidence_matrix() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            this->_add_major(n);
        else
            this->_add_minor(n);
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            this->_remove_major(vertex_id);
        else
            this->_remove_minor(vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            return this->_incident_with_major(vertex_id);
        else
            return this->_incident_with_minor(vertex_id);
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            return this->_count_major(vertex_id);
        else
            return this->_count_minor(vertex_id);
    }

    // TODO: tail/head-specific variants for incident_hyperedges and degree

    // --- hyperedge methods ---

    gl_attr_force_inline void add_hyperedges(const types::size_type n) noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            this->_add_major(n);
        else
            this->_add_minor(n);
    }

    gl_attr_force_inline void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            this->_remove_major(hyperedge_id);
        else
            this->_remove_minor(hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            return this->_incident_with_major(hyperedge_id);
        else
            return this->_incident_with_minor(hyperedge_id);
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            return this->_count_major(hyperedge_id);
        else
            return this->_count_minor(hyperedge_id);
    }

    // TODO: tail/head-specific variants for incident_vertices and hyperedge_size (tail_size, head_size)

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = true;
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_matrix[major_id][minor_id] = incidence_type::none;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_matrix[major_id][minor_id] != incidence_type::none;
    }

    // TODO: tail/head-specific variants for bind/are_bound

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_matrix;
#endif

private:
    enum class incidence_type : std::int8_t {
        none = 0,      // v not in E
        backward = -1, // v in T(E)
        forward = 1,   // v in H(E)
    };

    template <typename P>
    concept c_incidence_pred = std::predicate<P, incidence_type>;

    static constexpr auto _are_incident_pred = [](const incidence_type t) { return t != incidence_type::none; };

    using matrix_row_type = std::vector<incidence_type>;
    using hypergraph_storage_type = std::vector<matrix_row_type>;

    gl_attr_force_inline void _add_major(const types::size_type n) noexcept {
        this->_matrix.resize(
            this->_matrix.size() + n, matrix_row_type(this->_matrix_row_size, incidence_type::none)
        );
    }

    void _add_minor(const types::size_type n) noexcept {
        this->_matrix_row_size += n;
        for (auto& row : this->_matrix)
            row.resize(this->_matrix_row_size, incidence_type::none);
    }

    gl_attr_force_inline void _remove_major(const types::id_type major_id) noexcept {
        this->_matrix.erase(this->_matrix.begin() + major_id);
    }

    gl_attr_force_inline void _remove_minor(const types::id_type minor_id) noexcept {
        if (this->_matrix_row_size == 0)
            return;
        this->_matrix_row_size--;
        for (auto& row : this->_matrix)
            row.erase(row.begin() + minor_id);
    }

    [[nodiscard]] auto _incident_with_major(const types::id_type major_id) const noexcept {
        return std::views::iota(0uz, this->_matrix_row_size)
             | std::views::filter([&row = this->_matrix[major_id]](const types::id_type minor_id) {
                   return incidence_matrix::_are_incident_pred(row[minor_id]);
               });
    }

    [[nodiscard]] auto _incident_with_minor(const types::id_type minor_id) const noexcept {
        return std::views::iota(0uz, this->_matrix.size())
             | std::views::filter([this, minor_id](const types::id_type major_id) {
                   return incidence_matrix::_are_incident_pred(this->_matrix[major_id][minor_id]);
               });
    }

    [[nodiscard]] types::size_type _count_major(
        const types::id_type major_id,
        const c_incidence_pred auto pred = incidence_matrix::_are_incident_pred
    ) const noexcept {
        types::size_type count = 0;
        for (const incidence_type t : this->_matrix[major_id])
            count += static_cast<types::size_type>(pred(t));
        return count;
    }

    [[nodiscard]] types::size_type _count_minor(
        const types::id_type minor_id,
        const c_incidence_pred auto pred = incidence_matrix::_are_incident_pred
    ) const noexcept {
        types::size_type count = 0;
        for (const auto& row : this->_matrix)
            count += static_cast<types::size_type>(pred(row[minor_id]));
        return count;
    }

    types::size_type _matrix_row_size = 0uz;
    hypergraph_storage_type _matrix;
};

} // namespace hgl::impl
