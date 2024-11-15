// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "common.hpp"

#include <queue>

namespace gl::algorithm::impl {

template <
    type_traits::c_graph GraphType,
    std::predicate<types::vertex_info, types::vertex_info> PQCompare,
    type_traits::c_sized_range_of<types::vertex_info> InitQueueRangeType =
        std::vector<types::vertex_info>,
    type_traits::c_optional_vertex_callback<GraphType, bool> VisitVertexPredicate,
    type_traits::c_optional_callback<GraphType, bool, types::id_type> VisitCallback,
    type_traits::
        c_vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>
            EnqueueVertexPred,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        types::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        types::empty_callback>
bool pfs(
    const GraphType& graph,
    const PQCompare& pq_compare,
    const InitQueueRangeType& initial_queue_content,
    const VisitVertexPredicate& visit_vertex_pred,
    const VisitCallback& visit,
    const EnqueueVertexPred& enqueue_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (initial_queue_content.size() == constants::default_size)
        return false;

    // prepare the vertex queue
    using vertex_queue_type =
        std::priority_queue<types::vertex_info, std::vector<types::vertex_info>, PQCompare>;
    vertex_queue_type vertex_queue(pq_compare);

    // TODO [C++23]: replace with push_range
    for (const auto& vinfo : initial_queue_content)
        vertex_queue.push(vinfo);

    // search the graph
    while (not vertex_queue.empty()) {
        const auto vinfo = vertex_queue.top();
        vertex_queue.pop();

        const auto& vertex = graph.get_vertex(vinfo.id);
        if constexpr (not type_traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(vertex))
                continue;

        if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
            pre_visit(vertex);

        if constexpr (not type_traits::c_empty_callback<VisitCallback>)
            if (not visit(vertex, vinfo.source_id))
                return false;

        for (const auto& edge : graph.adjacent_edges(vinfo.id)) {
            const auto& incident_vertex = edge.incident_vertex(vertex);

            const auto enqueue = enqueue_vertex_pred(incident_vertex, edge);
            if (not enqueue.has_value())
                return false;

            if (enqueue.value())
                vertex_queue.emplace(incident_vertex.id(), vinfo.id);
        }
        if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
            post_visit(vertex);
    }

    return true;
}

} // namespace gl::algorithm::impl
