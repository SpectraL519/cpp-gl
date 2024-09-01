#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <type_traits::c_graph_type GraphType>
[[nodiscard]] GraphType full_bipartite(
    const types::size_type n_vertices_a, const types::size_type n_vertices_b
) {
    const auto n_vertices = n_vertices_a + n_vertices_b;
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices_a; source_id++) {
        for (types::id_type destination_id = n_vertices_a; destination_id < n_vertices;
             destination_id++) {
            graph.add_edge(source_id, destination_id);
            if constexpr (type_traits::is_directed_v<GraphType>)
                graph.add_edge(destination_id, source_id);
        }
    }

    return graph;
}

} // namespace gl::topology
