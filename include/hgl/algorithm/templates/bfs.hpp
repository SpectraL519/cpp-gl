// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/algorithm/core.hpp"

#include <queue>

namespace hgl::algorithm {

template <
    hgl::traits::c_hypergraph H,
    traits::c_forward_range_of<search_node<H>> InitQueueRangeType = std::vector<search_node<H>>,
    traits::c_optional_predicate<const search_node<H>&> VisitVertexPredicate = empty_callback,
    traits::c_optional_predicate<const search_node<H>&> VisitCallback = empty_callback,
    traits::c_optional_decision_predicate<typename H::id_type, typename H::id_type>
        TraverseHyperedgePred = empty_callback,
    traits::c_decision_predicate<const search_node<H>&> EnqueueVertexPred = empty_callback,
    traits::c_optional_callback<void, const search_node<H>&> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, const search_node<H>&> PostVisitCallback = empty_callback>
bool bfs(
    const H& hypergraph,
    const InitQueueRangeType& initial_queue_content,
    const VisitVertexPredicate& visit_vertex_pred = {},
    const VisitCallback& visit = {},
    const TraverseHyperedgePred& traverse_he_pred = {},
    const EnqueueVertexPred& enqueue_vertex_pred = {},
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using policy = traversal_policy<H>;

    if (std::ranges::empty(initial_queue_content))
        return false;

    std::queue<search_node<H>> q;
    for (const auto& node : initial_queue_content)
        q.push(node);

    while (not q.empty()) {
        const search_node curr_node = q.front();
        q.pop();

        if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(curr_node))
                continue;

        if constexpr (not traits::c_empty_callback<PreVisitCallback>)
            pre_visit(curr_node);

        if constexpr (not traits::c_empty_callback<VisitCallback>)
            if (not visit(curr_node))
                return false;

        for (const auto he_id : policy::out_hyperedges(hypergraph, curr_node.vertex_id)) {
            if constexpr (not traits::c_empty_callback<TraverseHyperedgePred>) {
                const auto traverse = traverse_he_pred(he_id, curr_node.vertex_id);
                if (traverse == decision::abort)
                    return false;
                if (traverse == decision::reject)
                    continue;
            }

            for (const auto target_id : policy::target_vertices(hypergraph, he_id)) {
                if (target_id == curr_node.vertex_id)
                    continue; // Skip the source vertex

                search_node<H> tgt_node{target_id, curr_node.vertex_id, he_id};
                const auto enqueue = enqueue_vertex_pred(tgt_node);
                if (enqueue == decision::abort)
                    return false;
                if (enqueue)
                    q.push(tgt_node);
            }
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(curr_node);
    }

    return true;
}

} // namespace hgl::algorithm
