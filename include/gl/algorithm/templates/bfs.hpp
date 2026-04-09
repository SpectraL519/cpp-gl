// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"

#include <queue>

namespace gl::algorithm {

template <
    traits::c_graph GraphType,
    traits::c_forward_range_of<vertex_info<GraphType>> InitQueueRangeType =
        std::vector<vertex_info<GraphType>>,
    traits::c_optional_predicate<typename GraphType::id_type> VisitVertexPredicate,
    traits::c_optional_predicate<typename GraphType::id_type, typename GraphType::id_type>
        VisitCallback,
    traits::c_decision_predicate<typename GraphType::id_type, const typename GraphType::edge_type&>
        EnqueueVertexPred,
    traits::c_optional_callback<void, typename GraphType::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename GraphType::id_type> PostVisitCallback =
        empty_callback>
bool bfs(
    const GraphType& graph,
    const InitQueueRangeType& initial_queue_content,
    const VisitVertexPredicate& visit_vertex_pred = {},
    const VisitCallback& visit = {},
    const EnqueueVertexPred& enqueue_vertex_pred = {},
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (std::ranges::empty(initial_queue_content))
        return false;

    // prepare the vertex queue
    using vertex_queue_type = std::queue<vertex_info<GraphType>>;
    vertex_queue_type vertex_queue;

    for (const auto& vinfo : initial_queue_content)
        vertex_queue.push(vinfo);

    // search the graph
    while (not vertex_queue.empty()) {
        const vertex_info vinfo = vertex_queue.front();
        vertex_queue.pop();

        if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(vinfo.id))
                continue;

        if constexpr (not traits::c_empty_callback<PreVisitCallback>)
            pre_visit(vinfo.id);

        if constexpr (not traits::c_empty_callback<VisitCallback>)
            if (not visit(vinfo.id, vinfo.pred_id))
                return false;

        for (const auto& edge : graph.adjacent_edges(vinfo.id)) {
            const auto incident_vertex_id = edge.incident_vertex(vinfo.id);

            const auto enqueue = enqueue_vertex_pred(incident_vertex_id, edge);
            if (enqueue == decision::abort)
                return false;

            if (enqueue)
                vertex_queue.emplace(incident_vertex_id, vinfo.id);
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(vinfo.id);
    }

    return true;
}

} // namespace gl::algorithm
