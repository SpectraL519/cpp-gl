// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/types/types.hpp"
#include "layout_tags.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_incidence_matrix;
} // namespace hgl_testing
#endif

namespace hgl::impl {

template <type_traits::c_hypergraph_layout_tag LayoutTag>
class undirected_incidence_matrix final {
public:
    using layout_tag = LayoutTag;

    // Rows are "Major", Columns are "Minor"
    using matrix_row_type = std::vector<bool>;
    using hypergraph_storage_type = std::vector<matrix_row_type>;

    undirected_incidence_matrix(const undirected_incidence_matrix&) = delete;
    undirected_incidence_matrix& operator=(const undirected_incidence_matrix&) = delete;

    undirected_incidence_matrix() = default;

    undirected_incidence_matrix(
        const types::size_type n_vertices, const types::size_type n_hyperedges
    )
    : _minor_size{layout_tag::minor(n_vertices, n_hyperedges)},
      _major_storage(
          layout_tag::major(n_vertices, n_hyperedges), matrix_row_type(_minor_size, false)
      ) {}

    undirected_incidence_matrix(undirected_incidence_matrix&&) = default;
    undirected_incidence_matrix& operator=(undirected_incidence_matrix&&) = default;

    ~undirected_incidence_matrix() = default;

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
            return this->_incident_to_major(vertex_id);
        else
            return this->_incident_to_minor(vertex_id);
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::vertex_major_t>)
            return this->_count_major_bits(vertex_id);
        else
            return this->_count_minor_bits(vertex_id);
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
            return this->_incident_to_major(hyperedge_id);
        else
            return this->_incident_to_minor(hyperedge_id);
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        if constexpr (std::same_as<layout_tag, impl::hyperedge_major_t>)
            return this->_count_major_bits(hyperedge_id);
        else
            return this->_count_minor_bits(hyperedge_id);
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_major_storage[major_id][minor_id] = true;
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        this->_major_storage[major_id][minor_id] = false;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        const auto [major_id, minor_id] = layout_tag::majmin(vertex_id, hyperedge_id);
        return this->_major_storage[major_id][minor_id];
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_matrix;
#endif

private:
    types::size_type _minor_size = 0uz; // matrix row size
    hypergraph_storage_type _major_storage;

    gl_attr_force_inline void _add_major(const types::size_type n) noexcept {
        this->_major_storage.resize(
            this->_major_storage.size() + n, matrix_row_type(this->_minor_size, false)
        );
    }

    void _add_minor(const types::size_type n) noexcept {
        this->_minor_size += n;
        for (auto& row : this->_major_storage)
            row.resize(this->_minor_size, false);
    }

    gl_attr_force_inline void _remove_major(const types::id_type major_id) noexcept {
        this->_major_storage.erase(this->_major_storage.begin() + major_id);
    }

    gl_attr_force_inline void _remove_minor(const types::id_type minor_id) noexcept {
        if (this->_minor_size == 0)
            return;
        this->_minor_size--;
        for (auto& row : this->_major_storage) {
            row.erase(row.begin() + minor_id);
        }
    }

    [[nodiscard]] auto _incident_to_major(const types::id_type major_id) const noexcept {
        return std::views::iota(0uz, this->_minor_size)
             | std::views::filter(
                   [&row = this->_major_storage[major_id]](const types::id_type minor_id) {
                       return row[minor_id];
                   }
             );
    }

    [[nodiscard]] auto _incident_to_minor(const types::id_type minor_id) const noexcept {
        return std::views::iota(0uz, this->_major_storage.size())
             | std::views::filter([this, minor_id](const types::id_type major_id) {
                   return this->_major_storage[major_id][minor_id];
               });
    }

    [[nodiscard]] types::size_type _count_major_bits(const types::id_type major_id) const noexcept {
        types::size_type count = 0;
        for (const bool bit : this->_major_storage[major_id])
            count += static_cast<types::size_type>(bit);
        return count;
    }

    [[nodiscard]] types::size_type _count_minor_bits(const types::id_type minor_id) const noexcept {
        types::size_type count = 0;
        for (const auto& row : this->_major_storage)
            count += static_cast<types::size_type>(row[minor_id]);
        return count;
    }
};

} // namespace hgl::impl
