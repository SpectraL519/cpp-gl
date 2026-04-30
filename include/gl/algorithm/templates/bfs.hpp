// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/templates/bfs.hpp
/// @brief Generic Breadth-First Search (BFS) template algorithm engine.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"

#include <queue>

namespace gl::algorithm {

/// @ingroup GL-Algorithm
/// @brief A highly customizable, generic Breadth-First Search (BFS) algorithm engine.
///
/// This template does not implement a specific algorithm (like finding a shortest path).
/// Instead, it provides the strict structural execution of a queue-based Breadth-First Search.
/// Concrete algorithms are constructed by injecting logic into the provided callback and
/// predicate hooks.
///
/// ### Example Usage
/// ```cpp
/// std::vector<bool> visited(graph.n_vertices(), false); // (1)!
///
/// bool completed = gl::algorithm::bfs(
///     graph,
///     gl::algorithm::init_node_range<graph_type>(start_id), // (2)!
///     gl::algorithm::default_visit_vertex_predicate(visited), // (3)!
///     [&](auto v, auto p) { // (4)!
///         std::cout << "Visited vertex " << v << '\n';
///         return true; // Continue search
///     },
///     gl::algorithm::default_enqueue_node_predicate<graph_type, true>(visited) // (5)!
/// );
/// ```
///
/// 1\. Tracks discovered vertices.
///
/// 2\. Initializes the search queue with the starting vertex.
///
/// 3\. Predicate ensuring we don't process a vertex if it was already marked visited.
///
/// 4\. The main visit callback. Here we just print the ID. Returning `false` would abort the search.
///
/// 5\. Predicate ensuring we only enqueue adjacent vertices that haven't been visited yet, returning a @ref gl::algorithm::decision "decision".
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | InitQueueRangeType | The type of the container providing the initial roots to enqueue. | Must be a *forward range* of @ref gl::algorithm::search_node "search nodes". |
/// | VisitVertexPredicate | Type of the callable deciding if a popped vertex should be processed. | Must be one of:<br/>- An `(id_type) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Type of the callable executed when a vertex is officially visited. | Must be one of:<br/>- An `(id_type, id_type) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueueNodePred | Type of the callable deciding if a node corresponding to an adjacent vertex should be pushed to the queue. | Must be one of:<br/>- An `(id_type, const edge_type&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. | Must be one of:<br/>- An `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges are evaluated. | Must be one of:<br/>- An `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param initial_queue_content A range of initial @ref gl::algorithm::search_node "search nodes" to seed the BFS queue.
/// @param visit_vertex_pred Predicate evaluated immediately after popping a vertex. If it returns `false`, the vertex is skipped.
/// @param visit Callback invoked when a vertex is officially visited. If it returns `false`, the entire BFS immediately aborts.
/// @param enqueue_node_pred Predicate evaluated for each outgoing edge. Returns a @ref gl::algorithm::decision "decision":
/// - `accept` to enqueue,
/// - `reject` to skip,
/// - `abort` to terminate the BFS entirely.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been evaluated.
/// @return `true` if the queue was exhausted naturally, `false` if the search was aborted early by a callback or predicate.
/// @hideparams
template <
    traits::c_graph G,
    traits::c_forward_range_of<search_node<G>> InitQueueRangeType = std::vector<search_node<G>>,
    traits::c_optional_predicate<typename G::id_type> VisitVertexPredicate = empty_callback,
    traits::c_optional_predicate<typename G::id_type, typename G::id_type> VisitCallback =
        empty_callback,
    traits::c_decision_predicate<typename G::id_type, const typename G::edge_type&>
        EnqueueNodePred = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
bool bfs(
    const G& graph,
    const InitQueueRangeType& initial_queue_content,
    VisitVertexPredicate visit_vertex_pred = {},
    VisitCallback visit = {},
    EnqueueNodePred enqueue_node_pred = {},
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    if (std::ranges::empty(initial_queue_content))
        return false;

    // prepare the node queue
    std::queue<search_node<G>> q;
    for (const auto& node : initial_queue_content)
        q.push(node);

    // search the graph
    while (not q.empty()) {
        const search_node node = q.front();
        q.pop();

        if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(node.vertex_id))
                continue;

        if constexpr (not traits::c_empty_callback<PreVisitCallback>)
            pre_visit(node.vertex_id);

        if constexpr (not traits::c_empty_callback<VisitCallback>)
            if (not visit(node.vertex_id, node.pred_id))
                return false;

        for (const auto& edge : graph.out_edges(node.vertex_id)) {
            const auto target_vertex_id = edge.other(node.vertex_id);
            const auto enqueue = enqueue_node_pred(target_vertex_id, edge);
            if (enqueue == decision::abort)
                return false;
            if (enqueue)
                q.emplace(target_vertex_id, node.vertex_id);
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(node.vertex_id);
    }

    return true;
}

} // namespace gl::algorithm
