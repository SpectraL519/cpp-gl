#pragma once

#include "gl/impl/impl_tags_decl.hpp"

#include <algorithm>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_matrix;

namespace specialized {

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_directed_v<typename AdjacencyMatrix::edge_type>)
struct directed_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using vertex_ptr_type = typename impl_type::vertex_ptr_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    static void remove_vertex(impl_type& self, const vertex_ptr_type& vertex) {
        const auto vertex_id = vertex->id();

        self._n_unique_edges -= self.adjacent_edges(vertex_id).distance();
        self._matrix.erase(std::next(std::begin(self._matrix), vertex->id()));

        for (auto& row : self._matrix) {
            const auto vertex_it = std::next(std::begin(row), vertex_id);
            self._n_unique_edges -= static_cast<types::size_type>(*vertex_it != nullptr);
            row.erase(vertex_it);
        }
    }

    static const edge_ptr_type& add_edge(impl_type& self, edge_ptr_type edge) {
        self._n_unique_edges++;
        auto& matrix_element = self._matrix.at(edge->first()->id()).at(edge->second()->id());
        matrix_element = std::move(edge);
        return matrix_element;
    }

    static inline void remove_edge(impl_type& self, const edge_ptr_type& edge) {
        self._matrix.at(edge->first()->id()).at(edge->second()->id()) = nullptr;
        self._n_unique_edges--;
    }
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct undirected_adjacency_matrix {
    using impl_type = AdjacencyMatrix;
    using vertex_ptr_type = typename impl_type::vertex_ptr_type;
    using edge_ptr_type = typename impl_type::edge_ptr_type;

    static void remove_vertex(impl_type& self, const vertex_ptr_type& vertex) {
        const auto vertex_id = vertex->id();

        self._n_unique_edges -= self.adjacent_edges(vertex_id).distance();
        self._matrix.erase(std::next(std::begin(self._matrix), vertex->id()));

        for (auto& row : self._matrix)
            row.erase(std::next(std::begin(row), vertex_id));
    }

    static const edge_ptr_type& add_edge(impl_type& self, edge_ptr_type edge) {
        const auto first_id = edge->first()->id();
        const auto second_id = edge->second()->id();

        self._n_unique_edges++;
        if (not edge->is_loop())
            self._matrix.at(second_id).at(first_id) = edge;
        auto& matrix_element = self._matrix.at(first_id).at(second_id);
        matrix_element = edge;
        return matrix_element;
    }

    static void remove_edge(impl_type& self, const edge_ptr_type& edge) {
        const auto first_id = edge->first()->id();
        const auto second_id = edge->second()->id();
        const bool is_loop = edge->is_loop();

        self._matrix.at(first_id).at(second_id) = nullptr;
        if (not is_loop)
            self._matrix.at(second_id).at(first_id) = nullptr;
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
