// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/types/types.hpp"

#include <algorithm>
#include <ranges>
#include <vector>

#ifdef HGL_TESTING
namespace hgl_testing {
struct test_incidence_matrix;
} // namespace hgl_testing
#endif

namespace hgl::impl {

class undirected_incidence_matrix final {
public:
    using matrix_row_type = std::vector<bool>;
    using hypergraph_storage_type = std::vector<matrix_row_type>;

    undirected_incidence_matrix(const undirected_incidence_matrix&) = delete;
    undirected_incidence_matrix& operator=(const undirected_incidence_matrix&) = delete;

    undirected_incidence_matrix() = default;

    undirected_incidence_matrix(
        const types::size_type n_vertices, const types::size_type n_hyperedges
    )
    : _matrix_row_size{n_hyperedges}, _matrix{n_vertices} {
        for (auto& row : this->_matrix)
            row.resize(this->_matrix_row_size, false);
    }

    undirected_incidence_matrix(undirected_incidence_matrix&&) = default;
    undirected_incidence_matrix& operator=(undirected_incidence_matrix&&) = default;

    ~undirected_incidence_matrix() = default;

    // --- vertex methods ---

    gl_attr_force_inline void add_vertices(const types::size_type n) noexcept {
        this->_matrix.resize(
            this->_matrix.size() + n, matrix_row_type(this->_matrix_row_size, false)
        );
    }

    gl_attr_force_inline void remove_vertex(const types::id_type vertex_id) noexcept {
        this->_matrix.erase(this->_matrix.begin() + vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_hyperedges(const types::id_type vertex_id
    ) const noexcept {
        return std::views::iota(0uz, this->_matrix_row_size)
             | std::views::filter(
                   [&vertex_row = this->_matrix[vertex_id]](const auto hyperedge_id) -> bool {
                       return vertex_row[hyperedge_id];
                   }
             );
    }

    [[nodiscard]] types::size_type degree(const types::id_type vertex_id) const noexcept {
        types::size_type deg = 0;
        for (const bool is_hyperedge_incident : this->_matrix[vertex_id])
            deg += static_cast<types::size_type>(is_hyperedge_incident);
        return deg;
    }

    // --- hyperedge methods ---

    void add_hyperedges(const types::size_type n) noexcept {
        this->_matrix_row_size += n;
        for (auto& row : this->_matrix)
            row.resize(this->_matrix_row_size, false);
    }

    void remove_hyperedge(const types::id_type hyperedge_id) noexcept {
        this->_matrix_row_size--;
        for (auto& row : this->_matrix)
            row.erase(row.begin() + hyperedge_id);
    }

    [[nodiscard]] gl_attr_force_inline auto incident_vertices(const types::id_type hyperedge_id
    ) const noexcept {
        return std::views::iota(0uz, this->_matrix.size())
             | std::views::filter([this, hyperedge_id](const auto vertex_id) {
                   return this->_matrix[vertex_id][hyperedge_id];
               });
    }

    [[nodiscard]] types::size_type hyperedge_size(const types::id_type hyperedge_id
    ) const noexcept {
        types::size_type size = 0;
        for (const auto& row : this->_matrix)
            size += static_cast<types::size_type>(row[hyperedge_id]);
        return size;
    }

    // --- binding methods ---

    gl_attr_force_inline void bind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        this->_matrix[vertex_id][hyperedge_id] = true;
    }

    gl_attr_force_inline void unbind(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) noexcept {
        this->_matrix[vertex_id][hyperedge_id] = false;
    }

    [[nodiscard]] gl_attr_force_inline bool are_bound(
        const types::id_type vertex_id, const types::id_type hyperedge_id
    ) const noexcept {
        return this->_matrix[vertex_id][hyperedge_id];
    }

#ifdef HGL_TESTING
    friend struct hgl_testing::test_incidence_matrix;
#endif

private:
    types::size_type _matrix_row_size = 0uz;
    hypergraph_storage_type _matrix;
};

} // namespace hgl::impl
