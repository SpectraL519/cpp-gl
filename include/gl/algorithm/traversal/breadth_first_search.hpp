// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/bfs.hpp"
#include "gl/algorithm/traits.hpp"

namespace gl::algorithm {

template <
    result_discriminator ResultDiscriminator = algorithm::ret,
    traits::c_graph GraphType,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
return_type<ResultDiscriminator, predecessors_map> breadth_first_search(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited(graph.order(), false);
    std::vector<types::id_type> sources(graph.order());

    auto pred_map = init_predecessors_map<ResultDiscriminator>(graph);

    if (root_vertex_id_opt) {
        bfs(graph,
            init_range(root_vertex_id_opt.value()),
            default_visit_vertex_predicate(visited),
            default_visit_callback<ResultDiscriminator>(visited, pred_map),
            default_enqueue_vertex_predicate<GraphType, true>(visited),
            pre_visit,
            post_visit);
    }
    else {
        for (const auto root_id : graph.vertex_ids())
            bfs(graph,
                init_range(root_id),
                default_visit_vertex_predicate(visited),
                default_visit_callback<ResultDiscriminator>(visited, pred_map),
                default_enqueue_vertex_predicate<GraphType, true>(visited),
                pre_visit,
                post_visit);
    }

    if constexpr (ResultDiscriminator == algorithm::ret)
        return pred_map;
}

} // namespace gl::algorithm
