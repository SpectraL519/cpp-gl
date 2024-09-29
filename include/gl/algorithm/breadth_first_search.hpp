#pragma once

#include "constants.hpp"
#include "detail/bfs_impl.hpp"

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

    if (root_vertex_id_opt) {
        detail::bfs_impl(
            graph,
            detail::init_range(root_vertex_id_opt.value()),
            detail::default_visit_vertex_predicate<GraphType>(visited),
            detail::default_visit_callback<GraphType>(visited, search_tree),
            detail::default_enqueue_vertex_predicate<GraphType, true>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_id : graph.vertex_ids())
            detail::bfs_impl(
                graph,
                detail::init_range(root_id),
                detail::default_visit_vertex_predicate<GraphType>(visited),
                detail::default_visit_callback<GraphType>(visited, search_tree),
                detail::default_enqueue_vertex_predicate<GraphType, true>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::is_alg_no_return_type_v<SearchTreeType>)
        return search_tree;
}

} // namespace gl::algorithm