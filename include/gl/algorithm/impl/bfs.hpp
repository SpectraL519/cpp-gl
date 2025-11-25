// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "common.hpp"

#include <queue>

namespace gl::algorithm::impl {

template <
    type_traits::c_graph GraphType,
    type_traits::c_sized_range_of<algorithm::vertex_info> InitQueueRangeType =
        std::vector<algorithm::vertex_info>,
    type_traits::c_optional_id_callback<bool> VisitVertexPredicate,
    type_traits::c_optional_id_callback<bool, types::id_type> VisitCallback,
    type_traits::c_id_callback<predicate_result, const typename GraphType::edge_type&>
        EnqueueVertexPred,
    type_traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    type_traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
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
    using vertex_queue_type = std::queue<algorithm::vertex_info>;
    vertex_queue_type vertex_queue;

    for (const auto& vinfo : initial_queue_content)
        vertex_queue.push(vinfo);

    // search the graph
    while (not vertex_queue.empty()) {
        const algorithm::vertex_info vinfo = vertex_queue.front();
        vertex_queue.pop();

        if constexpr (not type_traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(vinfo.id))
                continue;

        if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
            pre_visit(vinfo.id);

        if constexpr (not type_traits::c_empty_callback<VisitCallback>)
            if (not visit(vinfo.id, vinfo.pred_id))
                return false;

        for (const auto& edge : graph.adjacent_edges(vinfo.id)) {
            const auto incident_vertex_id = edge.incident_vertex(vinfo.id);

            const auto enqueue = enqueue_vertex_pred(incident_vertex_id, edge);
            if (enqueue == predicate_result::unknown)
                return false;

            if (enqueue)
                vertex_queue.emplace(incident_vertex_id, vinfo.id);
        }

        if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
            post_visit(vinfo.id);
    }

    return true;
}

} // namespace gl::algorithm::impl
