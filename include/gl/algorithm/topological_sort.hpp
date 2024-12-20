// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "impl/bfs.hpp"

namespace gl::algorithm {

template <
    type_traits::c_directed_graph GraphType,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        algorithm::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        algorithm::empty_callback>
[[nodiscard]] std::optional<std::vector<types::id_type>> topological_sort(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    const auto vertex_ids = graph.vertex_ids();

    // prepare the vertex in degree map
    std::vector<types::size_type> in_degree_map = graph.in_degree_map();

    // prepare the initial queue content (source vertices)
    std::vector<algorithm::vertex_info> source_vertex_list;
    source_vertex_list.reserve(graph.n_vertices());
    for (const auto id : vertex_ids)
        if (in_degree_map[id] == constants::default_size)
            source_vertex_list.emplace_back(id);

    std::optional<std::vector<types::id_type>> topological_order_opt =
        std::vector<types::id_type>{};
    auto& topological_order = topological_order_opt.value();
    topological_order.reserve(graph.n_vertices());

    impl::bfs(
        graph,
        source_vertex_list,
        algorithm::empty_callback{}, // visit predicate
        [&topological_order](
            const vertex_type& vertex, const types::id_type source_id
        ) { // visit callback
            topological_order.push_back(vertex.id());
            return true;
        },
        [&in_degree_map](const vertex_type& vertex, const edge_type& in_edge)
            -> std::optional<bool> { // enqueue predicate
            if (in_edge.is_loop())
                return false;
            return --in_degree_map[vertex.id()] == constants::default_size;
        },
        pre_visit,
        post_visit
    );

    if (topological_order.size() != graph.n_vertices())
        return std::nullopt;

    return topological_order_opt;
}

} // namespace gl::algorithm
