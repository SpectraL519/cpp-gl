#pragma once

#include <memory>
#include <stack>
#include <vector>

#include "gl/graph/graph_traits.hpp"
#include "gl/algorithm/callbacks.hpp"



namespace gl {

// TODO:
// * remove at() method from igraph and mutable_graph
// * use get_vertex() instead of at() in dfs algorithms

template <gl_graph_c graph_type>
void depth_first_search(
    const std::unique_ptr<graph_type>& graph,
    vertex_ptr_type_callback<graph_type> pre_visit = {},
    vertex_ptr_type_callback<graph_type> post_visit = {}
) {
    // TODO (*): account for mutable_graph (some keys can be missing)

    using key_type = typename graph_type::vertex_key_type;

    const auto num_vertices = graph->num_vertices();
    std::vector<bool> visited(num_vertices, false); // TODO: (*)
    std::stack<key_type> key_stack;

    for (auto& vertex : graph->vertices()) {
        if (visited.at(vertex->key)) // TODO: (*)
            continue;

        if (pre_visit)
            pre_visit(vertex);

        while (not key_stack.empty()) {
            visited.at(vertex->key) = true; // TODO: (*)

            for (auto& edge : vertex->adjacent()) {
                if (visited.at(edge->destination))
                    continue;

                key_stack.push(edge->destination);
            }
        }

        if (post_visit)
            post_visit(vertex);
    }
}



namespace detail {

template <gl_graph_c graph_type>
void depth_first_search_recursive_unit(
    const std::unique_ptr<graph_type>& graph,
    const typename graph_type::vertex_ptr_type& vertex,
    std::vector<bool>& visited,
    vertex_ptr_type_callback<graph_type> pre_visit,
    vertex_ptr_type_callback<graph_type> post_visit
) {
    if (visited.at(vertex->key)) // TODO: (*)
        return;

    if (pre_visit)
        pre_visit(vertex);

    visited.at(vertex->key) = true; // TODO: (*)
    for (auto& edge : vertex->adjacent()) {
        if (visited.at(edge->destination))
            continue;

        depth_first_search_recursive_unit(
            graph, graph->at(edge->destination), visited,
            pre_visit, post_visit
        ); // TODO: (*)
    }

    if (post_visit)
        post_visit(vertex);
}

} // namespace detail

template <gl_graph_c graph_type>
void depth_first_search_recursive(
    const std::unique_ptr<graph_type>& graph,
    vertex_ptr_type_callback<graph_type> pre_visit = {},
    vertex_ptr_type_callback<graph_type> post_visit = {}
) {
    // TODO (*): account for mutable_graph (some keys can be missing)

    const auto num_vertices = graph->num_vertices();
    std::vector<bool> visited(num_vertices, false); // TODO: (*)

    for (auto& vertex : graph->vertices())
        detail::depth_first_search_recursive_unit(
            graph, vertex, visited, pre_visit, post_visit
        );
}

} // namespace gl
