// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "impl/bfs_impl.hpp"

namespace gl::algorithm {

template <
    type_traits::c_directed_graph GraphType,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        types::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        types::empty_callback>
[[nodiscard]] std::optional<std::vector<types::id_type>> topological_sort(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    const auto vertex_ids = graph.vertex_ids();

    // prepare the vertex in degree list
    std::vector<types::size_type> vertex_in_deg_list;
    vertex_in_deg_list.reserve(graph.n_vertices());
    for (const auto id : vertex_ids)
        vertex_in_deg_list.push_back(graph.in_degree(id));

    // prepare the initial queue content (source vertices)
    std::vector<types::vertex_info> source_vertex_list;
    source_vertex_list.reserve(graph.n_vertices());
    for (const auto id : vertex_ids)
        if (vertex_in_deg_list[id] == constants::default_size)
            source_vertex_list.emplace_back(id);

    std::optional<std::vector<types::id_type>> topological_order_opt =
        std::vector<types::id_type>{};
    auto& topological_order = topological_order_opt.value();
    topological_order.reserve(graph.n_vertices());

    impl::bfs_impl(
        graph,
        source_vertex_list,
        types::empty_callback{}, // visit predicate
        [&topological_order](
            const vertex_type& vertex, const types::id_type source_id
        ) { // visit callback
            topological_order.push_back(vertex.id());
            return true;
        },
        [&vertex_in_deg_list](const vertex_type& vertex, const edge_type& in_edge)
            -> std::optional<bool> { // enqueue predicate
            if (in_edge.is_loop())
                return false;
            return --vertex_in_deg_list[vertex.id()] == constants::default_size;
        },
        pre_visit,
        post_visit
    );

    if (topological_order.size() != graph.n_vertices())
        return std::nullopt;

    return topological_order_opt;
}

} // namespace gl::algorithm
