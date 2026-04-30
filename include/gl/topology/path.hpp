// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/topology/path.hpp
/// @brief Generators for path (linear) graph topologies.

#pragma once

#include "gl/constants.hpp"
#include "gl/conversion.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

/// @ingroup GL-Topology
/// @brief Generates a regular path graph (linear topology).
///
/// A path graph consists of a simple linear sequence of vertices. Vertex \f$v_i\f$ is connected
/// to vertex \f$v_{i+1}\f$, terminating at the final vertex.
///
/// For directed graphs, this creates a one-way chain from the first to the last vertex.
///
/// > [!NOTE] Performance for Flat List Graphs
/// >
/// > If the requested `GraphType` satisfies [**c_flat_list_graph**](gl_concepts.md#gl-traits-c-flat-list-graph),
/// > an optimized overload is automatically selected. It internally constructs a standard adjacency list graph
/// > first, and then utilizes the @ref gl::to "to" conversion to flatten it. This is significantly faster than
/// > inserting edges one-by-one into a flat representation.
///
/// @tparam GraphType The target graph type to generate.
/// @param n_vertices The total number of vertices in the path.
/// @return A newly constructed graph representing the linear path.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType path(size_type n_vertices) {
    using id_type = typename GraphType::id_type;

    GraphType graph{n_vertices};

    for (id_type source_id = initial_id; source_id < n_vertices - 1uz; ++source_id)
        graph.add_edge(source_id, source_id + static_cast<id_type>(1));

    return graph;
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType path(size_type n_vertices) {
    using base_graph_type = traits::swap_impl_tag_t<GraphType, impl::list_t>;
    return to<impl::flat_list_t>(path<base_graph_type>(n_vertices));
}

/// @ingroup GL-Topology
/// @brief Generates a path graph with bidirectional edges.
///
/// For directed graphs, this function creates reciprocal edges along the linear sequence,
/// allowing traversal in both directions.
///
/// For undirected graphs, this function simply falls back to @ref gl::topology::path "path",
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
/// @param n_vertices The total number of vertices in the path.
/// @return A newly constructed graph representing the bidirectional path.
template <traits::c_graph GraphType>
[[nodiscard]] GraphType bidirectional_path(size_type n_vertices) {
    if constexpr (traits::c_directed_graph<GraphType>) {
        using id_type = typename GraphType::id_type;

        GraphType graph{n_vertices};

        for (id_type source_id = initial_id; source_id < n_vertices - 1uz; ++source_id) {
            const auto target_id = source_id + static_cast<id_type>(1);
            graph.add_edge(source_id, target_id);
            graph.add_edge(target_id, source_id);
        }

        return graph;
    }
    else {
        return path<GraphType>(n_vertices);
    }
}

template <traits::c_flat_list_graph GraphType>
[[nodiscard]] GraphType bidirectional_path(size_type n_vertices) {
    using base_graph_type = traits::swap_impl_tag_t<GraphType, impl::list_t>;
    return to<impl::flat_list_t>(bidirectional_path<base_graph_type>(n_vertices));
}

} // namespace gl::topology
