// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/topology/cycle.hpp
/// @brief Generators for cycle (ring) graph topologies.

#pragma once

#include "gl/constants.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

/// @ingroup GL-Topology
/// @brief Generates a regular cycle graph (ring topology).
///
/// A cycle graph consists of a single closed chain of vertices. Vertex \f$v_i\f$ is connected
/// to vertex \f$v_{i+1}\f$, with the final vertex connecting back to vertex \f$v_0\f$.
///
/// For directed graphs, this creates a unidirectional loop.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested `GraphType` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to "to" conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam GraphType The target graph type to generate.
/// @param n_vertices The total number of vertices in the cycle.
/// @return A newly constructed graph representing the cycle.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType cycle(size_type n_vertices) {
    using id_type = typename GraphType::id_type;

    GraphType graph{n_vertices};

    for (id_type source_id = initial_id; source_id < n_vertices; ++source_id)
        graph.add_edge(source_id, static_cast<id_type>((source_id + 1uz) % n_vertices));

    return graph;
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType cycle(size_type n_vertices) {
    using base_graph_type = traits::swap_repr_tag_t<GraphType, repr::list_t>;
    return to<repr::flat_list_t>(cycle<base_graph_type>(n_vertices));
}

/// @ingroup GL-Topology
/// @brief Generates a cycle graph with bidirectional edges.
///
/// For directed graphs, this function creates a reciprocal edge for every forward edge
/// in the closed chain, forming a bidirectional ring.
///
/// For undirected graphs, this function simply falls back to @ref gl::topology::cycle "cycle",
/// as undirected edges are inherently bidirectional.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested `GraphType` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to "to" conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam GraphType The target graph type to generate.
/// @param n_vertices The total number of vertices in the cycle.
/// @return A newly constructed graph representing the bidirectional cycle.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType bidirectional_cycle(size_type n_vertices) {
    if constexpr (traits::c_directed_graph<GraphType>) {
        using id_type = typename GraphType::id_type;

        GraphType graph{n_vertices};

        for (id_type source_id = initial_id; source_id < n_vertices; ++source_id) {
            const auto target_id = static_cast<id_type>((source_id + 1uz) % n_vertices);
            graph.add_edge(source_id, target_id);
            graph.add_edge(target_id, source_id);
        }

        return graph;
    }
    else {
        return cycle<GraphType>(n_vertices);
    }
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType bidirectional_cycle(size_type n_vertices) {
    using base_graph_type = traits::swap_repr_tag_t<GraphType, repr::list_t>;
    return to<repr::flat_list_t>(bidirectional_cycle<base_graph_type>(n_vertices));
}

} // namespace gl::topology
