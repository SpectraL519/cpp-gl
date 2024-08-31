#pragma once

#include "gl/constants.hpp"
#include "gl/graph.hpp"

namespace gl::topology {

template <type_traits::c_directed_graph_type GraphType>
[[nodiscard]] GraphType clique(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices; source_id++)
        for (types::id_type destination_id = constants::initial_id; destination_id < n_vertices;
             destination_id++)
            if (source_id != destination_id)
                graph.add_edge(source_id, destination_id);

    return graph;
}

template <type_traits::c_undirected_graph_type GraphType>
[[nodiscard]] GraphType clique(const types::size_type n_vertices) {
    GraphType graph{n_vertices};

    for (types::id_type source_id = constants::initial_id; source_id < n_vertices; source_id++)
        for (types::id_type destination_id = constants::initial_id; destination_id < source_id;
             destination_id++)
            graph.add_edge(source_id, destination_id);

    return graph;
}

} // namespace gl::topology
