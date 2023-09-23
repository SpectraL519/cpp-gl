#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "gl/graph/graph_traits.hpp"
#include "gl/algorithm/callbacks.hpp"



namespace gl {

// TODO:
// * remove at() method from igraph and mutable_graph
// * use get_vertex() instead of at() in dfs algorithms

template <gl_graph_t graph_type>
void breadth_first_search(
    const std::unique_ptr<graph_type>& graph,
    vertex_ptr_callback<graph_type> pre_visit = {},
    vertex_ptr_callback<graph_type> post_visit = {}
) {
    // TODO (*): account for mutable_graph (some keys can be missing)

    using key_type = typename graph_type::vertex_key_type;

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
