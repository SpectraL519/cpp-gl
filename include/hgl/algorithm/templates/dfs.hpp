// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/algorithm/templates/dfs.hpp
/// @brief Generic Depth-First Search (DFS) template algorithm engine for hypergraphs.

#pragma once

#include "hgl/algorithm/core.hpp"

#include <stack>

namespace hgl::algorithm {

/// @ingroup HGL-Algorithm
/// @brief A highly customizable, generic Depth-First Search (DFS) algorithm engine for hypergraphs.
///
/// This template provides the strict structural execution of a stack-based Depth-First Search
/// over a hypergraph's topology. Because a hypergraph traversal inherently requires a two-step
/// expansion (from a vertex to its incident hyperedges, and then to the adjacent vertices), this
/// engine exposes specific hooks for both steps. Concrete algorithms are constructed by injecting
/// logic into the provided callback and predicate hooks.
///
/// ### Example Usage
/// ```cpp
/// std::vector<bool> visited(hypergraph.n_vertices(), false); // (1)!
///
/// bool completed = hgl::algorithm::dfs(
///     hypergraph,
///     hgl::algorithm::init_node_range<H>(start_id), // (2)!
///     [&](const auto& node) { return not visited[node.vertex_id]; }, // (3)!
///     [&](const auto& node) { // (4)!
///         visited[node.vertex_id] = true;
///         std::cout << "Visited vertex " << node.vertex_id << '\n';
///         return true; // Continue search
///     },
///     hgl::empty_callback{}, // (5)!
///     [&](const auto& tgt_node) { return not visited[tgt_node.vertex_id]; } // (6)!
/// );
/// ```
///
/// 1\. Provide an external state array to track the visited vertices.
///
/// 2\. Initialize the search stack with a root @ref hgl::algorithm::search_node "search node" representing the starting point.
///
/// 3\. The *visit predicate* ensures vertices are not processed multiple times.
///
/// 4\. The concrete vertex visiting logic - marks the vertex as visited and logs it to the console.
///
/// 5\. Traverse all hyperedges unconditionally.
///
/// 6\. The *enqueue predicate* filters out already visited adjacent vertices before they are pushed to the stack.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | Dir | The @ref hgl::algorithm::traversal_direction "traversal direction" (i.e., `forward` or `backward`). Relevant only for BF-directed hypergraphs. | Defaults to `forward`. |
/// | H | The type of the hypergraph being searched. | Must satisfy the [**c_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph) concept. |
/// | InitQueueRangeType | A forward range of `search_node<hypergraph_val_t<H>>` used to prime the DFS stack. | Must be a *forward range* of @ref hgl::algorithm::search_node "search nodes". |
/// | VisitPredicate | Type of the callable deciding if a popped node should be processed. | Must be one of:<br/>- A `(const search_node<hypergraph_val_t<H>>&) -> bool` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Type of the callable executed when a vertex is officially visited. | Must be one of:<br/>- A `(const search_node<hypergraph_val_t<H>>&) -> bool` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | TraverseHePredicate | Type of the callable deciding if an incident hyperedge should be traversed. | Must be one of:<br/>- An `(id_type, id_type) -> decision` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | EnqueuePredicate | Type of the callable deciding if a target vertex should be pushed to the stack via a specific hyperedge. | Must be one of:<br/>- A `(const search_node<hypergraph_val_t<H>>&) -> decision` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. | Must be one of:<br/>- A `(const search_node<hypergraph_val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent elements are evaluated. | Must be one of:<br/>- A `(const search_node<hypergraph_val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
///
/// @param hypergraph The hypergraph to traverse.
/// @param initial_queue_content The initial set of search nodes to begin the traversal from.
/// @param visit_pred Predicate to filter nodes immediately after popping them from the stack.
/// @param visit Primary callback for node processing.
/// @param traverse_he_pred Predicate to determine if an incident hyperedge should be traversed. Returns a @ref hgl::algorithm::decision "decision":
/// - `accept` to traverse the hyperedge,
/// - `reject` to skip the hyperedge,
/// - `abort` to terminate the DFS entirely.
/// @param enqueue_pred Predicate to determine if an adjacent vertex should be pushed to the stack via a hyperedge. Returns a @ref hgl::algorithm::decision "decision":
/// - `accept` to push the vertex's search node,
/// - `reject` to skip the node,
/// - `abort` to terminate the DFS entirely.
/// @param pre_visit Callback executed prior to the primary visit logic.
/// @param post_visit Callback executed after all adjacent elements of the current node have been processed.
/// @return `true` if the search completed normally, `false` if it was explicitly aborted via a callback.
/// @hideparams
template <
    traversal_direction Dir = traversal_direction::forward,
    traits::c_hypergraph H,
    traits::c_forward_range_of<search_node<hypergraph_val_t<H>>> InitQueueRangeType =
        std::vector<search_node<hypergraph_val_t<H>>>,
    traits::c_optional_predicate<const search_node<hypergraph_val_t<H>>&> VisitPredicate =
        empty_callback,
    traits::c_optional_predicate<const search_node<hypergraph_val_t<H>>&> VisitCallback =
        empty_callback,
    traits::c_optional_decision_predicate<id_t<H>, id_t<H>> TraverseHePredicate = empty_callback,
    traits::c_decision_predicate<const search_node<hypergraph_val_t<H>>&> EnqueuePredicate =
        empty_callback,
    traits::c_optional_callback<void, const search_node<hypergraph_val_t<H>>&> PreVisitCallback =
        empty_callback,
    traits::c_optional_callback<void, const search_node<hypergraph_val_t<H>>&> PostVisitCallback =
        empty_callback>
bool dfs(
    H&& hypergraph,
    const InitQueueRangeType& initial_queue_content,
    const VisitPredicate& visit_pred = {},
    const VisitCallback& visit = {},
    const TraverseHePredicate& traverse_he_pred = {},
    const EnqueuePredicate& enqueue_pred = {},
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using policy = traversal_policy<H, Dir>;

    if (std::ranges::empty(initial_queue_content))
        return false;

    std::stack<search_node<hypergraph_val_t<H>>> s;
    for (const auto& node : initial_queue_content)
        s.push(node);

    while (not s.empty()) {
        const search_node curr_node = s.top();
        s.pop();

        if constexpr (not traits::c_empty_callback<VisitPredicate>)
            if (not visit_pred(curr_node))
                continue;

        if constexpr (not traits::c_empty_callback<PreVisitCallback>)
            pre_visit(curr_node);

        if constexpr (not traits::c_empty_callback<VisitCallback>)
            if (not visit(curr_node))
                return false;

        for (const auto he_id : policy::target_hyperedges(hypergraph, curr_node.vertex_id)) {
            if constexpr (not traits::c_empty_callback<TraverseHePredicate>) {
                const auto traverse = traverse_he_pred(he_id, curr_node.vertex_id);
                if (traverse == decision::abort)
                    return false;
                if (traverse == decision::reject)
                    continue;
            }

            for (const auto target_id : policy::target_vertices(hypergraph, he_id)) {
                if (target_id == curr_node.vertex_id)
                    continue;

                search_node<hypergraph_val_t<H>> tgt_node{target_id, curr_node.vertex_id, he_id};
                const auto enqueue = enqueue_pred(tgt_node);
                if (enqueue == decision::abort)
                    return false;
                if (enqueue)
                    s.push(tgt_node);
            }
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(curr_node);
    }

    return true;
}

} // namespace hgl::algorithm
