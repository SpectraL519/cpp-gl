// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "impl/common.hpp"

#include <iostream>
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

    // TODO: replace with edge->incident_vertex_id(id) after it's been added to the edge_descriptor class
    const auto get_other_vertex_id = [](const edge_type& edge, const types::id_type source_id) {
        return edge.first_id() == source_id ? edge.second_id() : edge.first_id();
    };

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

        const auto& target_id = get_other_vertex_id(min_edge, min_edge_info.source_id);
        if (visited[target_id])
            continue;

        // add the minimum weight edge to the mst
        mst.edges.emplace_back(min_edge);
        mst.weight += min_weight;

        visited[target_id] = true;
        ++n_vertices_in_mst;

        // enqueue all edges adjacent to the `target` vertex if they lead to unvisited verties
        for (const auto& edge : graph.adjacent_edges(target_id))
            if (not visited[get_other_vertex_id(edge, target_id)])
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
[[nodiscard]] mst_descriptor<GraphType> prim_mst(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // type definitions
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using edge_info_type = algorithm::edge_info<edge_type>;
    using distance_type = types::vertex_distance_type<GraphType>;

    struct prim_vertex_info {
        distance_type min_cost = std::numeric_limits<distance_type>::max(
        ); // the cost of the cheapest connection to the vertex
        const edge_type* min_cost_edge =
            nullptr; // the edge corresponding to the cheapest connection
        bool in_mst = false;
    };

    // prepare the necessary utility
    const auto n_vertices = graph.n_vertices();
    std::vector<prim_vertex_info> vinfo_list(n_vertices);
    mst_descriptor<GraphType> mst(n_vertices);

    // TODO: replace with edge->incident_vertex_id(id) after it's been added to the edge_descriptor class
    const auto get_other_vertex_id = [](const edge_type& edge, const types::id_type source_id) {
        return edge.first_id() == source_id ? edge.second_id() : edge.first_id();
    };

    const auto find_min_cost_vertex = [&vinfo_list]() {
        distance_type min_cost = std::numeric_limits<distance_type>::max();
        types::id_type min_cost_vertex_id = 0;

        types::size_type i = 0;
        for (const auto& vinfo : vinfo_list) {
            if (not vinfo.in_mst and vinfo.min_cost <= min_cost) {
                min_cost = vinfo.min_cost;
                min_cost_vertex_id = i;
            }
            ++i;
        }

        return std::make_pair(min_cost_vertex_id, min_cost);
    };

    for (types::size_type n = constants::zero; n < n_vertices; ++n) {
        // find a vertex with the cheapest connection which is not yet in the MST
        const auto [min_cost_vertex_id, min_cost] = find_min_cost_vertex();
        auto& min_cost_vertex = vinfo_list[min_cost_vertex_id];

        min_cost_vertex.in_mst = true; // add the vertex to the MST

        const auto* min_cost_edge = min_cost_vertex.min_cost_edge;
        if (min_cost_edge != nullptr) { // add the edge to MST
            mst.edges.emplace_back(*min_cost_edge);
            mst.weight += get_weight<GraphType>(*min_cost_edge);
        }

        for (const auto& edge : graph.adjacent_edges(min_cost_vertex_id)) {
            const auto edge_weight = get_weight<GraphType>(edge);
            auto& incident_vinfo = vinfo_list[get_other_vertex_id(edge, min_cost_vertex_id)];
            if (edge_weight < incident_vinfo.min_cost) {
                incident_vinfo.min_cost = edge_weight;
                incident_vinfo.min_cost_edge = &edge;
            }
        }
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
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // type definitions
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using distance_type = types::vertex_distance_type<GraphType>;

    struct prim_vertex_info {
        prim_vertex_info(const types::id_type id)
        : id(id),
          min_cost(std::numeric_limits<distance_type>::max()),
          min_cost_edge(nullptr),
          in_mst(false) {}

        types::id_type id; // ID of the vertex
        distance_type min_cost; // cheapest connection
        const edge_type* min_cost_edge; // edge for the cheapest connection
        bool in_mst = false;
    };

    // Comparator for the binary heap
    auto heap_comparator = [](const prim_vertex_info* a, const prim_vertex_info* b) {
        return a->min_cost > b->min_cost; // Min-heap based on min_cost
    };

    // Prepare the necessary utility
    const auto n_vertices = graph.n_vertices();
    std::vector<prim_vertex_info> vinfo_list;
    std::vector<prim_vertex_info*> heap; // min-heap storing pointers to vinfo_list elements
    mst_descriptor<GraphType> mst(n_vertices);

    // Initialize the vertex info and the heap
    vinfo_list.reserve(n_vertices);
    for (types::id_type id = constants::initial_id; id < n_vertices; ++id) {
        auto& vinfo = vinfo_list.emplace_back(id);
        heap.push_back(&vinfo);
    }
    std::make_heap(heap.begin(), heap.end(), heap_comparator);

    // Helper to find the other vertex of an edge
    const auto get_other_vertex_id = [](const edge_type& edge, const types::id_type source_id) {
        return edge.first_id() == source_id ? edge.second_id() : edge.first_id();
    };

    while (not heap.empty()) {
        // Extract the vertex with the smallest cost
        std::pop_heap(heap.begin(), heap.end(), heap_comparator);
        auto* min_vertex_info = heap.back();
        heap.pop_back();

        if (min_vertex_info->in_mst)
            continue; // Skip if already in MST
        min_vertex_info->in_mst = true; // Add vertex to MST

        const auto vertex_id = min_vertex_info->id;
        const auto* min_cost_edge = min_vertex_info->min_cost_edge;

        if (min_cost_edge != nullptr) { // Add the corresponding edge to MST
            mst.edges.emplace_back(*min_cost_edge);
            mst.weight += get_weight<GraphType>(*min_cost_edge);
        }

        // Update adjacent vertices
        for (const auto& edge : graph.adjacent_edges(vertex_id)) {
            const auto edge_weight = get_weight<GraphType>(edge);
            auto& incident_vinfo = vinfo_list[get_other_vertex_id(edge, vertex_id)];

            if (not incident_vinfo.in_mst && edge_weight < incident_vinfo.min_cost) {
                incident_vinfo.min_cost = edge_weight;
                incident_vinfo.min_cost_edge = &edge;
            }
        }

        // Rebuild the heap for the updated vertices
        std::make_heap(heap.begin(), heap.end(), heap_comparator);
    }

    return mst;
}

} // namespace gl::algorithm
