#pragma once

#include "common.hpp"

#include <queue>

namespace gl::algorithm::detail {

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
void bfs_impl(
    const GraphType& graph,
    const typename GraphType::vertex_type& root_vertex,
    const vertex_callback<GraphType, bool>& search_vertex_pred,
    const vertex_callback<GraphType, void, types::id_type>& visit,
    const vertex_callback<GraphType, bool, const typename GraphType::edge_type&>& enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_queue_type = std::queue<search_vertex_info>;

    if (not search_vertex_pred(root_vertex))
        return;

    vertex_queue_type vertex_queue;
    vertex_queue.emplace(root_vertex.id());

    while (not vertex_queue.empty()) {
        const search_vertex_info sv = vertex_queue.front();
        vertex_queue.pop();

        const auto& vertex = graph.get_vertex(sv.id);
        if (not search_vertex_pred(vertex))
            continue;

        if constexpr (not type_traits::is_empty_callback_v<PreVisitCallback>)
            pre_visit(vertex);

        visit(vertex, sv.source_id);

        for (const auto& edge : graph.adjacent_edges(sv.id)) {
            const auto& incident_vertex = edge.incident_vertex(vertex);
            if (enque_vertex_pred(incident_vertex, edge))
                vertex_queue.emplace(incident_vertex.id(), sv.id);
        }

        if constexpr (not type_traits::is_empty_callback_v<PostVisitCallback>)
            post_visit(vertex);
    }
}

} // namespace gl::algorithm::detail
