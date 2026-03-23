// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/templates/bfs.hpp"

namespace gl::algorithm {

using bicoloring_type = std::vector<types::binary_color>;

template <
    traits::c_graph GraphType,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
[[nodiscard]] std::optional<bicoloring_type> bipartite_coloring(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using edge_type = typename GraphType::edge_type;

    std::optional<bicoloring_type> coloring_opt;
    coloring_opt.emplace(graph.order(), bin_color_value::unset);
    auto& coloring = coloring_opt.value();

    for (const auto root_id : graph.vertex_ids()) {
        if (coloring[root_id].is_set())
            continue;

        // color the root vertex
        coloring[root_id] = bin_color_value::black;

        const bool is_bipartite = bfs(
            graph,
            init_range(root_id),
            algorithm::empty_callback{}, // visit predicate
            algorithm::empty_callback{}, // visit callback
            [&coloring](const types::id_type vertex_id, const edge_type& in_edge)
                -> predicate_result { // enqueue predicate
                if (in_edge.is_loop())
                    return false;

                const auto pred_id = in_edge.incident_vertex(vertex_id);

                if (coloring[vertex_id] == coloring[pred_id])
                    return predicate_result::unknown; // graph is not bipartite

                if (not coloring[vertex_id].is_set()) {
                    coloring[vertex_id] = coloring[pred_id].next();
                    return true;
                }

                return false;
            },
            pre_visit,
            post_visit
        );

        if (not is_bipartite)
            return std::nullopt;
    }

    return coloring_opt;
}

[[nodiscard]] gl_attr_force_inline bool is_bipartite(const traits::c_graph auto& graph) {
    return bipartite_coloring(graph).has_value();
}

template <traits::c_graph GraphType, traits::c_sized_range_of<types::binary_color> ColorRange>
requires(traits::c_binary_color_properties_type<typename GraphType::vertex_properties_type>)
bool apply_coloring(GraphType& graph, const ColorRange& color_range) {
    if (std::ranges::size(color_range) != graph.order())
        return false;

    auto vertices = graph.vertices(); // store the view to extend its lifetime
    for (const auto& [vertex, color] : std::views::zip(vertices, color_range))
        vertex.properties().color = color;

    return true;
}

} // namespace gl::algorithm
