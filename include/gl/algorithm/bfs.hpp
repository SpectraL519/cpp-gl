#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "gl/algorithm/callbacks.hpp"
#include "gl/graph/graph_traits.hpp"



namespace gl {

// TODO:
// * remove at() method from abstract_graph_base and mutable_graph
// * use get_vertex() instead of at() in dfs algorithms

template <gl_graph_c Graph>
void breadth_first_search(
    const std::unique_ptr<Graph>& graph,
    vertex_ptr_type_callback<Graph> pre_visit = {},
    vertex_ptr_type_callback<Graph> post_visit = {}
) {
    // TODO (*): account for mutable_graph (some keys can be missing)

    using key_type = typename Graph::vertex_key_type;

    const auto num_vertices = graph->num_vertices();
    std::vector<bool> visited(num_vertices, false); // TODO: (*)
    std::queue<key_type> key_queue;

    for (auto& vertex : graph->vertices()) {
        if (visited.at(vertex->key)) // TODO: (*)
            continue;

        if (pre_visit)
            pre_visit(vertex);

        while (not key_queue.empty()) {
            visited.at(vertex->key) = true; // TODO: (*)

            for (auto& edge : vertex->adjacent()) {
                if (visited.at(edge->destination))
                    continue;

                key_queue.push(edge->destination);
            }
        }

        if (post_visit)
            post_visit(vertex);
    }
}

} // namespace gl
