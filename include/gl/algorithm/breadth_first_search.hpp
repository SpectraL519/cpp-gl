#pragma once

#include "gl/graph.hpp"
#include "types.hpp"

#include <iostream>
#include <queue>

namespace gl::algorithm {

namespace detail {

template <type_traits::c_alg_return_graph SearchTreeType, type_traits::c_directed_graph GraphType>
[[nodiscard]] gl_attr_force_inline SearchTreeType init_search_tree(const GraphType& graph) {
    if constexpr (type_traits::is_alg_no_return_type_v<SearchTreeType>)
        return SearchTreeType{};
    else
        return SearchTreeType(graph.n_vertices());
}

template <type_traits::c_alg_return_graph SearchTreeType, type_traits::c_directed_graph GraphType>
void visit(
    const typename GraphType::vertex_type& vertex,
    const types::id_type source_id,
    visited_list& visited,
    SearchTreeType& search_tree
) {
    const auto vertex_id = vertex.id();

    std::cout << "visiting: " << vertex_id << std::endl;
    visited[vertex_id] = true;

    if constexpr (not type_traits::is_alg_no_return_type_v<SearchTreeType>) {
        if (source_id != vertex_id)
            search_tree.add_edge(source_id, vertex_id);
    }
}

} // namespace detail

template <type_traits::c_alg_return_graph SearchTreeType, type_traits::c_graph GraphType>
type_traits::alg_return_type<SearchTreeType> breadth_first_search(const GraphType& graph) {
    using vertex_type = typename GraphType::vertex_type;

    struct bfs_qvertex {
        types::id_type id;
        types::id_type source_id;
        // if source_id == vertex.id() then vertex_id is the id of the starting vertex
    };

    using vertex_queue_type = std::queue<bfs_qvertex>;
    vertex_queue_type vertex_queue;

    visited_list visited(graph.n_vertices(), false);
    auto search_tree = detail::init_search_tree<SearchTreeType>(graph);

    for (const auto& vertex : graph.vertices()) {
        const auto vertex_id = vertex.id();
        if (visited[vertex_id])
            continue;

        vertex_queue.push(bfs_qvertex{vertex_id, vertex_id});

        while (not vertex_queue.empty()) {
            bfs_qvertex qv = vertex_queue.front();
            vertex_queue.pop();

            if (visited[qv.id])
                continue;

            const auto& vertex = graph.get_vertex(qv.id);
            detail::visit<SearchTreeType, GraphType>(vertex, qv.source_id, visited, search_tree);

            for (const auto& edge : graph.adjacent_edges(qv.id)) {
                const auto adjacent_id = edge.incident_vertex(vertex).id();
                if (not visited[adjacent_id])
                    vertex_queue.push(bfs_qvertex{adjacent_id, qv.id});
            }
        }
    }
}

} // namespace gl::algorithm
