// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/bfs.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/constants.hpp"

namespace gl::algorithm {

template <
    result_discriminator Result = ret,
    traits::c_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
return_type<Result, predecessors_map<G>> breadth_first_search(
    const G& graph,
    const typename G::id_type root_vertex_id = no_root,
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    std::vector<bool> visited(graph.order(), false);
    std::vector<typename G::id_type> sources(graph.order());

    auto pred_map = init_predecessors_map<Result>(graph);

    // clang-format off

    if (root_vertex_id != no_root) {
        bfs(
            graph,
            init_range<G>(root_vertex_id),
            default_visit_vertex_predicate(visited),
            default_visit_callback<G, Result>(visited, pred_map),
            default_enqueue_vertex_predicate<G, true>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_id : graph.vertex_ids())
            bfs(
                graph,
                init_range<G>(root_id),
                default_visit_vertex_predicate(visited),
                default_visit_callback<G, Result>(visited, pred_map),
                default_enqueue_vertex_predicate<G, true>(visited),
                pre_visit,
                post_visit
            );
    }

    // clang-format on

    if constexpr (Result == ret)
        return pred_map;
}

} // namespace gl::algorithm
