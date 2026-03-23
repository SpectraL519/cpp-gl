// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/impl/common.hpp"

#include <stack>

namespace gl::algorithm::impl {

template <
    traits::c_graph GraphType,
    traits::c_optional_id_callback<bool> VisitVertexPredicate,
    traits::c_optional_id_callback<bool, types::id_type> VisitCallback,
    traits::c_id_callback<predicate_result, const typename GraphType::edge_type&> EnqueueVertexPred,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
void dfs(
    const GraphType& graph,
    const types::id_type root_id,
    const VisitVertexPredicate& visit_vertex_pred,
    const VisitCallback& visit,
    const EnqueueVertexPred& enqueue_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_stack_type = std::stack<algorithm::vertex_info>;

    if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
        if (not visit_vertex_pred(root_id))
            return;

    // prepare the vertex stack
    vertex_stack_type vertex_stack;
    vertex_stack.emplace(root_id);

    // search the graph
    while (not vertex_stack.empty()) {
        const auto vinfo = vertex_stack.top();
        vertex_stack.pop();

        if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(vinfo.id))
                continue;

        if constexpr (not traits::c_empty_callback<PreVisitCallback>)
            pre_visit(vinfo.id);

        visit(vinfo.id, vinfo.pred_id);

        for (const auto& edge : graph.adjacent_edges(vinfo.id)) {
            const auto incident_vertex_id = edge.incident_vertex(vinfo.id);
            if (enqueue_vertex_pred(incident_vertex_id, edge))
                vertex_stack.emplace(incident_vertex_id, vinfo.id);
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(vinfo.id);
    }
}

template <
    traits::c_graph GraphType,
    traits::c_optional_id_callback<bool> VisitVertexPredicate,
    traits::c_optional_id_callback<bool, types::id_type> VisitCallback,
    traits::c_id_callback<predicate_result, const typename GraphType::edge_type&> EnqueueVertexPred,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
void r_dfs(
    const GraphType& graph,
    const types::id_type vertex_id,
    const types::id_type pred_id,
    const VisitVertexPredicate& visit_vertex_pred,
    const VisitCallback& visit,
    const EnqueueVertexPred& enqueue_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
        if (not visit_vertex_pred(vertex_id))
            return;

    if constexpr (not traits::c_empty_callback<PreVisitCallback>)
        pre_visit(vertex_id);

    visit(vertex_id, pred_id);

    // recursively search vertices adjacent to the current vertex
    for (const auto& edge : graph.adjacent_edges(vertex_id)) {
        const auto& incident_vertex_id = edge.incident_vertex(vertex_id);
        if (enqueue_vertex_pred(incident_vertex_id, edge))
            r_dfs(
                graph,
                incident_vertex_id,
                vertex_id,
                visit_vertex_pred,
                visit,
                enqueue_vertex_pred,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not traits::c_empty_callback<PostVisitCallback>)
        post_visit(vertex_id);
}

} // namespace gl::algorithm::impl
