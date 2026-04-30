// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/topology/bipartite.hpp
/// @brief Generators for complete bipartite (biclique) graph topologies.

#pragma once

#include "gl/constants.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"
#include "gl/repr_tags.hpp"

namespace gl::topology {

/// @ingroup GL-Topology
/// @brief Generates a complete bipartite graph (biclique).
///
/// A complete bipartite graph is a graph whose vertices are partitioned into two disjoint
/// sets, \f$A\f$ and \f$B\f$, such that every vertex in \f$A\f$ connects to every vertex in \f$B\f$.
///
/// If the requested `GraphType` is directed, this function generates reciprocal edges
/// (from \f$A\f$ to \f$B\f$ and from \f$B\f$ to \f$A\f$) to mimic full undirected connectivity.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested `GraphType` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to "to" conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam GraphType The target graph type to generate.
/// @param n_vertices_a The number of vertices in partition \f$A\f$.
/// @param n_vertices_b The number of vertices in partition \f$B\f$.
/// @return A newly constructed graph representing the biclique.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType biclique(size_type n_vertices_a, size_type n_vertices_b) {
    using id_type = typename GraphType::id_type;

    const auto n_vertices = n_vertices_a + n_vertices_b;
    GraphType graph{n_vertices};

    for (id_type source_id = initial_id; source_id < n_vertices_a; ++source_id) {
        for (auto target_id = static_cast<id_type>(n_vertices_a); target_id < n_vertices;
             ++target_id) {
            graph.add_edge(source_id, target_id);
            if constexpr (traits::c_directed_graph<GraphType>)
                graph.add_edge(target_id, source_id);
        }
    }

    return graph;
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType biclique(size_type n_vertices_a, size_type n_vertices_b) {
    using base_graph_type = traits::swap_repr_tag_t<GraphType, repr::list_t>;
    return to<repr::flat_list_t>(biclique<base_graph_type>(n_vertices_a, n_vertices_b));
}

} // namespace gl::topology
