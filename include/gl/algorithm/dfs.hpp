#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "gl/graph/graph_traits.hpp"



namespace gl {

// TODO:
// template <gl_graph_t graph_type>
// void depth_first_search(std::unique_ptr<graph_t> graph) {

template <
    directed_specifier directed_v,
    vertex_descriptor_t vertex_t,
    graph_container_t container_t
>
void depth_first_search(std::unique_ptr<igraph<directed_v, vertex_t, container_t>> graph) {
    // TODO: add pre and post functions

    using key_type = igraph<directed_v, vertex_t, container_t>::key_type;

    auto num_vertices = graph->num_vertices();
    std::vector<bool> visited(num_vertices, false); // TODO: (*) account for mutable_graph (some keys can be missing)
    std::stack<key_type> key_stack;

    for (auto& vertex : graph->vertices()) {
        if (visited.at(vertex->key)) // TODO: (*)
            continue;

        while (not key_stack.empty()) {
            visited.at(vertex->key) = true; // TODO: (*)

            for (auto& edge : vertex->adjacent()) {
                if (visited.at(edge.destination))
                    continue;

                key_stack.push(edge.destination);
            }
        }
    }
}

} // namespace gl
