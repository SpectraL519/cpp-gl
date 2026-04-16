// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/bfs.hpp"

namespace gl::algorithm {

template <
    traits::c_directed_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
[[nodiscard]] std::optional<std::vector<typename G::id_type>> topological_sort(
    const G& graph, PreVisitCallback pre_visit = {}, PostVisitCallback post_visit = {}
) {
    using id_type = typename G::id_type;
    using edge_type = typename G::edge_type;

    // prepare the vertex in degree map
    std::vector<size_type> in_degree_map = graph.in_degree_map();

    // prepare the initial queue content (source vertices)
    std::vector<search_node<G>> source_vertex_list;
    source_vertex_list.reserve(graph.n_vertices());
    for (const auto id : graph.vertex_ids())
        if (in_degree_map[to_idx(id)] == 0uz)
            source_vertex_list.emplace_back(id);

    std::vector<id_type> topological_order{};
    topological_order.reserve(graph.n_vertices());

    bfs(
        graph,
        source_vertex_list,
        empty_callback{}, // visit predicate
        [&topological_order](id_type vertex_id, id_type) { // visit callback
            topological_order.push_back(vertex_id);
            return true;
        },
        [&in_degree_map](id_type vertex_id, const edge_type& in_edge)
            -> decision { // enqueue predicate
            if (in_edge.is_loop())
                return false;
            return --in_degree_map[to_idx(vertex_id)] == 0uz;
        },
        pre_visit,
        post_visit
    );

    if (topological_order.size() != graph.n_vertices())
        return std::nullopt;

    return topological_order;
}

} // namespace gl::algorithm
