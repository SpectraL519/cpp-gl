#pragma once

#include "gl/algorithm/types.hpp"
#include "gl/graph.hpp"

#include <queue>
#include <stack>

namespace gl::algorithm::detail {

template <type_traits::c_alg_return_graph SearchTreeType, type_traits::c_graph GraphType>
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

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline auto default_search_vertex_predicate(std::vector<bool>& visited
) {
    return [&](const typename GraphType::vertex_type& vertex) -> bool {
        return not visited[vertex.id()];
    };
}

template <type_traits::c_graph GraphType, type_traits::c_alg_return_graph SearchTreeType>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited, SearchTreeType& search_tree
) {
    return [&](const typename GraphType::vertex_type& vertex, const types::id_type source_id) {
        const auto vertex_id = vertex.id();
        visited[vertex_id] = true;
        if constexpr (not type_traits::is_alg_no_return_type_v<SearchTreeType>) {
            if (source_id != vertex_id)
                search_tree.add_edge(source_id, vertex_id);
        }
    };
}

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_vertex_predicate(std::vector<bool>& visited
) {
    return [&](const typename GraphType::vertex_type& vertex,
               const typename GraphType::edge_type& in_edge) -> bool {
        return not visited[vertex.id()];
    };
}

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

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
void dfs_impl(
    const GraphType& graph,
    const typename GraphType::vertex_type& root_vertex,
    const vertex_callback<GraphType, bool>& search_vertex_pred,
    const vertex_callback<GraphType, void, types::id_type>& visit,
    const vertex_callback<GraphType, bool, const typename GraphType::edge_type&>& enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_stack_type = std::stack<search_vertex_info>;

    if (not search_vertex_pred(root_vertex))
        return;

    vertex_stack_type vertex_stack;
    vertex_stack.emplace(root_vertex.id());

    while (not vertex_stack.empty()) {
        const search_vertex_info sv = vertex_stack.top();
        vertex_stack.pop();

        const auto& vertex = graph.get_vertex(sv.id);
        if (not search_vertex_pred(vertex))
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
    const vertex_callback<GraphType, bool>& search_vertex_pred,
    const vertex_callback<GraphType, void, types::id_type>& visit,
    const vertex_callback<GraphType, bool, const typename GraphType::edge_type&>& enque_vertex_pred,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (not search_vertex_pred(vertex))
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
                search_vertex_pred,
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
