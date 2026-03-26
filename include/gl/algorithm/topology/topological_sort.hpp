// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/templates/bfs.hpp"

namespace gl::algorithm {

template <
    traits::c_directed_graph GraphType,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
[[nodiscard]] std::optional<std::vector<id_type>> topological_sort(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using edge_type = typename GraphType::edge_type;

    // prepare the vertex in degree map
    std::vector<size_type> in_degree_map = graph.in_degree_map();

    // prepare the initial queue content (source vertices)
    std::vector<algorithm::vertex_info> source_vertex_list;
    source_vertex_list.reserve(graph.order());
    for (const auto id : graph.vertex_ids())
        if (in_degree_map[id] == 0uz)
            source_vertex_list.emplace_back(id);

    std::optional<std::vector<id_type>> topological_order_opt = std::vector<id_type>{};
    auto& topological_order = topological_order_opt.value();
    topological_order.reserve(graph.order());

    bfs(
        graph,
        source_vertex_list,
        algorithm::empty_callback{}, // visit predicate
        [&topological_order](
            const id_type vertex_id, [[maybe_unused]] const id_type source_id
        ) { // visit callback
            topological_order.push_back(vertex_id);
            return true;
        },
        [&in_degree_map](const id_type vertex_id, const edge_type& in_edge)
            -> predicate_result { // enqueue predicate
            if (in_edge.is_loop())
                return false;
            return --in_degree_map[vertex_id] == 0uz;
        },
        pre_visit,
        post_visit
    );

    if (topological_order.size() != graph.order())
        return std::nullopt;

    return topological_order_opt;
}

} // namespace gl::algorithm
