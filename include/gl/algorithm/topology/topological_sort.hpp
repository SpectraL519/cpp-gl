// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/topology/topological_sort.hpp
/// @brief Algorithms for computing the topological ordering of directed acyclic graphs.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/bfs.hpp"

namespace gl::algorithm {

/// @ingroup GL GL-Algorithm
/// @brief Computes a topological ordering of the vertices in a Directed Acyclic Graph (DAG).
///
/// This implementation relies on Kahn's Algorithm. It utilizes the generic @ref gl::algorithm::bfs "bfs"
/// template, seeding the queue with all vertices that have an in-degree of 0. As vertices are processed,
/// the in-degrees of adjacent vertices are iteratively decremented.
///
/// If the final sorted order does not contain all vertices in the graph, it indicates the presence
/// of a cycle, meaning the graph is not a DAG.
///
/// ### Example Usage
/// ```cpp
/// if (auto top_order = gl::algorithm::topological_sort(graph)) { // (1)!
///     std::cout << "Topological Order: "
///               << gl::io::range_formatter(top_order.value()) // (2)!
///               << '\n';
/// }
/// else {
///     std::cout << "Graph contains a cycle!\n";
/// }
/// ```
///
/// 1\. Attempts to compute the ordering. Fails and returns `std::nullopt` if a cycle is detected.
///
/// 2\. Prints the topologically sorted vector of vertex IDs using the @ref gl::io::range_formatter "range_formatter" helper.
///
/// > [!INFO] Algorithmic Complexity
/// >
/// > The time complexity depends entirely on the underlying representation of `GraphType`:
/// > - **Adjacency List Representations**: \f$O(|V| + |E|)\f$
/// >   - *Includes:* @ref gl::impl::list_t "list_t" and @ref gl::impl::flat_list_t "flat_list_t".
/// > - **Adjacency Matrix Representations**: \f$O(|V|^2)\f$
/// >   - *Includes:* @ref gl::impl::matrix_t "matrix_t" and @ref gl::impl::flat_matrix_t "flat_matrix_t".
/// >   - *Note:* Iterating over adjacent vertices requires scanning the entire \f$|V|\f$-length matrix row.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the directed graph being traversed. | Must satisfy the [**c_directed_graph**](gl_concepts.md#gl-traits-c-directed-graph) concept. |
/// | PreVisitCallback | Type of the callable executed immediately before a vertex is pushed into the sort order. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges of a vertex are evaluated. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The directed graph to evaluate.
/// @param pre_visit Hook executed immediately before the internal sort logic processes a vertex.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been evaluated and their in-degrees decremented.
/// @return An `std::optional` containing a vector of vertex IDs in topological order or `std::nullopt` if the graph is not a DAG.
/// @hideparams
template <
    traits::c_directed_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
[[nodiscard]] std::optional<std::vector<typename G::id_type>> topological_sort(
    const G& graph, PreVisitCallback pre_visit = {}, PostVisitCallback post_visit = {}
) {
    using id_type = typename G::id_type;
    using edge_type = typename G::edge_type;

    // prepare the vertex in degree map
    std::vector<size_type> in_degree_map = graph.in_degree_map();

    // prepare the initial queue content (source vertices)
    std::vector<search_node<G>> source_vertex_list;
    source_vertex_list.reserve(graph.n_vertices());
    for (const auto id : graph.vertex_ids())
        if (in_degree_map[to_idx(id)] == 0uz)
            source_vertex_list.emplace_back(id);

    std::vector<id_type> topological_order{};
    topological_order.reserve(graph.n_vertices());

    bfs(
        graph,
        source_vertex_list,
        empty_callback{}, // visit predicate
        [&topological_order](id_type vertex_id, id_type) { // visit callback
            topological_order.push_back(vertex_id);
            return true;
        },
        [&in_degree_map](id_type vertex_id, const edge_type& in_edge)
            -> decision { // enqueue predicate
            if (in_edge.is_loop())
                return false;
            return --in_degree_map[to_idx(vertex_id)] == 0uz;
        },
        pre_visit,
        post_visit
    );

    if (topological_order.size() != graph.n_vertices())
        return std::nullopt;

    return topological_order;
}

} // namespace gl::algorithm
