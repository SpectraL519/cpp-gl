#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <type_traits::c_graph_type GraphType>
[[nodiscard]] GraphType path(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices - constants::one;
         source_id++)
        graph.add_edge(source_id, source_id + constants::one);

    return graph;
}

template <type_traits::c_graph_type GraphType>
[[nodiscard]] GraphType bidirectional_path(const types::size_type n_vertices) {
    if constexpr (type_traits::is_directed_v<GraphType>) {
        GraphType graph{n_vertices};

        for (types::id_type source_id = constants::initial_id;
             source_id < n_vertices - constants::one;
             source_id++) {
            const auto destination_id = source_id + constants::one;
            graph.add_edge(source_id, destination_id);
            graph.add_edge(destination_id, source_id);
        }

        return graph;
    }
    else {
        return path<GraphType>(n_vertices);
    }
}

} // namespace gl::topology
