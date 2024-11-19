// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "common.hpp"

#include <stack>

namespace gl::algorithm::impl {

template <
    type_traits::c_graph GraphType,
    type_traits::c_optional_vertex_callback<GraphType, bool> VisitVertexPredicate,
    type_traits::c_vertex_callback<GraphType, bool, types::id_type> VisitCallback,
    type_traits::
        c_vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>
            EnqueueVertexPred,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        algorithm::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        algorithm::empty_callback>
void dfs(
    const GraphType& graph,
    const typename GraphType::vertex_type& root_vertex,
    const VisitVertexPredicate& visit_vertex_pred,
    const VisitCallback& visit,
    const EnqueueVertexPred& enqueue_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_stack_type = std::stack<algorithm::vertex_info>;

    if constexpr (not type_traits::c_empty_callback<VisitVertexPredicate>)
        if (not visit_vertex_pred(root_vertex))
            return;

    // prepare the vertex stack
    vertex_stack_type vertex_stack;
    vertex_stack.emplace(root_vertex.id());

    // search the graph
    while (not vertex_stack.empty()) {
        const auto vinfo = vertex_stack.top();
        vertex_stack.pop();

        const auto& vertex = graph.get_vertex(vinfo.id);
        if constexpr (not type_traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(vertex))
                continue;

        if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
            pre_visit(vertex);

        visit(vertex, vinfo.source_id);

        for (const auto& edge : graph.adjacent_edges(vinfo.id)) {
            const auto& incident_vertex = edge.incident_vertex(vertex);
            if (enqueue_vertex_pred(incident_vertex, edge))
                vertex_stack.emplace(incident_vertex.id(), vinfo.id);
        }

        if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
            post_visit(vertex);
    }
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, bool> VisitVertexPredicate,
    type_traits::c_vertex_callback<GraphType, bool, types::id_type> VisitCallback,
    type_traits::
        c_vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>
            EnqueueVertexPred,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        algorithm::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        algorithm::empty_callback>
void r_dfs(
    const GraphType& graph,
    const typename GraphType::vertex_type& vertex,
    const types::id_type source_id,
    const VisitVertexPredicate& visit_vertex_pred,
    const VisitCallback& visit,
    const EnqueueVertexPred& enqueue_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if constexpr (not type_traits::c_empty_callback<VisitVertexPredicate>)
        if (not visit_vertex_pred(vertex))
            return;

    if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
        pre_visit(vertex);

    visit(vertex, source_id);

    // recursively search vertices adjacent to the current vertex
    const auto vertex_id = vertex.id();
    for (const auto& edge : graph.adjacent_edges(vertex_id)) {
        const auto& incident_vertex = edge.incident_vertex(vertex);
        if (enqueue_vertex_pred(incident_vertex, edge))
            r_dfs(
                graph,
                incident_vertex,
                vertex_id,
                visit_vertex_pred,
                visit,
                enqueue_vertex_pred,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
        post_visit(vertex);
}

} // namespace gl::algorithm::impl
