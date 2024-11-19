// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "impl/bfs.hpp"
#include "types.hpp"

namespace gl::algorithm {

template <
    type_traits::c_alg_return_type AlgReturnType = algorithm::default_return,
    type_traits::c_graph GraphType,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        algorithm::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        algorithm::empty_callback>
impl::alg_return_type<AlgReturnType, predecessors_descriptor> breadth_first_search(
    const GraphType& graph,
    const std::optional<types::id_type>& root_vertex_id_opt = no_root_vertex,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    std::vector<bool> visited(graph.n_vertices(), false);
    std::vector<types::id_type> sources(graph.n_vertices());

    auto pd = impl::init_return_value<AlgReturnType, predecessors_descriptor>(graph);

    if (root_vertex_id_opt) {
        impl::bfs(
            graph,
            impl::init_range(root_vertex_id_opt.value()),
            impl::default_visit_vertex_predicate<GraphType>(visited),
            impl::default_visit_callback<GraphType, AlgReturnType>(visited, pd),
            impl::default_enqueue_vertex_predicate<GraphType, true>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_id : graph.vertex_ids())
            impl::bfs(
                graph,
                impl::init_range(root_id),
                impl::default_visit_vertex_predicate<GraphType>(visited),
                impl::default_visit_callback<GraphType, AlgReturnType>(visited, pd),
                impl::default_enqueue_vertex_predicate<GraphType, true>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (not type_traits::c_alg_no_return_type<AlgReturnType>)
        return pd;
}

} // namespace gl::algorithm
