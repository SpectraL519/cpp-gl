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
    traits::c_graph G,
    traits::c_predicate<search_node<G>, search_node<G>> PQCompare,
    traits::c_forward_range_of<search_node<G>> InitQueueRangeType = std::vector<search_node<G>>,
    traits::c_optional_predicate<typename G::id_type> VisitVertexPredicate = empty_callback,
    traits::c_optional_predicate<typename G::id_type, typename G::id_type> VisitCallback =
        empty_callback,
    traits::c_decision_predicate<typename G::id_type, const typename G::edge_type&>
        EnqueueVertexPred = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
bool pfs(
    const G& graph,
    const PQCompare& pq_compare,
    const InitQueueRangeType& initial_queue_content,
    VisitVertexPredicate visit_vertex_pred = {},
    VisitCallback visit = {},
    EnqueueVertexPred enqueue_vertex_pred = {},
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    if (std::ranges::empty(initial_queue_content))
        return false;

    // prepare the vertex queue
    using queue_type = std::priority_queue<search_node<G>, std::vector<search_node<G>>, PQCompare>;
    queue_type q(pq_compare);

    for (const auto& node : initial_queue_content)
        q.push(node);

    // search the graph
    while (not q.empty()) {
        const auto node = q.top();
        q.pop();

        if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(node.vertex_id))
                continue;

        if constexpr (not traits::c_empty_callback<PreVisitCallback>)
            pre_visit(node.vertex_id);

        if constexpr (not traits::c_empty_callback<VisitCallback>)
            if (not visit(node.vertex_id, node.pred_id))
                return false;

        for (const auto& edge : graph.adjacent_edges(node.vertex_id)) {
            const auto incident_vertex_id = edge.incident_vertex(node.vertex_id);

            const auto enqueue = enqueue_vertex_pred(incident_vertex_id, edge);
            if (enqueue == decision::abort)
                return false;

            if (enqueue)
                q.emplace(incident_vertex_id, node.vertex_id);
        }
        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(node.vertex_id);
    }

    return true;
}

} // namespace gl::algorithm
