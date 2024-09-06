#pragma once

#include "gl/algorithm/types.hpp"
#include "gl/graph.hpp"

#include <queue>

namespace gl::algorithm::detail {

template <type_traits::c_alg_return_graph SearchTreeType, type_traits::c_directed_graph GraphType>
[[nodiscard]] gl_attr_force_inline SearchTreeType init_search_tree(const GraphType& graph) {
    if constexpr (type_traits::is_alg_no_return_type_v<SearchTreeType>)
        return SearchTreeType{};
    else
        return SearchTreeType(graph.n_vertices());
}

struct search_vertex_info {
    search_vertex_info(types::id_type id) : id(id), source_id(id) {}

    search_vertex_info(types::id_type id, types::id_type source_id)
    : id(id), source_id(source_id) {}

    // if id == source_id then vertex_id is the id of the starting vertex
    types::id_type id;
    types::id_type source_id;
};

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
void rooted_bfs_impl(
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
    vertex_queue.push(search_vertex_info{root_vertex.id()});

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
                vertex_queue.push(search_vertex_info{incident_vertex.id(), sv.id});
        }

        if constexpr (not type_traits::is_empty_callback_v<PostVisitCallback>)
            post_visit(vertex);
    }
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
gl_attr_force_inline void bfs_impl(
    const GraphType& graph,
    const vertex_callback<GraphType, bool>& search_vertex_pred,
    const vertex_callback<GraphType, void, types::id_type>& visit,
    const vertex_callback<GraphType, bool, const typename GraphType::edge_type&>& enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // iterate over all vertices to search through all connected components
    for (const auto& root_vertex : graph.vertices())
        rooted_bfs_impl(
            graph, root_vertex, search_vertex_pred, visit, enque_vertex_pred, pre_visit, post_visit
        );
}

} // namespace gl::algorithm::detail
