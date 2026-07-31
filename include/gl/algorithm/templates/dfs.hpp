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
/// > If a `PostVisitCallback` is provided, this engine automatically utilizes a stateful stack
/// > frame to guarantee a true post-order traversal (the callback fires only after the entire
/// > subtree of a node has been fully explored). If the callback is omitted (using `empty_callback`),
/// > the engine bypasses frame tracking entirely for maximum performance.
///
/// ### Example Usage
/// ```cpp
/// std::vector<bool> visited(graph.n_vertices(), false); // (1)!
///
/// bool completed = gl::algorithm::dfs(
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
/// 2\. Initializes the search stack with the starting vertex.
///
/// 3\. Predicate ensuring we don't process a vertex if it was already marked visited.
///
/// 4\. The main visit callback. Returning `false` would abort the search.
///
/// 5\. Predicate ensuring we only push adjacent, unvisited vertices to the stack, returning a @ref gl::algorithm::decision "decision".
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | InitStackRangeType | The type of the container providing the initial roots to push to the stack. | Must be a *forward range* of @ref gl::algorithm::search_node "search nodes". |
/// | VisitVertexPredicate | Type of the callable deciding if a popped vertex should be processed. | Must be one of:<br/>- An `(id_type) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Type of the callable executed when a vertex is officially visited. | Must be one of:<br/>- An `(id_type, id_type) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueueNodePred | Type of the callable deciding if a node corresponding to an adjacent vertex should be pushed to the stack. | Must be one of:<br/>- An `(id_type, const edge_type&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. | Must be one of:<br/>- An `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after a node's subtree is fully explored. | Must be one of:<br/>- An `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param initial_stack_content A range of initial @ref gl::algorithm::search_node "search nodes" to seed the DFS stack.
/// @param visit_vertex_pred Predicate evaluated immediately after popping a vertex. If it returns `false`, the vertex is skipped.
/// @param visit Callback invoked when a vertex is officially visited. If it returns `false`, the entire DFS immediately aborts.
/// @param enqueue_node_pred Predicate evaluated for each outgoing edge. Returns a @ref gl::algorithm::decision "decision":
/// - `accept` to enqueue,
/// - `reject` to skip,
/// - `abort` to terminate the DFS entirely.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after the current vertex's children have been exhaustively processed (true post-order).
/// @return `true` if the stack was exhausted naturally, `false` if the search was aborted early.
/// @hideparams
template <
    traits::c_graph G,
    traits::c_forward_range_of<search_node<val_t<G>>> InitStackRangeType =
        std::vector<search_node<val_t<G>>>,
    traits::c_optional_predicate<id_t<G>> VisitVertexPredicate = empty_callback,
    traits::c_optional_predicate<id_t<G>, id_t<G>> VisitCallback = empty_callback,
    traits::c_decision_predicate<id_t<G>, const edge_t<G>&> EnqueueNodePred = empty_callback,
    traits::c_optional_callback<void, id_t<G>> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, id_t<G>> PostVisitCallback = empty_callback>
