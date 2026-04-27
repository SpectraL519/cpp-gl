// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/topology/clique.hpp
/// @brief Generators for complete (clique) graph topologies.

#pragma once

#include "gl/constants.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

/// @ingroup GL GL-Topology
/// @brief Generates a complete graph (clique).
///
/// A complete graph is a simple graph in which every pair of distinct vertices is connected by an edge.
///
/// If the requested `GraphType` is directed, this function generates fully bidirectional
/// edges between every pair of vertices.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested `GraphType` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to "to" conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam GraphType The target graph type to generate.
/// @param n_vertices The total number of vertices in the clique.
/// @return A newly constructed graph representing the clique.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType clique(size_type n_vertices) {
    using id_type = typename GraphType::id_type;

    GraphType graph{n_vertices};

    for (id_type source_id = initial_id; source_id < n_vertices; ++source_id) {
        for (id_type target_id = initial_id; target_id < source_id; ++target_id) {
            graph.add_edge(source_id, target_id);
            if constexpr (traits::c_directed_graph<GraphType>)
                graph.add_edge(target_id, source_id);
        }
    }

    return graph;
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType clique(size_type n_vertices) {
    using base_graph_type = traits::swap_impl_tag_t<GraphType, impl::list_t>;
    return to<impl::flat_list_t>(clique<base_graph_type>(n_vertices));
}

} // namespace gl::topology
