// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/templates/dfs.hpp
/// @brief Generic Depth-First Search (DFS) template algorithm engines (iterative and recursive).

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"

#include <stack>

namespace gl::algorithm {

/// @ingroup GL-Algorithm
/// @brief A highly customizable, generic iterative Depth-First Search (DFS) algorithm engine.
///
/// This engine provides the strict structural execution of a stack-based Depth-First Search.
/// Concrete algorithms (like cycle detection or topological sorting) are built by injecting
/// specific logic into the provided callback hooks.
///
/// > [!NOTE] True Post-Order Traversal
/// > If a non-empty `PostVisitCallback` is provided, this engine automatically utilizes a stateful
/// > stack frame to guarantee a true post-order traversal (the callback fires only after the entire
/// > subtree of a node has been fully explored). If the callback is omitted (using `empty_callback`),
/// > the engine bypasses frame tracking entirely for maximum performance.
///
/// ### Example Usage
/// ```cpp
/// std::vector<bool> visited(graph.n_vertices(), false); // (1)!
///
/// bool completed = gl::algorithm::dfs(
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
/// 2\. Initializes the search stack with the starting search node.
///
/// 3\. Predicate ensuring we don't process a node if its vertex was already marked visited.
///
/// 4\. The main visit callback receiving the full search node. Returning `false` would abort the search.
///
/// 5\. Predicate evaluating newly constructed target nodes to ensure we only push unvisited vertices to the stack, returning a @ref gl::algorithm::decision "decision".
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | InitNodeRngType | The type of the container providing the initial roots to push to the stack. | Must be a *forward range* of @ref gl::algorithm::search_node "search nodes". |
/// | VisitPredicate | Type of the callable deciding if a popped node should be processed. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Type of the callable executed when a node is officially visited. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueuePredicate | Type of the callable deciding if a target node should be pushed to the stack. | Must be one of:<br/>- A `(search_node<val_t<G>>, const edge_t<G>&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after a node's subtree is fully explored. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param initial_nodes A range of initial @ref gl::algorithm::search_node "search nodes" to seed the DFS stack.
/// @param visit_pred Predicate evaluated immediately after popping a node. If it returns `false`, the node is skipped.
/// @param visit Callback invoked when a node is officially visited. If it returns `false`, the entire DFS immediately aborts.
/// @param enqueue_pred Predicate evaluated for each outgoing edge and target node. Returns a @ref gl::algorithm::decision "decision":
/// - `accept` to enqueue,
/// - `reject` to skip,
/// - `abort` to terminate the DFS entirely.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after the current vertex's children have been exhaustively processed (true post-order).
/// @return `true` if the stack was exhausted naturally, `false` if the search was aborted early.
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
bool dfs(
    G&& graph,
    const InitNodeRngType& initial_nodes,
    const VisitPredicate& visit_pred = {},
    const VisitCallback& visit = {},
    const EnqueuePredicate& enqueue_pred = {},
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using stateless_node_t = search_node<val_t<G>>;

    if (std::ranges::empty(initial_nodes))
        return false;

    if constexpr (traits::c_empty_callback<PostVisitCallback>) { // stateless stack
        std::stack<stateless_node_t> s;
        for (const auto& node : initial_nodes)
            s.push(node);

        while (not s.empty()) {
            const stateless_node_t curr_node = s.top();
            s.pop();

            if constexpr (not traits::c_empty_callback<VisitPredicate>)
                if (not visit_pred(curr_node))
                    continue;

            if constexpr (not traits::c_empty_callback<PreVisitCallback>)
                pre_visit(curr_node);

            if constexpr (not traits::c_empty_callback<VisitCallback>)
                if (not visit(curr_node))
                    return false;

            for (const auto& edge : graph.out_edges(curr_node.vertex_id)) {
                stateless_node_t tgt_node{edge.other(curr_node.vertex_id), curr_node.vertex_id};
                const auto enqueue = enqueue_pred(tgt_node, edge);

                if (enqueue == decision::abort)
                    return false;
                if (enqueue)
                    s.push(tgt_node);
            }
        }
    }
    else { // stateful stack

        struct dfs_ext {
            bool expanded = false; // Indicates if all of the node's children have been visited
        };

        using stateful_node_t = search_node<val_t<G>, dfs_ext>;
        std::stack<stateful_node_t> s;

        for (const auto& node : initial_nodes)
            s.push(stateful_node_t(node)); // Initialize as unexpanded

        while (not s.empty()) {
            auto curr_node = s.top();
            s.pop();

            // Reconstruct the stateless base node to safely satisfy the callback concepts
            const stateless_node_t base_node(curr_node);

            if (curr_node.ext.expanded) {
                post_visit(base_node);
            }
            else {
                if constexpr (not traits::c_empty_callback<VisitPredicate>)
                    if (not visit_pred(base_node))
                        continue;

                if constexpr (not traits::c_empty_callback<PreVisitCallback>)
                    pre_visit(base_node);

                if constexpr (not traits::c_empty_callback<VisitCallback>)
                    if (not visit(base_node))
                        return false;

                // Push parent back marked as expanded to wait for children
                curr_node.ext.expanded = true;
                s.push(curr_node);

                for (const auto& edge : graph.out_edges(curr_node.vertex_id)) {
                    stateless_node_t tgt_base{edge.other(curr_node.vertex_id), curr_node.vertex_id};
                    const auto enqueue = enqueue_pred(tgt_base, edge);

                    if (enqueue == decision::abort)
                        return false;
                    if (enqueue)
                        s.push(stateful_node_t(tgt_base));
                }
            }
        }
    }

    return true;
}

/// @ingroup GL-Algorithm
/// @brief A highly customizable, generic recursive Depth-First Search (DFS) algorithm engine.
///
/// This engine mirrors the iterative `dfs` behavior but utilizes the C++ call stack.
/// It does not accept an initial range, but instead is kicked off for a specific root node.
/// It does not return a boolean abort signal; logic flow must be managed by the injected callbacks.
///
/// ### Example Usage
/// ```cpp
/// std::vector<bool> visited(graph.n_vertices(), false); // (1)!
///
/// gl::algorithm::r_dfs(
///     graph,
///     gl::algorithm::root_node<G>(start_id), // (2)!
///     gl::algorithm::default_visit_predicate<G>(visited), // (3)!
///     [&](const auto& node) { // (4)!
///         std::cout << "Recursively visiting vertex " << node.vertex_id << '\n';
///         return true;
///     },
///     gl::algorithm::default_enqueue_node_predicate<graph_type, false>(visited) // (5)!
/// );
/// ```
///
/// 1\. Tracks discovered vertices.
///
/// 2\. A fully initialized root search node to begin the recursion.
///
/// 3\. Predicate evaluated upon entering the recursive call to prevent duplicate processing.
///
/// 4\. The main visit callback.
///
/// 5\. Predicate evaluating whether to recursively traverse into the target node.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | VisitPredicate | Type of the callable deciding if the current node should be processed. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Type of the callable executed when the node is officially visited. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueuePredicate | Type of the callable deciding if a target node should be recursed into. | Must be one of:<br/>- A `(search_node<val_t<G>>, const edge_t<G>&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges are evaluated. | Must be one of:<br/>- A `(search_node<val_t<G>>) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param curr_node The active @ref gl::algorithm::search_node "search node" currently being evaluated.
/// @param visit_pred Predicate evaluated immediately upon entry. If it returns `false`, recursion returns early.
/// @param visit Callback invoked when a node is officially visited.
/// @param enqueue_pred Predicate evaluated for each outgoing edge and target node. If `true`, the target is recursed into.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after returning from all adjacent recursive calls.
/// @hideparams
template <
    traits::c_graph G,
    traits::c_optional_predicate<search_node<val_t<G>>> VisitPredicate,
    traits::c_optional_predicate<search_node<val_t<G>>> VisitCallback,
    traits::c_decision_predicate<search_node<val_t<G>>, const edge_t<G>&> EnqueuePredicate,
    traits::c_optional_callback<void, search_node<val_t<G>>> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, search_node<val_t<G>>> PostVisitCallback = empty_callback>
void r_dfs(
    G&& graph,
    search_node<val_t<G>> curr_node,
    VisitPredicate visit_pred,
    VisitCallback visit,
    EnqueuePredicate enqueue_pred,
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    if constexpr (not traits::c_empty_callback<VisitPredicate>)
        if (not visit_pred(curr_node))
            return;

    if constexpr (not traits::c_empty_callback<PreVisitCallback>)
        pre_visit(curr_node);

    visit(curr_node);

    // recursively search vertices adjacent to the current vertex
    for (const auto& edge : graph.out_edges(curr_node.vertex_id)) {
        search_node<val_t<G>> tgt_node{edge.other(curr_node.vertex_id), curr_node.vertex_id};
        if (enqueue_pred(tgt_node, edge))
            r_dfs(graph, tgt_node, visit_pred, visit, enqueue_pred, pre_visit, post_visit);
    }

    if constexpr (not traits::c_empty_callback<PostVisitCallback>)
        post_visit(curr_node);
}

} // namespace gl::algorithm
