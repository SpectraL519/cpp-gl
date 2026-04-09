// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/algorithm/core.hpp"
#include "hgl/algorithm/templates/bfs.hpp"
#include "hgl/algorithm/util.hpp"

namespace hgl::algorithm {

template <
    result_discriminator ResultDiscriminator = ret,
    hgl::traits::c_hypergraph HypergraphType,
    gl::traits::c_optional_callback<void, const traversal_context<HypergraphType>&>
        PreVisitCallback = empty_callback,
    gl::traits::c_optional_callback<void, const traversal_context<HypergraphType>&>
        PostVisitCallback = empty_callback>
return_type<ResultDiscriminator, predecessors_map<HypergraphType>> breadth_first_search(
    const HypergraphType& hypergraph,
    const typename HypergraphType::id_type root_vertex_id = no_root,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // Two state vectors required for efficient hypergraph traversal
    std::vector<bool> visited_vertices(hypergraph.order(), false);
    std::vector<bool> visited_hyperedges(hypergraph.size(), false);

    auto pred_map = init_predecessors_map<ResultDiscriminator>(hypergraph);

    // clang-format off

    if (root_vertex_id != no_root) {
        bfs(
            hypergraph,
            init_range<HypergraphType>(root_vertex_id),
            default_visit_vertex_predicate<HypergraphType>(visited_vertices),
            default_visit_callback<HypergraphType, ResultDiscriminator>(visited_vertices, pred_map),
            default_traverse_hyperedge_predicate(visited_hyperedges), // Pass the hyperedge state!
            default_enqueue_vertex_predicate<HypergraphType, true>(visited_vertices),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_id : hypergraph.vertex_ids())
            bfs(
                hypergraph,
                init_range<HypergraphType>(root_id),
                default_visit_vertex_predicate<HypergraphType>(visited_vertices),
                default_visit_callback<HypergraphType, ResultDiscriminator>(visited_vertices, pred_map),
                default_traverse_hyperedge_predicate(visited_hyperedges), // Pass the hyperedge state!
                default_enqueue_vertex_predicate<HypergraphType, true>(visited_vertices),
                pre_visit,
                post_visit
            );
    }

    // clang-format on

    if constexpr (ResultDiscriminator == ret)
        return pred_map;
}

} // namespace hgl::algorithm
