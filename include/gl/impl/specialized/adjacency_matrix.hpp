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
struct directed_adjacency_list {
    using impl_type = AdjacencyMatrix;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct undirected_adjacency_list {
    using impl_type = AdjacencyMatrix;
};

// common utility

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
struct matrix_impl_traits {
    using specialized_type = void;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_directed_v<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using specialized_type = directed_adjacency_list<AdjacencyMatrix>;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct matrix_impl_traits<AdjacencyMatrix> {
    using specialized_type = undirected_adjacency_list<AdjacencyMatrix>;
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
using impl_type = typename matrix_impl_traits<AdjacencyMatrix>::specialized_type;

} // namespace specialized

} // namespace gl::impl
