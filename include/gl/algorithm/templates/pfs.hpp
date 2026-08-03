// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/templates/pfs.hpp
/// @brief Generic Priority-First Search (PFS) template algorithm engine.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"

#include <concepts>
#include <queue>
#include <ranges>

namespace gl::algorithm {

/// @ingroup GL-Algorithm
/// @brief A highly customizable, generic Priority-First Search (PFS) algorithm engine.
///
/// This template provides the strict structural execution of a priority queue-based search.
/// It acts as the underlying engine for algorithms like Dijkstra's Shortest Path Search.
/// Concrete algorithms are constructed by injecting logic into the provided callbacks and
/// defining the custom priority comparator.
///
/// ### Example Usage
/// ```cpp
/// std::vector<bool> visited(graph.n_vertices(), false); // (1)!
///
/// bool completed = gl::algorithm::pfs(
///     graph,
///     [](const auto& lhs, const auto& rhs) { // (2)!
///         return lhs.vertex_id > rhs.vertex_id;
///     },
///     std::array{gl::algorithm::root_node<G>(start_vertex_id)}, // (3)!
///     gl::algorithm::default_visit_predicate<G>(visited), // (4)!
///     [&](const auto& node) { // (5)!
///         std::cout << "Priority visited vertex " << node.vertex_id << '\n';
///         return true; // Continue search
///     },
///     gl::algorithm::default_enqueue_node_predicate<graph_type, true>(visited) // (6)!
/// );
/// ```
///
/// 1\. Tracks discovered vertices.
///
/// 2\. Injects the comparator to order the search exploration. A min-heap based on vertex IDs.
///
/// 3\. Initializes the priority queue with the starting node. By default, this yields standard @ref gl::algorithm::search_node "search_nodes".
///
/// 4\. Predicate evaluated after popping the highest priority node. Accepts the entire `NodeType` to allow for snapshot inspections (like stale-node rejection).
///
/// 5\. The main visit callback receiving the full search node. Returning `false` aborts the search.
///
/// 6\. Predicate determining if an adjacent target node should be pushed into the priority queue.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | PQCmp | The comparator used to order elements within the priority queue. | Must be a `(NodeType, NodeType) -> bool` callable. |
/// | InitNodeRngType | The container providing the initial roots to enqueue. | Must satisfy `std::ranges::forward_range` and yield a @ref gl::algorithm::search_node "search_node". |
/// | NodeType | The exact search node type extracted implicitly from the range. | Must strictly match `search_node<val_t<G>, Extension>`. |
/// | VisitPredicate | Decides if a popped node should be processed. | Must be one of:<br/>- A `(const NodeType&) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Executed when a node is officially visited. | Must be one of:<br/>- A `(const NodeType&) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueuePredicate | Decides if a target node should be pushed to the queue. | Must be one of:<br/>- A `(const search_node<val_t<G>>&, const edge_t<G>&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | MakeNodeCallback | Constructs a custom `NodeType` before pushing to the queue. | Must be one of:<br/>- A `(id_type, id_type, const edge_t<G>&) -> NodeType` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Executed immediately before `VisitCallback`. | Must be one of:<br/>- A `(const NodeType&) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Executed after all adjacent edges are evaluated. | Must be one of:<br/>- A `(const NodeType&) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param pq_cmp The comparator instance used to determine priority (highest priority is popped first).
/// @param initial_nodes A range of initial nodes to seed the priority queue.
/// @param visit_pred Predicate evaluated immediately after popping a node. If it returns `false`, the node is skipped (often used for late-rejection in Dijkstra).
/// @param visit Callback invoked when a node is officially visited. If it returns `false`, the entire PFS immediately aborts.
/// @param enqueue_pred Predicate evaluated for each outgoing edge and target node. Returns a @ref gl::algorithm::decision "decision":
/// - `accept` to enqueue,
/// - `reject` to skip,
/// - `abort` to terminate the PFS entirely.
/// @param make_node Factory callback to construct a custom stateful node prior to enqueueing (useful for computing extensions like cumulative weights). Defaults to injecting a default-extended `NodeType`.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been evaluated.
/// @return `true` if the queue was exhausted naturally, `false` if the search was aborted early by a callback or predicate.
/// @hideparams
template <
    traits::c_graph G,
    typename PQCmp,
    typename InitNodeRngType = std::vector<search_node<val_t<G>>>,
    typename NodeType = std::ranges::range_value_t<InitNodeRngType>,
    traits::c_optional_predicate<const NodeType&> VisitPredicate = empty_callback,
    traits::c_optional_predicate<const NodeType&> VisitCallback = empty_callback,
    traits::c_decision_predicate<search_node<val_t<G>>, const edge_t<G>&> EnqueuePredicate =
        empty_callback,
    traits::c_optional_callback<NodeType, id_t<G>, id_t<G>, const edge_t<G>&> MakeNodeCallback =
        empty_callback,
    traits::c_optional_callback<void, const NodeType&> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, const NodeType&> PostVisitCallback = empty_callback>
requires(traits::c_predicate<PQCmp, NodeType, NodeType> and traits::c_instantiation_of<NodeType, search_node> and std::same_as<typename NodeType::graph_type, val_t<G>>)
bool pfs(
    G&& graph,
    const PQCmp& pq_cmp,
    const InitNodeRngType& initial_nodes,
    const VisitPredicate& visit_pred = {},
    const VisitCallback& visit = {},
    const EnqueuePredicate& enqueue_pred = {},
    const MakeNodeCallback& make_node = {},
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    if (std::ranges::empty(initial_nodes))
        return false;

    // prepare the node queue
    using queue_type = std::priority_queue<NodeType, std::vector<NodeType>, PQCmp>;
    queue_type q(pq_cmp);

    for (const auto& node : initial_nodes)
        q.push(node);

    // search the graph
    while (not q.empty()) {
        const auto curr_node = q.top();
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
            const auto target_vertex_id = edge.other(curr_node.vertex_id);
            search_node<val_t<G>> tgt_base_node{target_vertex_id, curr_node.vertex_id};

            const auto enqueue = enqueue_pred(tgt_base_node, edge);
            if (enqueue == decision::abort)
                return false;

            if (enqueue) {
                if constexpr (not traits::c_empty_callback<MakeNodeCallback>)
                    q.push(make_node(target_vertex_id, curr_node.vertex_id, edge));
                else
                    q.push(NodeType{target_vertex_id, curr_node.vertex_id});
            }
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(curr_node);
    }

    return true;
}

} // namespace gl::algorithm