bool dfs(
    G&& graph,
    const InitStackRangeType& initial_stack_content,
    VisitVertexPredicate visit_vertex_pred = {},
    VisitCallback visit = {},
    EnqueueNodePred enqueue_node_pred = {},
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    if (std::ranges::empty(initial_stack_content))
        return false;

    if constexpr (traits::c_empty_callback<PostVisitCallback>) { // stateless stack
        std::stack<search_node<val_t<G>>> s;
        for (const auto& node : initial_stack_content)
            s.push(node);

        while (not s.empty()) {
            const auto node = s.top();
            s.pop();

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
                    s.emplace(target_vertex_id, node.vertex_id);
            }
        }
    }
    else { // statefull stack

        struct dfs_extension {
            bool expanded = false; // Indicated if all of the node's children have been visited
        };

        using stateful_node_t = search_node<val_t<G>, dfs_extension>;
        std::stack<stateful_node_t> s;

        for (const auto& node : initial_stack_content)
            s.emplace(node.vertex_id, node.pred_id); // Initialize as unexpanded

        while (not s.empty()) {
            auto curr_node = s.top();
            s.pop();

            if (curr_node.ext.expanded) {
                post_visit(curr_node.vertex_id);
            }
            else {
                if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
                    if (not visit_vertex_pred(curr_node.vertex_id))
                        continue;

                if constexpr (not traits::c_empty_callback<PreVisitCallback>)
                    pre_visit(curr_node.vertex_id);

                if constexpr (not traits::c_empty_callback<VisitCallback>)
                    if (not visit(curr_node.vertex_id, curr_node.pred_id))
                        return false;

                // Push parent back marked as expanded to wait for children
                curr_node.ext.expanded = true;
                s.push(curr_node);

                for (const auto& edge : graph.out_edges(curr_node.vertex_id)) {
                    const auto target_vertex_id = edge.other(curr_node.vertex_id);
                    const auto enqueue = enqueue_node_pred(target_vertex_id, edge);
                    if (enqueue == decision::abort)
                        return false;
                    if (enqueue)
                        s.emplace(target_vertex_id, curr_node.vertex_id);
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
/// It does not accept an initial range, but instead is kicked off for a specific root vertex.
/// It does not return a boolean abort signal; logic flow must be managed by the injected callbacks.
///
/// ### Example Usage
/// ```cpp
/// std::vector<bool> visited(graph.n_vertices(), false); // (1)!
///
/// gl::algorithm::r_dfs(
///     graph,
///     start_id, // (2)!
///     gl::algorithm::no_root, // (3)!
///     gl::algorithm::default_visit_vertex_predicate(visited), // (4)!
///     [&](auto v, auto p) { // (5)!
///         std::cout << "Recursively visiting vertex " << v << '\n';
///         return true;
///     },
///     gl::algorithm::default_enqueue_node_predicate<graph_type, false>(visited) // (6)!
/// );
/// ```
///
/// 1\. Tracks discovered vertices.
///
/// 2\. The ID of the starting vertex for the recursion.
///
/// 3\. Indicates that the starting vertex has no predecessor.
///
/// 4\. Predicate evaluated upon entering the recursive call to prevent duplicate processing.
///
/// 5\. The main visit callback.
///
/// 6\. Predicate evaluating whether to recursively traverse into the target vertex.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | VisitVertexPredicate | Type of the callable deciding if the current vertex should be processed. | Must be one of:<br/>- An `(id_type) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Type of the callable executed when the vertex is officially visited. | Must be one of:<br/>- An `(id_type, id_type) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueueNodePred | Type of the callable deciding if a node corresponding to an adjacent vertex should be recursed into. | Must be one of:<br/>- An `(id_type, const edge_type&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. | Must be one of:<br/>- An `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges are evaluated. | Must be one of:<br/>- An `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param vertex_id The ID of the vertex currently being visited.
/// @param pred_id The ID of the predecessor vertex.
/// @param visit_vertex_pred Predicate evaluated immediately upon entry. If it returns `false`, recursion returns early.
/// @param visit Callback invoked when a vertex is officially visited.
/// @param enqueue_node_pred Predicate evaluated for each outgoing edge. If `true`, the target is recursed into.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after returning from all adjacent recursive calls.
/// @hideparams
template <
    traits::c_graph G,
    traits::c_optional_predicate<id_t<G>> VisitVertexPredicate,
    traits::c_optional_predicate<id_t<G>, id_t<G>> VisitCallback,
    traits::c_decision_predicate<id_t<G>, const edge_t<G>&> EnqueueNodePred,
    traits::c_optional_callback<void, id_t<G>> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, id_t<G>> PostVisitCallback = empty_callback>
void r_dfs(
    G&& graph,
    const id_t<G> vertex_id,
    const id_t<G> pred_id,
    VisitVertexPredicate visit_vertex_pred,
    VisitCallback visit,
    EnqueueNodePred enqueue_node_pred,
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
        if (not visit_vertex_pred(vertex_id))
            return;

    if constexpr (not traits::c_empty_callback<PreVisitCallback>)
        pre_visit(vertex_id);

    visit(vertex_id, pred_id);

    // recursively search vertices adjacent to the current vertex
    for (const auto& edge : graph.out_edges(vertex_id)) {
        const auto target_vertex_id = edge.other(vertex_id);
        if (enqueue_node_pred(target_vertex_id, edge))
            r_dfs(
                graph,
                target_vertex_id,
                vertex_id,
                visit_vertex_pred,
                visit,
                enqueue_node_pred,
                pre_visit,
                post_visit
            );
    }

    if constexpr (not traits::c_empty_callback<PostVisitCallback>)
        post_visit(vertex_id);
}

} // namespace gl::algorithm
