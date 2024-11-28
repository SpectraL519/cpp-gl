// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"

#include <algorithm>
#include <vector>

namespace gl::impl {

template <type_traits::c_matrix_graph_traits GraphTraits>
class adjacency_matrix;

namespace specialized {

namespace detail {

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
[[nodiscard]] typename AdjacencyMatrix::edge_ptr_type& strict_get(
    typename AdjacencyMatrix::matrix_type& matrix, const typename AdjacencyMatrix::edge_type* edge
) {
    // get the edge and validate the address
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

    [[nodiscard]] gl_attr_force_inline static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return std::ranges::count_if(
            self._matrix,
            [](const auto& edge) { return edge != nullptr; },
            [vertex_id](const auto& row) -> const edge_ptr_type& { return row[vertex_id]; }
        );
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type out_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return std::ranges::count_if(self._matrix[vertex_id], [](const auto& edge) {
            return edge != nullptr;
        });
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return in_degree(self, vertex_id) + out_degree(self, vertex_id);
    }

    [[nodiscard]] static std::vector<types::size_type> in_degree_map(const impl_type& self) {
        std::vector<types::id_type> in_degree_map(self._matrix.size(), constants::zero);

        for (const auto& row : self._matrix)
            for (types::id_type id = constants::initial_id; id < self._matrix.size(); ++id)
                if (row[id] != nullptr)
                    ++in_degree_map[id];

        return in_degree_map;
    }

    [[nodiscard]] static std::vector<types::size_type> out_degree_map(const impl_type& self) {
        std::vector<types::id_type> out_degree_map;
        out_degree_map.reserve(self._matrix.size());

        for (types::id_type id = constants::initial_id; id < self._matrix.size(); ++id)
            out_degree_map.push_back(out_degree(self, id));

        return out_degree_map;
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::id_type> degree_map(self._matrix.size(), constants::zero);

        for (types::id_type u_id = constants::initial_id; u_id < self._matrix.size(); ++u_id) {
            const auto& row = self._matrix[u_id];
            for (types::id_type v_id = constants::initial_id; v_id < self._matrix.size(); ++v_id) {
                if (row[v_id] != nullptr) {
                    ++degree_map[u_id];
                    ++degree_map[v_id];
                }
            }
        }

        return degree_map;
    }

    static void remove_vertex(impl_type& self, const types::id_type vertex_id) {
        self._n_unique_edges -= self.adjacent_edges(vertex_id).distance();
        self._matrix.erase(std::next(std::begin(self._matrix), vertex_id));

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
        ++self._n_unique_edges;

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
        --self._n_unique_edges;
    }
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct undirected_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using vertex_type = typename impl_type::vertex_type;
    using edge_type = typename impl_type::edge_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    [[nodiscard]] gl_attr_force_inline static types::size_type in_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type out_degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        return degree(self, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline static types::size_type degree(
        const impl_type& self, const types::id_type vertex_id
    ) {
        types::size_type degree = constants::default_size;
        for (const auto& edge : self._matrix[vertex_id])
            if (edge)
                degree += constants::one + static_cast<types::size_type>(edge->is_loop());
        return degree;
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> in_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    [[nodiscard]] gl_attr_force_inline static std::vector<types::size_type> out_degree_map(
        const impl_type& self
    ) {
        return degree_map(self);
    }

    [[nodiscard]] static std::vector<types::size_type> degree_map(const impl_type& self) {
        std::vector<types::id_type> degree_map(self._matrix.size(), constants::zero);

        for (types::id_type u_id = constants::initial_id; u_id < self._matrix.size(); ++u_id) {
            const auto& row = self._matrix[u_id];
            for (types::id_type v_id = constants::initial_id; v_id <= u_id; ++v_id) {
                if (row[v_id] != nullptr) {
                    ++degree_map[u_id];
                    ++degree_map[v_id];
                }
            }
        }

        return degree_map;
    }

    static void remove_vertex(impl_type& self, const types::id_type vertex_id) {
        self._n_unique_edges -= self.adjacent_edges(vertex_id).distance();
        self._matrix.erase(std::next(std::begin(self._matrix), vertex_id));

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

        ++self._n_unique_edges;
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
            // if the edge was found in the first matrix cell,
            // it will also be present in the second matrix cell
            self._matrix[second_id][first_id] = nullptr;
        }
        --self._n_unique_edges;
    }
};

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
