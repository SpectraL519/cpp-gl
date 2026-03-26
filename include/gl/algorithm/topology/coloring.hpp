// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/templates/bfs.hpp"

namespace gl::algorithm {

using bicoloring_type = std::vector<binary_color>;

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

    bicoloring_type coloring(graph.order(), binary_color::value::unset);
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
            [&coloring](const id_type vertex_id, const edge_type& in_edge)
                -> predicate_result { // enqueue predicate
                if (in_edge.is_loop())
                    return predicate_result::unknown; // graph is not bipartite

                const auto pred_id = in_edge.incident_vertex(vertex_id);

                auto& v_color = coloring[static_cast<size_type>(vertex_id)];
                auto p_color = coloring[static_cast<size_type>(pred_id)];

                if (v_color == p_color)
                    return predicate_result::unknown; // graph is not bipartite

                if (not v_color.is_set()) {
                    v_color = p_color.next();
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

    return coloring;
}

[[nodiscard]] gl_attr_force_inline bool is_bipartite(const traits::c_graph auto& graph) {
    return bipartite_coloring(graph).has_value();
}

template <traits::c_graph GraphType, traits::c_sized_range_of<binary_color> ColorRange>
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
