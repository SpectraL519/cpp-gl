// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/algorithm/core.hpp"

#include <queue>

namespace hgl::algorithm {

template <
    hgl::traits::c_hypergraph HypergraphType,
    gl::traits::c_forward_range_of<traversal_context<HypergraphType>> InitQueueRangeType =
        std::vector<traversal_context<HypergraphType>>,
    gl::traits::c_optional_predicate<const traversal_context<HypergraphType>&>
        VisitVertexPredicate = gl::algorithm::empty_callback,
    gl::traits::c_optional_predicate<const traversal_context<HypergraphType>&> VisitCallback =
        gl::algorithm::empty_callback,
    gl::traits::c_optional_decision_predicate<
        typename HypergraphType::id_type,
        typename HypergraphType::id_type> TraverseHyperedgePred = gl::algorithm::empty_callback,
    gl::traits::c_decision_predicate<const traversal_context<HypergraphType>&> EnqueueVertexPred =
        gl::algorithm::empty_callback,
    gl::traits::c_optional_callback<void, const traversal_context<HypergraphType>&>
        PreVisitCallback = gl::algorithm::empty_callback,
    gl::traits::c_optional_callback<void, const traversal_context<HypergraphType>&>
        PostVisitCallback = gl::algorithm::empty_callback>
bool bfs(
    const HypergraphType& hypergraph,
    const InitQueueRangeType& initial_queue_content,
    const VisitVertexPredicate& visit_vertex_pred = {},
    const VisitCallback& visit = {},
    const TraverseHyperedgePred& traverse_he_pred = {},
    const EnqueueVertexPred& enqueue_vertex_pred = {},
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using router = traversal_traits<HypergraphType>;
    using ctx_queue_type = std::queue<traversal_context<HypergraphType>>;

    if (std::ranges::empty(initial_queue_content))
        return false;

    ctx_queue_type ctx_queue;

    for (const auto& ctx : initial_queue_content)
        ctx_queue.push(ctx);

    while (not ctx_queue.empty()) {
        const traversal_context current_ctx = ctx_queue.front();
        ctx_queue.pop();

        if constexpr (not gl::traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(current_ctx))
                continue;

        if constexpr (not gl::traits::c_empty_callback<PreVisitCallback>)
            pre_visit(current_ctx);

        if constexpr (not gl::traits::c_empty_callback<VisitCallback>)
            if (not visit(current_ctx))
                return false;

        for (const auto he_id : router::out_hyperedges(hypergraph, current_ctx.id)) {
            if constexpr (not gl::traits::c_empty_callback<TraverseHyperedgePred>) {
                const auto traverse = traverse_he_pred(he_id, current_ctx.id);
                if (traverse == gl::algorithm::decision::abort)
                    return false;
                if (traverse == gl::algorithm::decision::reject)
                    continue;
            }

            for (const auto target_id : router::target_vertices(hypergraph, he_id)) {
                if (target_id == current_ctx.id)
                    continue; // Skip the source vertex

                traversal_context<HypergraphType> target_ctx{target_id, current_ctx.id, he_id};
                const auto enqueue = enqueue_vertex_pred(target_ctx);
                if (enqueue == gl::algorithm::decision::abort)
                    return false;
                if (enqueue)
                    ctx_queue.push(target_ctx);
            }
        }

        if constexpr (not gl::traits::c_empty_callback<PostVisitCallback>)
            post_visit(current_ctx);
    }

    return true;
}

} // namespace hgl::algorithm
