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
///     std::array{gl::algorithm::root_node<G>(start_vertex_id)}, // (2)!
///     gl::algorithm::default_visit_predicate<G>(visited), // (3)!
///     [&](const auto& node) { // (4)!
///         std::cout << "Visited vertex " << node.vertex_id << '\n';
///         return true; // Continue search
///     },
///     gl::algorithm::default_enqueue_node_predicate<graph_type, true>(visited) // (5)!
/// );
/// ```
///
/// 1\. Tracks discovered vertices.
///
/// 2\. Initializes the search queue with the starting search node.
///
/// 3\. Predicate ensuring we don't process a node if its vertex was already marked visited.
///
/// 4\. The main visit callback receiving the full search node. Returning `false` would abort the search.
///
/// 5\. Predicate evaluating newly constructed target nodes to ensure we only enqueue unvisited vertices, returning a @ref gl::algorithm::decision "decision".
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | InitNodeRngType | The type of the container providing the initial roots to enqueue. | Must be a *forward range* of @ref gl::algorithm::search_node "search nodes". |
/// | VisitPredicate | Type of the callable deciding if a popped node should be processed. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Type of the callable executed when a node is officially visited. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueuePredicate | Type of the callable deciding if a target node should be pushed to the queue. | Must be one of:<br/>- A `(search_node<val_t<G>>, const edge_t<G>&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges are evaluated. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param initial_nodes A range of initial @ref gl::algorithm::search_node "search nodes" to seed the BFS queue.
/// @param visit_pred Predicate evaluated immediately after popping a node. If it returns `false`, the node is skipped.
/// @param visit Callback invoked when a node is officially visited. If it returns `false`, the entire BFS immediately aborts.
/// @param enqueue_pred Predicate evaluated for each outgoing edge and target node. Returns a @ref gl::algorithm::decision "decision":
/// - `accept` to enqueue,
/// - `reject` to skip,
/// - `abort` to terminate the BFS entirely.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been evaluated.
/// @return `true` if the queue was exhausted naturally, `false` if the search was aborted early by a callback or predicate.
/// @hideparams
template <
    traits::c_graph G,
    traits::c_forward_range_of<search_node<val_t<G>>> InitNodeRngType =
        std::vector<search_node<val_t<G>>>,
    traits::c_optional_predicate<search_node<val_t<G>>> VisitPredicate = empty_callback,
    traits::c_optional_predicate<search_node<val_t<G>>> VisitCallback = empty_callback,
    traits::c_decision_predicate<search_node<val_t<G>>, const edge_t<G>&> EnqueuePredicate =
        empty_callback,
    traits::c_optional_callback<void, search_node<val_t<G>>> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, search_node<val_t<G>>> PostVisitCallback = empty_callback>
bool bfs(
    G&& graph,
    const InitNodeRngType& initial_nodes,
    const VisitPredicate& visit_pred = {},
    const VisitCallback& visit = {},
    const EnqueuePredicate& enqueue_pred = {},
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (std::ranges::empty(initial_nodes))
        return false;

    // prepare the node queue
    std::queue<search_node<val_t<G>>> q;
    for (const auto& node : initial_nodes)
        q.push(node);

    // search the graph
    while (not q.empty()) {
        const auto curr_node = q.front();
        q.pop();

        if constexpr (not traits::c_empty_callback<VisitPredicate>)
            if (not visit_pred(curr_node))
                continue;

        if constexpr (not traits::c_empty_callback<PreVisitCallback>)
            pre_visit(curr_node);

        if constexpr (not traits::c_empty_callback<VisitCallback>)
            if (not visit(curr_node))
                return false;

        for (const auto& edge : graph.out_edges(curr_node.vertex_id)) {
            search_node<val_t<G>> tgt_node{edge.other(curr_node.vertex_id), curr_node.vertex_id};
            const auto enqueue = enqueue_pred(tgt_node, edge);

            if (enqueue == decision::abort)
                return false;
            if (enqueue)
                q.push(tgt_node);
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(curr_node);
    }

    return true;
}

} // namespace gl::algorithm
