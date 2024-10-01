#pragma once

#include "common.hpp"

#include <stack>

namespace gl::algorithm::detail {

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
void dfs_impl(
    const GraphType& graph,
    const typename GraphType::vertex_type& root_vertex,
    const types::vertex_callback<GraphType, bool>& visit_vertex_pred,
    const types::vertex_callback<GraphType, void, types::id_type>& visit,
    const types::vertex_callback<GraphType, bool, const typename GraphType::edge_type&>&
        enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_stack_type = std::stack<types::vertex_info>;

    if (not visit_vertex_pred(root_vertex))
        return;

    vertex_stack_type vertex_stack;
    vertex_stack.emplace(root_vertex.id());

    while (not vertex_stack.empty()) {
        const auto vinfo = vertex_stack.top();
        vertex_stack.pop();

        const auto& vertex = graph.get_vertex(vinfo.id);
        if (not visit_vertex_pred(vertex))
            continue;

        if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
            pre_visit(vertex);

        visit(vertex, vinfo.source_id);

        for (const auto& edge : graph.adjacent_edges(vinfo.id)) {
            const auto& incident_vertex = edge.incident_vertex(vertex);
            if (enque_vertex_pred(incident_vertex, edge))
                vertex_stack.emplace(incident_vertex.id(), vinfo.id);
        }

        if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
            post_visit(vertex);
    }
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
void rdfs_impl(
    const GraphType& graph,
    const typename GraphType::vertex_type& vertex,
    const types::id_type source_id,
    const types::vertex_callback<GraphType, bool>& visit_vertex_pred,
    const types::vertex_callback<GraphType, void, types::id_type>& visit,
    const types::vertex_callback<GraphType, bool, const typename GraphType::edge_type&>&
        enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (not visit_vertex_pred(vertex))
        return;

    if constexpr (not type_traits::c_empty_callback<PreVisitCallback>)
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

    if constexpr (not type_traits::c_empty_callback<PostVisitCallback>)
        post_visit(vertex);
}

} // namespace gl::algorithm::detail
