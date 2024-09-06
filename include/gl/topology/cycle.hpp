#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <type_traits::c_graph GraphType>
[[nodiscard]] GraphType cycle(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices; source_id++)
        graph.add_edge(source_id, (source_id + constants::one) % n_vertices);

    return graph;
}

template <type_traits::c_graph GraphType>
[[nodiscard]] GraphType bidirectional_cycle(const types::size_type n_vertices) {
    if constexpr (type_traits::is_directed_v<GraphType>) {
        GraphType graph{n_vertices};

        for (types::id_type source_id = constants::initial_id; source_id < n_vertices;
             source_id++) {
            const auto destination_id = (source_id + constants::one) % n_vertices;
            graph.add_edge(source_id, destination_id);
            graph.add_edge(destination_id, source_id);
        }

        return graph;
    }
    else {
        return cycle<GraphType>(n_vertices);
    }
}

} // namespace gl::topology
