// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/types/dereferencing_iterator.hpp"
#include "gl/types/iterator_range.hpp"
#include "gl/types/non_null_iterator.hpp"
#include "gl/types/types.hpp"
#include "specialized/adjacency_matrix.hpp"

namespace gl::impl {

template <type_traits::c_matrix_graph_traits GraphTraits>
class adjacency_matrix final {
public:
    using vertex_type = typename GraphTraits::vertex_type;
    using edge_type = typename GraphTraits::edge_type;
    using edge_id_list_type = std::vector<types::id_type>;
    using matrix_type = std::vector<edge_id_list_type>;

    adjacency_matrix(const adjacency_matrix&) = delete;
    adjacency_matrix& operator=(const adjacency_matrix&) = delete;

    adjacency_matrix() = default;

    adjacency_matrix(const types::size_type n_vertices) : _matrix(n_vertices) {
        // initialize a full n x n matrix with null elements
        for (auto& row : this->_matrix)
            row.resize(n_vertices, constants::invalid_id);
    }

    adjacency_matrix(adjacency_matrix&&) = default;
    adjacency_matrix& operator=(adjacency_matrix&&) = default;

    ~adjacency_matrix() = default;

    // --- vertex methods ---

    void add_vertex() {
        for (auto& row : this->_matrix)
            row.push_back(constants::invalid_id);
        this->_matrix.emplace_back(this->_matrix.size() + 1uz, constants::invalid_id);
    }

    void add_vertices(const types::size_type n) {
        const auto new_n_vertices = this->_matrix.size() + n;

        for (auto& row : this->_matrix)
            row.resize(new_n_vertices, constants::invalid_id);

        for (types::size_type _ = constants::begin_idx; _ < n; ++_)
            this->_matrix.emplace_back(new_n_vertices, constants::invalid_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type in_degree(const types::id_type vertex_id
    ) const {
        return specialized_impl::in_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type out_degree(const types::id_type vertex_id
    ) const {
        return specialized_impl::out_degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline types::size_type degree(const types::id_type vertex_id
    ) const {
        return specialized_impl::degree(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> in_degree_map() const {
        return specialized_impl::in_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> out_degree_map() const {
        return specialized_impl::out_degree_map(*this);
    }

    [[nodiscard]] gl_attr_force_inline std::vector<types::size_type> degree_map() const {
        return specialized_impl::degree_map(*this);
    }

    template <bool GetRemovedEdgeIds>
    gl_attr_force_inline auto remove_vertex(const types::id_type vertex_id) {
        return specialized_impl::template remove_vertex<GetRemovedEdgeIds>(*this, vertex_id);
    }

    // --- edge methods ---

    gl_attr_force_inline void add_edge(
        types::id_type id, types::id_type source_id, types::id_type target_id
    ) {
        specialized_impl::add_edge(*this, id, source_id, target_id);
    }

    gl_attr_force_inline void add_edges_from(
        const type_traits::c_sized_range_of<types::id_type> auto& edge_ids,
        const types::id_type source_id,
        const type_traits::c_sized_range_of<types::id_type> auto& target_ids
    ) {
        specialized_impl::add_edges_from(*this, edge_ids, source_id, target_ids);
    }

    [[nodiscard]] gl_attr_force_inline bool has_edge(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        return this->_matrix[source_id][target_id] != constants::invalid_id;
    }

    [[nodiscard]] bool has_edge(const edge_type& edge) const {
        return this->_matrix[edge.first()][edge.second()] == edge.id();
    }

    [[nodiscard]] std::optional<edge_type> get_edge(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id)
            return std::nullopt;
        return std::make_optional<edge_type>(edge_id, source_id, target_id);
    }

    [[nodiscard]] std::optional<edge_type> get_edge(
        const types::id_type source_id,
        const types::id_type target_id,
        const auto& edge_properties_map
    ) const {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id)
            return std::nullopt;
        return std::make_optional<edge_type>(
            edge_id, source_id, target_id, edge_properties_map[edge_id]
        );
    }

    [[nodiscard]] std::vector<edge_type> get_edges(
        const types::id_type source_id, const types::id_type target_id
    ) const {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id)
            return std::vector<edge_type>();
        return std::vector<edge_type>(edge_type{edge_id, source_id, target_id});
    }

    [[nodiscard]] std::vector<edge_type> get_edges(
        const types::id_type source_id,
        const types::id_type target_id,
        const auto& edge_properties_map
    ) const {
        const auto edge_id = this->_matrix[source_id][target_id];
        if (edge_id == constants::invalid_id)
            return std::vector<edge_type>();
        return std::vector<edge_type>(
            edge_type{edge_id, source_id, target_id, edge_properties_map[edge_id]}
        );
    }

    gl_attr_force_inline void remove_edge(const edge_type& edge) {
        specialized_impl::remove_edge(*this, edge);
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(const types::id_type vertex_id) const {
        return this->_matrix[vertex_id] | std::views::enumerate
             | std::views::filter([](const auto target_id, const auto edge_id) {
                   return edge_id != constants::invalid_id;
               })
             | std::views::transform([vertex_id](const auto target_id, const auto edge_id) {
                   return edge_type{edge_id, vertex_id, target_id};
               });
    }

    [[nodiscard]] gl_attr_force_inline auto adjacent_edges(
        const types::id_type vertex_id, const auto& edge_properties_map
    ) const {
        return this->_matrix[vertex_id] | std::views::enumerate
             | std::views::filter([](const auto target_id, const auto edge_id) {
                   return edge_id != constants::invalid_id;
               })
             | std::views::transform(
                   [vertex_id, &edge_properties_map](const auto target_id, const auto edge_id) {
                       return edge_type{
                           edge_id, vertex_id, target_id, edge_properties_map[edge_id]
                       };
                   }
             );
    }

private:
    using specialized_impl = typename specialized::matrix_impl_traits<adjacency_matrix>::type;
    friend specialized_impl;

    matrix_type _matrix{};
    types::size_type _n_unique_edges{constants::default_size};
};

} // namespace gl::impl
