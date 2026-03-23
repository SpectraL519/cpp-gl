// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/constants.hpp"
#include "gl/algorithm/templates/dfs.hpp"

namespace gl::algorithm {

template <
    result_discriminator ResultDiscriminator = algorithm::ret,
    traits::c_graph GraphType,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
return_type<ResultDiscriminator, predecessors_descriptor> depth_first_search(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited(graph.order(), false);
    std::vector<types::id_type> sources(graph.order());

    auto pd = init_return_value<ResultDiscriminator, predecessors_descriptor>(graph);

    if (root_vertex_id_opt) {
        dfs(graph,
            root_vertex_id_opt.value(),
            default_visit_vertex_predicate(visited),
            default_visit_callback<ResultDiscriminator>(visited, pd),
            default_enqueue_vertex_predicate<GraphType>(visited),
            pre_visit,
            post_visit);
    }
    else {
        for (const auto root_vertex_id : graph.vertex_ids())
            dfs(graph,
                root_vertex_id,
                default_visit_vertex_predicate(visited),
                default_visit_callback<ResultDiscriminator>(visited, pd),
                default_enqueue_vertex_predicate<GraphType>(visited),
                pre_visit,
                post_visit);
    }

    if constexpr (ResultDiscriminator == algorithm::ret)
        return pd;
}

template <
    result_discriminator ResultDiscriminator = algorithm::ret,
    traits::c_graph GraphType,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
return_type<ResultDiscriminator, predecessors_descriptor> recursive_depth_first_search(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited(graph.order(), false);
    std::vector<types::id_type> sources(graph.order());

    auto pd = init_return_value<ResultDiscriminator, predecessors_descriptor>(graph);

    if (root_vertex_id_opt) {
        const auto root_id = root_vertex_id_opt.value();
        r_dfs(
            graph,
            root_id,
            root_id, // pred_id
            default_visit_vertex_predicate(visited),
            default_visit_callback<ResultDiscriminator>(visited, pd),
            default_enqueue_vertex_predicate<GraphType>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto& root_id : graph.vertex_ids())
            r_dfs(
                graph,
                root_id,
                root_id, // pred_id
                default_visit_vertex_predicate(visited),
                default_visit_callback<ResultDiscriminator>(visited, pd),
                default_enqueue_vertex_predicate<GraphType>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (ResultDiscriminator == algorithm::ret)
        return pd;
}

} // namespace gl::algorithm
