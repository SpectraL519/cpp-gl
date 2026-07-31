// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/pfs.hpp
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
///     gl::algorithm::init_node_range<graph_type>(start_id), // (3)!
///     gl::algorithm::default_visit_vertex_predicate(visited // (4)!
///     [&](auto v, auto p) { // (5)!
///         std::cout << "Priority visited vertex " << v << '\n';
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
/// 3\. Initializes the priority queue with the starting vertex. By default, this yields standard @ref gl::algorithm::search_node "search_nodes".
///
/// 4\. Predicate evaluated after popping the highest priority node. Accepts the entire `NodeType` to allow for snapshot inspections (like stale-node rejection).
///
/// 5\. The main visit callback. Returning `false` aborts the search.
///
/// 6\. Predicate determining if an adjacent vertex should be pushed into the priority queue.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | PQCmp | The comparator used to order elements within the priority queue. | Must be a `(NodeType, NodeType) -> bool` callable. |
/// | InitNodesType | The container providing the initial roots to enqueue. | Must satisfy `std::ranges::forward_range`. |
/// | NodeType | The type of the node stored in the priority queue. | Extracted implicitly. Must be constructible from `(id_type, id_type)` unless `MakeNodeCallback` is provided. |
/// | VisitVertexPredicate | Decides if a popped node should be processed. | Must be one of:<br/>- `(NodeType) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | VisitCallback | Executed when a vertex is officially visited. | Must be one of:<br/>- `(id_type, id_type) -> bool` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | EnqueueNodePred | Decides if a node corresponding to an adjacent vertex should be pushed to the queue. | Must be one of:<br/>- `(id_type, const edge_type&) -> decision` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | MakeNodeCallback | Constructs a custom `NodeType` before pushing to the queue. | Must be one of:<br/>- `(id_type, id_type, const edge_type&) -> NodeType` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PreVisitCallback | Executed immediately before `VisitCallback`. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Executed after all adjacent edges are evaluated. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param pq_cmp The comparator instance used to determine priority (highest priority is popped first).
/// @param initial_nodes A range of initial nodes to seed the priority queue.
/// @param visit_vertex_pred Predicate evaluated immediately after popping a node. If it returns `false`, the node is skipped (often used for late-rejection in Dijkstra).
/// @param visit Callback invoked when a vertex is officially visited. If it returns `false`, the entire PFS immediately aborts.
/// @param enqueue_node_pred Predicate evaluated for each outgoing edge. Returns a @ref gl::algorithm::decision "decision":
/// - `accept` to enqueue,
/// - `reject` to skip,
/// - `abort` to terminate the PFS entirely.
/// @param make_node Factory callback to construct a custom `NodeType` prior to enqueueing. Defaults to invoking the `NodeType(target_id, pred_id)` constructor.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been evaluated.
/// @return `true` if the queue was exhausted naturally, `false` if the search was aborted early by a callback or predicate.
/// @hideparams
template <
    traits::c_graph G,
    typename PQCmp,
    typename InitNodesType = std::vector<search_node<val_t<G>>>,
    typename NodeType = std::ranges::range_value_t<InitNodesType>,
    traits::c_optional_predicate<NodeType> VisitVertexPredicate = empty_callback,
    traits::c_optional_predicate<id_t<G>, id_t<G>> VisitCallback = empty_callback,
    traits::c_decision_predicate<id_t<G>, const edge_t<G>&> EnqueueNodePred = empty_callback,
    traits::c_optional_callback<NodeType, id_t<G>, id_t<G>, const edge_t<G>&> MakeNodeCallback =
        empty_callback,
    traits::c_optional_callback<void, id_t<G>> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, id_t<G>> PostVisitCallback = empty_callback>
requires traits::c_predicate<PQCmp, NodeType, NodeType>
bool pfs(
    G&& graph,
    const PQCmp& pq_cmp,
    const InitNodesType& initial_nodes,
    VisitVertexPredicate visit_vertex_pred = {},
    VisitCallback visit = {},
    EnqueueNodePred enqueue_node_pred = {},
    MakeNodeCallback make_node = {},
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
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
        const auto node = q.top();
        q.pop();

        if constexpr (not traits::c_empty_callback<VisitVertexPredicate>)
            if (not visit_vertex_pred(node))
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

            if (enqueue) {
                if constexpr (not traits::c_empty_callback<MakeNodeCallback>) {
                    q.push(make_node(target_vertex_id, node.vertex_id, edge));
                }
                else {
                    static_assert(
                        std::constructible_from<NodeType, id_t<G>, id_t<G>>,
                        "[gl::algorithm::pfs] Custom NodeType provided without a MakeNodeCallback. "
                        "The NodeType must be constructible from (target_id, pred_id), or you must "
                        "provide a MakeNodeCallback!"
                    );
                    q.emplace(target_vertex_id, node.vertex_id);
                }
            }
        }

        if constexpr (not traits::c_empty_callback<PostVisitCallback>)
            post_visit(node.vertex_id);
    }

    return true;
}

} // namespace gl::algorithm
