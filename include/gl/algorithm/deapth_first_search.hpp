// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "detail/dfs_impl.hpp"

namespace gl::algorithm {

template <
    type_traits::c_alg_return_graph_type SearchTreeType = types::no_return,
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
detail::alg_return_graph_type<SearchTreeType> depth_first_search(
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
        detail::dfs_impl(
            graph,
            graph.get_vertex(root_vertex_id_opt.value()),
            detail::default_visit_vertex_predicate<GraphType>(visited),
            detail::default_visit_callback<GraphType>(visited, search_tree),
            detail::default_enqueue_vertex_predicate<GraphType>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto& root_vertex : graph.vertices())
            detail::dfs_impl(
                graph,
                root_vertex,
                detail::default_visit_vertex_predicate<GraphType>(visited),
                detail::default_visit_callback<GraphType>(visited, search_tree),
                detail::default_enqueue_vertex_predicate<GraphType>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::c_alg_no_return_type<SearchTreeType>)
        return search_tree;
}

template <
    type_traits::c_alg_return_graph_type SearchTreeType = types::no_return,
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
detail::alg_return_graph_type<SearchTreeType> recursive_depth_first_search(
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
        const auto root_id = root_vertex_id_opt.value();
        detail::rdfs_impl(
            graph,
            graph.get_vertex(root_id),
            root_id,
            detail::default_visit_vertex_predicate<GraphType>(visited),
            detail::default_visit_callback<GraphType>(visited, search_tree),
            detail::default_enqueue_vertex_predicate<GraphType>(visited),
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
                detail::default_visit_vertex_predicate<GraphType>(visited),
                detail::default_visit_callback<GraphType>(visited, search_tree),
                detail::default_enqueue_vertex_predicate<GraphType>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::c_alg_no_return_type<SearchTreeType>)
        return search_tree;
}

} // namespace gl::algorithm
