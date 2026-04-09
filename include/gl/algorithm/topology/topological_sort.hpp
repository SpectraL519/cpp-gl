// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/bfs.hpp"

namespace gl::algorithm {

template <
    traits::c_directed_graph GraphType,
    traits::c_optional_callback<void, typename GraphType::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename GraphType::id_type> PostVisitCallback =
        empty_callback>
[[nodiscard]] std::optional<std::vector<typename GraphType::id_type>> topological_sort(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using id_type = typename GraphType::id_type;
    using edge_type = typename GraphType::edge_type;

    // prepare the vertex in degree map
    std::vector<size_type> in_degree_map = graph.in_degree_map();

    // prepare the initial queue content (source vertices)
    std::vector<vertex_info<GraphType>> source_vertex_list;
    source_vertex_list.reserve(graph.order());
    for (const auto id : graph.vertex_ids())
        if (in_degree_map[to_idx(id)] == 0uz)
            source_vertex_list.emplace_back(id);

    std::vector<id_type> topological_order{};
    topological_order.reserve(graph.order());

    bfs(
        graph,
        source_vertex_list,
        empty_callback{}, // visit predicate
        [&topological_order](
            const id_type vertex_id, [[maybe_unused]] const id_type source_id
        ) { // visit callback
            topological_order.push_back(vertex_id);
            return true;
        },
        [&in_degree_map](const id_type vertex_id, const edge_type& in_edge)
            -> decision { // enqueue predicate
            if (in_edge.is_loop())
                return false;
            return --in_degree_map[to_idx(vertex_id)] == 0uz;
        },
        pre_visit,
        post_visit
    );

    if (topological_order.size() != graph.order())
        return std::nullopt;

    return topological_order;
}

} // namespace gl::algorithm
