// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "impl/common.hpp"

#include <numeric>
#include <queue>

namespace gl::algorithm {

template <type_traits::c_undirected_graph GraphType>
struct mst_descriptor {
    using graph_type = GraphType;
    using edge_type = typename graph_type::edge_type;
    using weight_type = types::vertex_distance_type<graph_type>;

    mst_descriptor(const types::size_type n_vertices) {
        edges.reserve(n_vertices - constants::one);
    }

    std::vector<types::const_ref_wrap<edge_type>> edges;
    weight_type weight = static_cast<weight_type>(constants::zero);
};

template <
    type_traits::c_undirected_graph GraphType,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        algorithm::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        algorithm::empty_callback>
[[nodiscard]] mst_descriptor<GraphType> mst(
    const GraphType& graph,
    const std::optional<types::id_type> root_id_opt,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // type definitions

    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using edge_info_type = algorithm::edge_info<edge_type>;
    using distance_type = types::vertex_distance_type<GraphType>;

    struct edge_info_comparator {
        [[nodiscard]] gl_attr_force_inline bool operator()(
            const edge_info_type& lhs, const edge_info_type& rhs
        ) const {
            return get_weight<GraphType>(lhs.edge.get()) > get_weight<GraphType>(rhs.edge.get());
        }
    };

    using queue_type =
        std::priority_queue<edge_info_type, std::vector<edge_info_type>, edge_info_comparator>;

    // prepare the necessary utility
    const auto n_vertices = graph.n_vertices();
    mst_descriptor<GraphType> mst(n_vertices);
    std::vector<bool> visited(n_vertices, false);
    queue_type edge_queue;

    // insert the edges adjacent to the root vertex to the queue
    const types::id_type root_id = root_id_opt.value_or(constants::zero);

    for (const auto& edge : graph.adjacent_edges(root_id))
        edge_queue.emplace(edge, root_id);

    // mark the root vertex as visited
    visited[root_id] = true;
    types::size_type n_vertices_in_mst = constants::one;

    // find the mst
    while (n_vertices_in_mst < n_vertices) {
        const auto min_edge_info = edge_queue.top();
        edge_queue.pop();

        const auto& min_edge = min_edge_info.edge.get();
        const auto min_weight = get_weight<GraphType>(min_edge);

        const auto& target_id = min_edge.incident_vertex_id(min_edge_info.source_id);
        if (visited[target_id])
            continue;

        // add the minimum weight edge to the mst
        mst.edges.emplace_back(min_edge);
        mst.weight += min_weight;

        visited[target_id] = true;
        ++n_vertices_in_mst;

        // enqueue all edges adjacent to the `target` vertex if they lead to unvisited verties
        for (const auto& edge : graph.adjacent_edges(target_id))
            if (not visited[edge.incident_vertex_id(target_id)])
                edge_queue.emplace(edge, target_id);
    }

    return mst;
}

template <
    type_traits::c_undirected_graph GraphType,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        algorithm::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        algorithm::empty_callback>
[[nodiscard]] mst_descriptor<GraphType> bin_heap_prim_mst(
    const GraphType& graph,
    const std::optional<types::id_type> root_id_opt,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // type definitions
    using edge_type = typename GraphType::edge_type;
    using distance_type = types::vertex_distance_type<GraphType>;

    // Prepare the necessary utility
    const auto n_vertices = graph.n_vertices();
    mst_descriptor<GraphType> mst(n_vertices);

    std::vector<bool> in_mst(n_vertices, false);
    std::vector<distance_type> min_cost(n_vertices, std::numeric_limits<distance_type>::max());
    std::vector<const edge_type*> min_cost_edges(n_vertices, nullptr);

    // set the distance to the root vertex to 0
    min_cost.at(root_id_opt.value_or(constants::zero)) = constants::zero;

    auto heap_comparator = [&min_cost](const types::id_type lhs, const types::id_type rhs) {
        return min_cost[lhs] > min_cost[rhs]; // min-heap based on min_cost
    };

    // Initialize the vertex info and the heap
    std::vector<types::id_type> heap(n_vertices);
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

        const auto* min_cost_edge = min_cost_edges[vertex_id];
        if (min_cost_edge != nullptr) { // Add the corresponding edge to MST
            mst.edges.emplace_back(*min_cost_edge);
            mst.weight += min_cost[vertex_id];
        }

        // Update adjacent vertices
        for (const auto& edge : graph.adjacent_edges(vertex_id)) {
            const auto edge_weight = get_weight<GraphType>(edge);
            const auto incident_vertex_id = edge.incident_vertex_id(vertex_id);

            if (not in_mst[incident_vertex_id] && edge_weight < min_cost[incident_vertex_id]) {
                min_cost[incident_vertex_id] = edge_weight;
                min_cost_edges[incident_vertex_id] = &edge;
            }
        }

        // Rebuild the heap for the updated vertices
        std::make_heap(heap.begin(), heap.end(), heap_comparator);
    }

    return mst;
}

} // namespace gl::algorithm
