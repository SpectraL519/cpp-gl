#pragma once

#include "gl/algorithm/callbacks.hpp"
#include "gl/graph/graph_traits.hpp"


namespace gl::detail {

template <gl_graph_c Graph>
void depth_first_search_recursive_unit(
    const std::unique_ptr<Graph>& graph,
    const typename Graph::vertex_ptr_type& vertex, std::vector<bool>& visited,
    vertex_ptr_type_callback<Graph> pre_visit, vertex_ptr_type_callback<Graph> post_visit
) {
    // TODO (*): account for mutable_graph (some keys can be missing)

    if (visited.at(vertex->key))  // TODO: (*)
        return;

    if (pre_visit)
        pre_visit(vertex);

    visited.at(vertex->key) = true;  // TODO: (*)
    for (auto& edge : vertex->adjacent()) {
        if (visited.at(edge->destination))
            continue;

        depth_first_search_recursive_unit(
            graph, graph->at(edge->destination), visited, pre_visit, post_visit
        );  // TODO: (*)
    }

    if (post_visit)
        post_visit(vertex);
}

}  // namespace gl::detail
