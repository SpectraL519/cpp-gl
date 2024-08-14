#pragma once

#include "gl/graph_traits.hpp"
#include "gl/types/type_traits.hpp"

#include <algorithm>

namespace gl::impl {

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class adjacency_matrix;

namespace specialized {

namespace detail {

constexpr auto make_null_edge = []() { return nullptr; };

} // namespace detail

// directed adjacency_matrix

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_directed_v<typename AdjacencyMatrix::edge_type>)
struct directed_adjacency_list {
    using impl_type = AdjacencyMatrix;

    static void construct(impl_type& self, const types::size_type no_vertices) {
        // initialize a full n x n matrix
        for (auto& row : self._matrix) {
            row.reserve(no_vertices);
            std::generate_n(std::back_inserter(row), no_vertices, detail::make_null_edge);
        }
    }
};

template <type_traits::c_instantiation_of<adjacency_matrix> AdjacencyMatrix>
requires(type_traits::is_undirected_v<typename AdjacencyMatrix::edge_type>)
struct undirected_adjacency_list {
    using impl_type = AdjacencyMatrix;

    static void construct(impl_type& self, const types::size_type no_vertices) {
        // initialize a (lower) triangular n x n matrix
        for (types::size_type row_idx = 0ull; row_idx < no_vertices; row_idx++) {
            auto& row = self._matrix[row_idx];
            const auto row_size = row_idx + 1ull;

            row.reserve(row_size);
            std::generate_n(std::back_inserter(row), row_size, detail::make_null_edge);
        }
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
