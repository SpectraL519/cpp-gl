#pragma once

#include "common.hpp"

#include <stack>

namespace gl::algorithm::detail {

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
void dfs_impl(
    const GraphType& graph,
    const typename GraphType::vertex_type& root_vertex,
    const vertex_callback<GraphType, bool>& visit_vertex_pred,
    const vertex_callback<GraphType, void, types::id_type>& visit,
    const vertex_callback<GraphType, bool, const typename GraphType::edge_type&>& enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_stack_type = std::stack<vertex_info>;

    if (not visit_vertex_pred(root_vertex))
        return;

    vertex_stack_type vertex_stack;
    vertex_stack.emplace(root_vertex.id());

    while (not vertex_stack.empty()) {
        const vertex_info sv = vertex_stack.top();
        vertex_stack.pop();

        const auto& vertex = graph.get_vertex(sv.id);
        if (not visit_vertex_pred(vertex))
            continue;

        if constexpr (not type_traits::is_empty_callback_v<PreVisitCallback>)
            pre_visit(vertex);

        visit(vertex, sv.source_id);

        for (const auto& edge : graph.adjacent_edges(sv.id)) {
            const auto& incident_vertex = edge.incident_vertex(vertex);
            if (enque_vertex_pred(incident_vertex, edge))
                vertex_stack.emplace(incident_vertex.id(), sv.id);
        }

        if constexpr (not type_traits::is_empty_callback_v<PostVisitCallback>)
            post_visit(vertex);
    }
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
void rdfs_impl(
    const GraphType& graph,
    const typename GraphType::vertex_type& vertex,
    const types::id_type source_id,
    const vertex_callback<GraphType, bool>& visit_vertex_pred,
    const vertex_callback<GraphType, void, types::id_type>& visit,
    const vertex_callback<GraphType, bool, const typename GraphType::edge_type&>& enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (not visit_vertex_pred(vertex))
        return;

    if constexpr (not type_traits::is_empty_callback_v<PreVisitCallback>)
        pre_visit(vertex);

    visit(vertex, source_id);

    const auto vertex_id = vertex.id();
    for (const auto& edge : graph.adjacent_edges(vertex_id)) {
        const auto& incident_vertex = edge.incident_vertex(vertex);
        if (enque_vertex_pred(incident_vertex, edge))
            rdfs_impl(
                graph,
                incident_vertex,
                vertex_id,
                visit_vertex_pred,
                visit,
                enque_vertex_pred,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::is_empty_callback_v<PostVisitCallback>)
        post_visit(vertex);
}

} // namespace gl::algorithm::detail
