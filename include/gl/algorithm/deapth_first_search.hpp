#pragma once

#include "constants.hpp"
#include "detail/search_algorithm_util.hpp"

#include <queue>

namespace gl::algorithm {

template <
    type_traits::c_alg_return_graph SearchTreeType = no_return,
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
type_traits::alg_return_type<SearchTreeType> depth_first_search(
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

    const auto search_vertex_pred = detail::default_search_vertex_predicate<GraphType>(visited);
    const auto visit = detail::default_visit_callback<GraphType>(visited, search_tree);
    const auto enque_vertex_pred = detail::default_enqueue_vertex_predicate<GraphType>(visited);

    if (root_vertex_id_opt) {
        detail::dfs_impl(
            graph,
            graph.get_vertex(root_vertex_id_opt.value()),
            search_vertex_pred,
            visit,
            enque_vertex_pred,
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto& root_vertex : graph.vertices())
            detail::dfs_impl(
                graph,
                root_vertex,
                search_vertex_pred,
                visit,
                enque_vertex_pred,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::is_alg_no_return_type_v<SearchTreeType>)
        return search_tree;
}

template <
    type_traits::c_alg_return_graph SearchTreeType = no_return,
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
type_traits::alg_return_type<SearchTreeType> recursive_depth_first_search(
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

    const auto search_vertex_pred = detail::default_search_vertex_predicate<GraphType>(visited);
    const auto visit = detail::default_visit_callback<GraphType>(visited, search_tree);
    const auto enque_vertex_pred = detail::default_enqueue_vertex_predicate<GraphType>(visited);

    if (root_vertex_id_opt) {
        const auto root_id = root_vertex_id_opt.value();
        detail::rdfs_impl(
            graph,
            graph.get_vertex(root_id),
            root_id,
            search_vertex_pred,
            visit,
            enque_vertex_pred,
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto& root_vertex : graph.vertices())
            detail::rdfs_impl(
                graph,
                root_vertex,
                root_vertex.id(),
                search_vertex_pred,
                visit,
                enque_vertex_pred,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::is_alg_no_return_type_v<SearchTreeType>)
        return search_tree;
}

} // namespace gl::algorithm
