// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"

#include <algorithm>

namespace gl::impl {

template <type_traits::c_matrix_graph_traits GraphTraits>
class adjacency_matrix;

namespace specialized {

namespace detail {

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
[[nodiscard]] typename AdjacencyMatrix::edge_ptr_type& strict_get(
    typename AdjacencyMatrix::matrix_type& matrix, const typename AdjacencyMatrix::edge_type* edge
) {
    auto& matrix_element = matrix.at(edge->first_id()).at(edge->second_id());
    if (edge != matrix_element.get())
        throw std::invalid_argument(std::format(
            "Got invalid edge [vertices = ({}, {}) | addr = {}]",
            edge->first_id(),
            edge->second_id(),
            io::format(edge)
        ));

    return matrix_element;
}

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
inline void check_edge_override(
    const AdjacencyMatrix& adj_matrix, const typename AdjacencyMatrix::edge_ptr_type& edge
) {
    const auto [first_id, second_id] = edge->incident_vertex_ids();

    if (adj_matrix.has_edge(first_id, second_id))
        throw std::logic_error(std::format(
            "Cannot override an existing edge without remove: ({}, {})", first_id, second_id
        ));
}

} // namespace detail

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_directed_v<typename AdjacencyMatrix::edge_type>)
struct directed_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    static void remove_vertex(impl_type& self, const vertex_type& vertex) {
        const auto vertex_id = vertex.id();

        self._n_unique_edges -= self.adjacent_edges(vertex_id).distance();
        self._matrix.erase(std::next(std::begin(self._matrix), vertex.id()));

        for (auto& row : self._matrix) {
            const auto vertex_it = std::next(std::begin(row), vertex_id);
            self._n_unique_edges -= static_cast<types::size_type>(*vertex_it != nullptr);
            row.erase(vertex_it);
        }
    }

    static const edge_type& add_edge(impl_type& self, edge_ptr_type edge) {
        detail::check_edge_override<impl_type>(self, edge);

        auto& matrix_element = self._matrix[edge->first_id()][edge->second_id()];
        matrix_element = std::move(edge);
        self._n_unique_edges++;

        return *matrix_element;
    }

    static void add_edges_from(
        impl_type& self, const types::id_type source_id, std::vector<edge_ptr_type> new_edges
    ) {
        // validate new edges
        for (auto& edge : new_edges)
            detail::check_edge_override(self, edge);

        auto& matrix_row_source = self._matrix[source_id];
        for (auto& edge : new_edges)
            matrix_row_source[edge->second_id()] = std::move(edge);

        self._n_unique_edges += new_edges.size();
    }

    static inline void remove_edge(impl_type& self, const edge_type& edge) {
        detail::strict_get<impl_type>(self._matrix, &edge) = nullptr;
        self._n_unique_edges--;
    }
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct undirected_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    static void remove_vertex(impl_type& self, const vertex_type& vertex) {
        const auto vertex_id = vertex.id();

        self._n_unique_edges -= self.adjacent_edges(vertex_id).distance();
        self._matrix.erase(std::next(std::begin(self._matrix), vertex.id()));

        for (auto& row : self._matrix)
            row.erase(std::next(std::begin(row), vertex_id));
    }

    static const edge_type& add_edge(impl_type& self, edge_ptr_type edge) {
        detail::check_edge_override<impl_type>(self, edge);

        const auto first_id = edge->first_id();
        const auto second_id = edge->second_id();

        if (not edge->is_loop())
            self._matrix[second_id][first_id] = edge;
        auto& matrix_element = self._matrix.at(first_id).at(second_id);
        matrix_element = edge;

        self._n_unique_edges++;
        return *matrix_element;
    }

    static void add_edges_from(
        impl_type& self, const types::id_type source_id, std::vector<edge_ptr_type> new_edges
    ) {
        // validate new edges
        for (auto& edge : new_edges)
            detail::check_edge_override(self, edge);

        auto& matrix_row_source = self._matrix[source_id];
        for (auto& edge : new_edges) {
            if (not edge->is_loop())
                self._matrix[edge->second_id()][source_id] = edge;
            matrix_row_source[edge->second_id()] = std::move(edge);
        }

        self._n_unique_edges += new_edges.size();
    }

    static void remove_edge(impl_type& self, const edge_type& edge) {
        if (edge.is_loop()) {
            detail::strict_get<impl_type>(self._matrix, &edge) = nullptr;
        }
        else {
            const auto first_id = edge.first_id();
            const auto second_id = edge.second_id();

            detail::strict_get<impl_type>(self._matrix, &edge) = nullptr;
            // if the edge was found in the first matrix cell, it will be in the second matrix cell
            self._matrix[second_id][first_id] = nullptr;
        }
        self._n_unique_edges--;
    }
};

// common utility

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
struct matrix_impl_traits {
    using type = void;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_directed_v<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using type = directed_adjacency_matrix<AdjacencyMatrix>;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using type = undirected_adjacency_matrix<AdjacencyMatrix>;
};

} // namespace specialized

} // namespace gl::impl
