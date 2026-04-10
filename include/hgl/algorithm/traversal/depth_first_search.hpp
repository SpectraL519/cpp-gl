// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/algorithm/core.hpp"
#include "hgl/algorithm/templates/dfs.hpp"
#include "hgl/algorithm/util.hpp"

namespace hgl::algorithm {

template <
    result_discriminator Result = ret,
    traits::c_hypergraph H,
    traits::c_optional_callback<void, const search_node<H>&> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, const search_node<H>&> PostVisitCallback = empty_callback>
return_type<Result, search_tree<H>> depth_first_search(
    const H& hypergraph,
    const typename H::id_type root_vertex_id = no_root,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited_vertices(hypergraph.order(), false);
    std::vector<bool> visited_hyperedges(hypergraph.size(), false);

    auto stree = init_search_tree<Result>(hypergraph);

    // clang-format off

    if (root_vertex_id != no_root) {
        dfs(
            hypergraph,
            init_range<H>(root_vertex_id),
            default_visit_vertex_predicate<H>(visited_vertices),
            default_visit_callback<H, Result>(visited_vertices, stree),
            default_traverse_hyperedge_predicate(visited_hyperedges),
            default_enqueue_vertex_predicate<H, true>(visited_vertices),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_id : hypergraph.vertex_ids())
            dfs(
                hypergraph,
                init_range<H>(root_id),
                default_visit_vertex_predicate<H>(visited_vertices),
                default_visit_callback<H, Result>(visited_vertices, stree),
                default_traverse_hyperedge_predicate(visited_hyperedges),
                default_enqueue_vertex_predicate<H, true>(visited_vertices),
                pre_visit,
                post_visit
            );
    }

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

} // namespace hgl::algorithm
