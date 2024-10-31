// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "impl/common.hpp"

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
        types::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        types::empty_callback>
[[nodiscard]] mst_descriptor<GraphType> prim_mst(
    const GraphType& graph,
    const std::optional<types::id_type> root_id_opt,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // type definitions

    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using edge_info_type = types::edge_info<edge_type>;
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

        if (min_weight < constants::zero)
            throw std::invalid_argument(std::format(
                "[alg::prim_mst] Found an edge with a negative weight: [{}, {} | w={}]",
                min_edge.first_id(),
                min_edge.second_id(),
                min_weight
            ));

        const auto& target_id = get_other_vertex_id(min_edge, min_edge_info.source_id);
        if (not visited[target_id]) {
            // add the minimum weight edge to the mst
            mst.edges.emplace_back(min_edge);
            mst.weight += min_weight;

            visited[target_id] = true;
            ++n_vertices_in_mst;
        }

        // enqueue all edges adjacent to the `target` vertex if they lead to unvisited verties
        for (const auto& edge : graph.adjacent_edges(target_id))
            if (not visited[get_other_vertex_id(edge, target_id)])
                edge_queue.emplace(edge, target_id);
    }

    return mst;
}

} // namespace gl::algorithm
