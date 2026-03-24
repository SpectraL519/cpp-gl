// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/util.hpp"
#include "gl/constants.hpp"

#include <numeric>
#include <queue>

namespace gl::algorithm {

template <traits::c_undirected_graph GraphType>
struct mst_descriptor {
    using graph_type = GraphType;
    using edge_type = typename graph_type::edge_type;
    using weight_type = types::vertex_distance_type<graph_type>;

    mst_descriptor(const size_type n_vertices) {
        edges.reserve(n_vertices - 1uz);
    }

    std::vector<edge_type> edges;
    weight_type weight = static_cast<weight_type>(0);
};

template <traits::c_undirected_graph GraphType>
[[nodiscard]] mst_descriptor<GraphType> edge_heap_prim_mst(
    const GraphType& graph, const std::optional<id_type> root_id_opt
) {
    // type definitions
    using edge_type = typename GraphType::edge_type;

    struct edge_comparator {
        [[nodiscard]] gl_attr_force_inline bool operator()(
            const edge_type& lhs, const edge_type& rhs
        ) const {
            return get_weight<GraphType>(lhs) > get_weight<GraphType>(rhs);
        }
    };

    using queue_type = std::priority_queue<edge_type, std::vector<edge_type>, edge_comparator>;

    // prepare the necessary utility
    const auto n_vertices = graph.order();
    mst_descriptor<GraphType> mst(n_vertices);
    std::vector<bool> visited(n_vertices, false);
    queue_type edge_queue;

    // insert the edges adjacent to the root vertex to the queue
    const id_type root_id = root_id_opt.value_or(constants::initial_id);

    for (const auto& edge : graph.adjacent_edges(root_id))
        edge_queue.emplace(edge);

    // mark the root vertex as visited
    visited[root_id] = true;
    size_type n_vertices_in_mst = 1uz;

    // find the mst
    while (n_vertices_in_mst < n_vertices) {
        const auto min_edge = edge_queue.top();
        edge_queue.pop();

        if (visited[min_edge.target()])
            continue;

        // add the minimum weight edge to the mst
        mst.edges.emplace_back(min_edge);
        mst.weight += get_weight<GraphType>(min_edge);

        visited[min_edge.target()] = true;
        ++n_vertices_in_mst;

        // enqueue all edges adjacent to the `target` vertex if they lead to unvisited verties
        for (const auto& edge : graph.adjacent_edges(min_edge.target()))
            if (not visited[edge.incident_vertex(min_edge.target())])
                edge_queue.emplace(edge);
    }

    return mst;
}

template <traits::c_undirected_graph GraphType>
requires traits::c_has_numeric_limits_max<types::vertex_distance_type<GraphType>>
[[nodiscard]] mst_descriptor<GraphType> vertex_heap_prim_mst(
    const GraphType& graph, const std::optional<id_type> root_id_opt
) {
    // type definitions
    using edge_type = typename GraphType::edge_type;
    using distance_type = types::vertex_distance_type<GraphType>;

    // Prepare the necessary utility
    const auto n_vertices = graph.order();
    mst_descriptor<GraphType> mst(n_vertices);

    std::vector<bool> in_mst(n_vertices, false);
    std::vector<distance_type> min_cost(n_vertices, std::numeric_limits<distance_type>::max());
    std::vector<std::optional<edge_type>> min_cost_edges(n_vertices, std::nullopt);

    // set the distance to the root vertex to 0
    min_cost.at(root_id_opt.value_or(constants::initial_id)) = static_cast<distance_type>(0);

    auto heap_comparator = [&min_cost](const id_type lhs, const id_type rhs) {
        return min_cost[lhs] > min_cost[rhs]; // min-heap based on min_cost
    };

    // Initialize the vertex info and the heap
    std::vector<id_type> heap(n_vertices);
    std::iota(heap.begin(), heap.end(), constants::initial_id);
    std::make_heap(heap.begin(), heap.end(), heap_comparator);

    while (not heap.empty()) {
        // Extract the vertex with the smallest cost
        std::pop_heap(heap.begin(), heap.end(), heap_comparator);
        const auto vertex_id = heap.back();
        heap.pop_back();

        if (in_mst[vertex_id])
            continue;

        in_mst[vertex_id] = true;

        const auto min_cost_edge = min_cost_edges[vertex_id];
        if (min_cost_edge.has_value()) { // Add the corresponding edge to MST
            mst.edges.emplace_back(*min_cost_edge);
            mst.weight += min_cost[vertex_id];
        }

        // Update adjacent vertices
        for (const auto& edge : graph.adjacent_edges(vertex_id)) {
            const auto edge_weight = get_weight<GraphType>(edge);
            const auto incident_vertex_id = edge.incident_vertex(vertex_id);

            if (not in_mst[incident_vertex_id] and edge_weight < min_cost[incident_vertex_id]) {
                min_cost[incident_vertex_id] = edge_weight;
                min_cost_edges[incident_vertex_id].emplace(edge);
            }
        }

        // Rebuild the heap for the updated vertices
        std::make_heap(heap.begin(), heap.end(), heap_comparator);
    }

    return mst;
}

} // namespace gl::algorithm
