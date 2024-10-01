// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "detail/bfs_impl.hpp"
#include "gl/types/properties.hpp"

namespace gl::algorithm {

using bicoloring_type = std::vector<types::binary_color>;

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
[[nodiscard]] std::optional<bicoloring_type> bipartite_coloring(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;

    bicoloring_type coloring(graph.n_vertices(), bin_color_value::unset);

    for (const auto& root_vertex : graph.vertices()) {
        const auto root_id = root_vertex.id();
        if (coloring[root_id].is_set())
            continue;

        // color the root vertex
        coloring[root_id] = bin_color_value::black;

        const bool is_bipartite = detail::bfs_impl(
            graph,
            detail::init_range(root_id),
            detail::constant_unary_predicate<true>(), // visit predicate
            detail::constant_binary_predicate<true>(), // visit callback
            [&coloring](const vertex_type& vertex, const edge_type& in_edge)
                -> std::optional<bool> { // enqueue predicate
                if (in_edge.is_loop())
                    return false;

                const auto vertex_id = vertex.id();
                const auto source_id = in_edge.incident_vertex(vertex).id();

                if (coloring[vertex_id] == coloring[source_id])
                    return std::nullopt; // graph is not bipartite

                if (not coloring[vertex.id()].is_set()) {
                    coloring[vertex.id()] = coloring[source_id].next();
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

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline bool is_bipartite(const GraphType& graph) {
    return bipartite_coloring(graph).has_value();
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_sized_range_of<types::binary_color> ColorRange>
requires(type_traits::c_binary_color_properties_type<typename GraphType::vertex_properties_type>)
bool apply_coloring(GraphType& graph, const ColorRange& color_range) {
    if (color_range.size() != graph.n_vertices())
        return false;

    using color_type = typename GraphType::vertex_properties_type::color_type;
    auto color_it = std::ranges::begin(color_range);

    for (const auto& vertex : graph.vertices())
        vertex.properties.color = color_type{*color_it++};

    return true;
}

} // namespace gl::algorithm
