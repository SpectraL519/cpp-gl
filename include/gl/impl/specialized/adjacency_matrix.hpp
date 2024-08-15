#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/type_traits.hpp"

#include <algorithm>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_matrix;

namespace specialized {

// directed adjacency_matrix

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_directed_v<typename AdjacencyMatrix::edge_type>)
struct directed_adjacency_matrix {
    using impl_type = AdjacencyMatrix;

    static void add_edge(impl_type& self, typename impl_type::edge_ptr_type edge) {
        self._matrix.at(edge->first()->id()).at(edge->second()->id()) = std::move(edge);
        self._no_unique_edges++;
    }

    static void remove_edge(impl_type& self, const typename impl_type::edge_ptr_type& edge) {
        self._matrix.at(edge->first()->id()).at(edge->second()->id()) = nullptr;
        self._no_unique_edges--;
    }
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct undirected_adjacency_matrix {
    using impl_type = AdjacencyMatrix;

    static void add_edge(impl_type& self, typename impl_type::edge_ptr_type edge) {
        const auto first_id = edge->first()->id();
        const auto second_id = edge->second()->id();

        self._matrix.at(first_id).at(second_id) = edge;
        if (not edge->is_loop())
            self._matrix.at(second_id).at(first_id) = edge;
        self._no_unique_edges++;
    }

    static void remove_edge(impl_type& self, const typename impl_type::edge_ptr_type& edge) {
        const auto first_id = edge->first()->id();
        const auto second_id = edge->second()->id();
        const bool is_loop = edge->is_loop();

        self._matrix.at(first_id).at(second_id) = nullptr;
        if (not is_loop)
            self._matrix.at(second_id).at(first_id) = nullptr;
        self._no_unique_edges--;
    }
};

// common utility

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
struct matrix_impl_traits {
    using specialized_type = void;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_directed_v<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using specialized_type = directed_adjacency_matrix<AdjacencyMatrix>;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using specialized_type = undirected_adjacency_matrix<AdjacencyMatrix>;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
using impl_type = typename matrix_impl_traits<AdjacencyMatrix>::specialized_type;

} // namespace specialized

} // namespace gl::impl
