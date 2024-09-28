#pragma once

#include "detail/bfs_impl.hpp"
#include "gl/types/properties.hpp"

#include <iostream>

namespace gl::algorithm {

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
[[nodiscard]] std::optional<std::vector<types::binary_color>> bipartite_coloring(
    const GraphType& graph,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using color = types::binary_color::value;

    std::vector<types::binary_color> coloring(graph.n_vertices(), color::unset);

    for (const auto& root_vertex : graph.vertices()) {
        const auto root_id = root_vertex.id();
        if (coloring[root_id].is_set())
            continue;

        // color the root vertex
        coloring[root_id] = color::black;

        const bool is_bipartite = detail::bfs_impl(
            graph,
            detail::init_range(root_id),
            detail::constant_unary_predicate<true>(), // visit pred
            detail::constant_binary_predicate<true>(), // visit callback
            [&coloring](const vertex_type& vertex, const edge_type& in_edge)
                -> std::optional<bool> { // enqueue pred
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

} // namespace gl::algorithm
