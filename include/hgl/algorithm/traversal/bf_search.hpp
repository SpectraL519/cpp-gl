// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/algorithm/core.hpp"
#include "hgl/algorithm/templates/bfs.hpp"
#include "hgl/algorithm/util.hpp"
#include "hgl/hypergraph.hpp"

namespace hgl::algorithm {

template <
    result_discriminator Result = ret,
    hgl::traits::c_bf_directed_hypergraph H,
    traits::c_forward_range_of<typename H::id_type> RootRange = std::vector<typename H::id_type>,
    gl::traits::c_optional_callback<void, const search_node<H>&> PreVisitCallback = empty_callback,
    gl::traits::c_optional_callback<void, const search_node<H>&> PostVisitCallback = empty_callback>
return_type<Result, search_tree<H>> backward_search(
    const H& hypergraph,
    const RootRange& root_vertices,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using id_type = typename H::id_type;

    std::vector<bool> visited_vertices(hypergraph.order(), false);
    auto tail_unvisited = hypergraph.tail_size_map() | std::ranges::to<std::vector>();

    auto stree = init_search_tree<Result>(hypergraph);
    auto root_queue =
        root_vertices
        | std::views::transform([](const id_type root_id) { return search_node<H>{root_id}; });

    const auto traverse_hyperedge_pred = [&tail_unvisited](id_type he_id, id_type) {
        return static_cast<decision>(--tail_unvisited[gl::to_idx(he_id)] == 0uz);
    };

    // clang-format off

    bfs(
        hypergraph,
        root_queue,
        default_visit_vertex_predicate<H>(visited_vertices),
        default_visit_callback<H, Result>(visited_vertices, stree),
        traverse_hyperedge_pred,
        default_enqueue_vertex_predicate<H, true>(visited_vertices),
        pre_visit,
        post_visit
    );

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

template <
    result_discriminator Result = ret,
    hgl::traits::c_bf_directed_hypergraph H,
    traits::c_forward_range_of<typename H::id_type> RootRange = std::vector<typename H::id_type>,
    gl::traits::c_optional_callback<void, const search_node<H>&> PreVisitCallback = empty_callback,
    gl::traits::c_optional_callback<void, const search_node<H>&> PostVisitCallback = empty_callback>
return_type<Result, search_tree<H>> forward_search(
    const H& hypergraph,
    const RootRange& root_vertices,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using id_type = typename H::id_type;

    std::vector<bool> visited_vertices(hypergraph.order(), false);
    auto head_unvisited = hypergraph.head_size_map() | std::ranges::to<std::vector>();

    auto stree = init_search_tree<Result>(hypergraph);
    auto root_queue =
        root_vertices
        | std::views::transform([](const id_type root_id) { return search_node<H>{root_id}; });

    const auto traverse_hyperedge_pred = [&head_unvisited](id_type he_id, id_type) {
        return static_cast<decision>(--head_unvisited[gl::to_idx(he_id)] == 0uz);
    };

    // clang-format off

    bfs<traversal_direction::backward>(
        hypergraph,
        root_queue,
        default_visit_vertex_predicate<H>(visited_vertices),
        default_visit_callback<H, Result>(visited_vertices, stree),
        traverse_hyperedge_pred,
        default_enqueue_vertex_predicate<H, true>(visited_vertices),
        pre_visit,
        post_visit
    );

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

} // namespace hgl::algorithm
