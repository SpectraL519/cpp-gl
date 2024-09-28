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

    const auto get_color_str = [](const types::binary_color& c) {
        switch (color{c.to_underlying()}) {
        case color::black:
            return "black";
        case color::white:
            return "white";
        default:
            return "unset";
        }
    };

    const auto print_coloring = [&coloring, &get_color_str]() {
        for (int i = 0; i < coloring.size(); i++)
            std::cout << std::format("{} : {}\n", i, get_color_str(coloring[i]));
        std::cout << std::endl;
    };

    // std::cout << "begin coloring:\n";
    // print_coloring();

    for (const auto& root_vertex : graph.vertices()) {
        const auto root_id = root_vertex.id();
        if (coloring[root_id].is_set())
            continue;

        // color the root vertex
        coloring[root_id] = color::black;

        // std::cout << std::format("bfs rooted in {}\n- initial coloring:\n", root_id);
        // print_coloring();

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

        // std::cout << "- final coloring:\n";
        // print_coloring();

        if (not is_bipartite)
            return std::nullopt;
    }

    return coloring;
}

} // namespace gl::algorithm
