#pragma once

#include "constants.hpp"
#include "detail/search_algorithm_util.hpp"

#include <iostream>
#include <queue>

namespace gl::algorithm {

template <
    type_traits::c_alg_return_graph SearchTreeType = no_return,
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
type_traits::alg_return_type<SearchTreeType> breadth_first_search(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    std::vector<bool> visited(graph.n_vertices(), false);
    std::vector<types::id_type> sources(graph.n_vertices());

    auto search_tree = detail::init_search_tree<SearchTreeType>(graph);

    const auto search_vertex_predicate = [&visited](const vertex_type& vertex) -> bool {
        return not visited[vertex.id()];
    };

    const auto visit = [&](const vertex_type& vertex, const types::id_type source_id) {
        const auto vertex_id = vertex.id();
        visited[vertex_id] = true;
        if constexpr (not type_traits::is_alg_no_return_type_v<SearchTreeType>) {
            if (source_id != vertex_id)
                search_tree.add_edge(source_id, vertex_id);
        }
    };

    const auto enque_vertex_predicate =
        [&visited](const vertex_type& vertex, const edge_type& in_edge) -> bool {
        return not visited[vertex.id()];
    };

    if (root_vertex_id_opt) {
        detail::bfs_impl(
            graph,
            graph.get_vertex(root_vertex_id_opt.value()),
            search_vertex_predicate,
            visit,
            enque_vertex_predicate,
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto& root_vertex : graph.vertices())
            detail::bfs_impl(
                graph,
                root_vertex,
                search_vertex_predicate,
                visit,
                enque_vertex_predicate,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::is_alg_no_return_type_v<SearchTreeType>)
        return search_tree;
}

} // namespace gl::algorithm
