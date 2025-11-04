// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "impl/dfs.hpp"

namespace gl::algorithm {

template <
    result_discriminator ResultDiscriminator = algorithm::ret,
    type_traits::c_graph GraphType,
    type_traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    type_traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
impl::alg_return_type<ResultDiscriminator, predecessors_descriptor> depth_first_search(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    std::vector<bool> visited(graph.n_vertices(), false);
    std::vector<types::id_type> sources(graph.n_vertices());

    auto pd = impl::init_return_value<ResultDiscriminator, predecessors_descriptor>(graph);

    if (root_vertex_id_opt) {
        impl::dfs(
            graph,
            root_vertex_id_opt.value(),
            impl::default_visit_vertex_predicate(visited),
            impl::default_visit_callback<ResultDiscriminator>(visited, pd),
            impl::default_enqueue_vertex_predicate<GraphType>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_vertex_id : graph.vertex_ids())
            impl::dfs(
                graph,
                root_vertex_id,
                impl::default_visit_vertex_predicate(visited),
                impl::default_visit_callback<ResultDiscriminator>(visited, pd),
                impl::default_enqueue_vertex_predicate<GraphType>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (ResultDiscriminator == algorithm::ret)
        return pd;
}

template <
    result_discriminator ResultDiscriminator = algorithm::ret,
    type_traits::c_graph GraphType,
    type_traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    type_traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
impl::alg_return_type<ResultDiscriminator, predecessors_descriptor> recursive_depth_first_search(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    std::vector<bool> visited(graph.n_vertices(), false);
    std::vector<types::id_type> sources(graph.n_vertices());

    auto pd = impl::init_return_value<ResultDiscriminator, predecessors_descriptor>(graph);

    if (root_vertex_id_opt) {
        const auto root_id = root_vertex_id_opt.value();
        impl::r_dfs(
            graph,
            root_id,
            root_id, // pred_id
            impl::default_visit_vertex_predicate(visited),
            impl::default_visit_callback<ResultDiscriminator>(visited, pd),
            impl::default_enqueue_vertex_predicate<GraphType>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto& root_id : graph.vertex_ids())
            impl::r_dfs(
                graph,
                root_id,
                root_id, // pred_id
                impl::default_visit_vertex_predicate(visited),
                impl::default_visit_callback<ResultDiscriminator>(visited, pd),
                impl::default_enqueue_vertex_predicate<GraphType>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (ResultDiscriminator == algorithm::ret)
        return pd;
}

} // namespace gl::algorithm
