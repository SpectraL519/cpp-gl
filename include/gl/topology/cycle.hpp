#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <type_traits::c_graph_type GraphType>
[[nodiscard]] GraphType cycle(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices; source_id++)
        graph.add_edge(source_id, (source_id + constants::one_element) % n_vertices);

    return graph;
}

template <type_traits::c_directed_graph_type GraphType>
[[nodiscard]] GraphType bidirectional_cycle(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices; source_id++) {
        const auto destination_id = (source_id + constants::one_element) % n_vertices;
        graph.add_edge(source_id, destination_id);
        graph.add_edge(destination_id, source_id);
    }

    return graph;
}

} // namespace gl::topology
