#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <type_traits::c_directed_graph_type GraphType>
[[nodiscard]] GraphType clique(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type first_id = constants::initial_id; first_id < n_vertices; first_id++)
        for (types::id_type second_id = constants::initial_id; second_id < n_vertices; second_id++)
            if (first_id != second_id)
                graph.add_edge(first_id, second_id);

    return graph;
}

template <type_traits::c_undirected_graph_type GraphType>
[[nodiscard]] GraphType clique(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type first_id = constants::initial_id; first_id < n_vertices; first_id++)
        for (types::id_type second_id = constants::initial_id; second_id < first_id; second_id++)
            graph.add_edge(first_id, second_id);

    return graph;
}

} // namespace gl::topology
