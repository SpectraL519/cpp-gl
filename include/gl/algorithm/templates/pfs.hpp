// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/pfs.hpp
/// @brief Generic Priority-First Search (PFS) template algorithm engine.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"

#include <queue>

namespace gl::algorithm {

/// @ingroup GL GL-Algorithm
/// @brief A highly customizable, generic Priority-First Search (PFS) algorithm engine.
///
/// This template provides the strict structural execution of a priority queue-based search.
/// It acts as the underlying engine for algorithms like Dijkstra's Shortest Path Search.
/// Concrete algorithms are constructed by injecting logic into the provided callbacks and
/// defining the custom priority comparator.
///
/// ### Example Usage
/// ```cpp
/// auto cmp = [](const auto& lhs, const auto& rhs) { // (1)!
///     return lhs.vertex_id > rhs.vertex_id; // Min-heap based on ID
/// };
///
/// std::vector<bool> visited(graph.n_vertices(), false); // (2)!
///
/// bool completed = gl::algorithm::pfs(
///     graph,
///     cmp,                                                    // (3)!
///     gl::algorithm::init_range<graph_type>(start_id),        // (4)!
///     gl::algorithm::default_visit_vertex_predicate(visited), // (5)!
///     [&](auto v, auto p) {                                   // (6)!
///         std::cout << "Priority visited vertex " << v << '\n';
///         return true; // Continue search
///     },
///     gl::algorithm::default_enqueue_vertex_predicate<graph_type, true>(visited) // (7)!
/// );
/// ```
///
/// 1\. A custom comparator for the internal priority queue.
///
/// 2\. Tracks discovered vertices.
///
/// 3\. Injects the comparator to order the search exploration.
///
/// 4\. Initializes the priority queue with the starting vertex.
///
/// 5\. Predicate evaluated after popping the highest priority vertex.
///
/// 6\. The main visit callback. Returning `false` aborts the search.
///
/// 7\. Predicate determining if an adjacent vertex should be pushed into the priority queue.
///
/// ### Template Parameters
/// | Parameter | Description |
/// | :-------- | :--- |
/// | G | The type of the graph being traversed. |
/// | PQCompare | The comparator type used to order elements within the internal `std::priority_queue`. |
/// | InitQueueRangeType | The type of the container providing the initial roots to enqueue. |
/// | VisitVertexPredicate | Type of the callable deciding if a popped vertex should be processed. |
/// | VisitCallback | Type of the callable executed when a vertex is officially visited. |
/// | EnqueueVertexPred | Type of the callable deciding if an adjacent vertex should be pushed to the queue. |
/// | PreVisitCallback | Type of the callable executed immediately before `VisitCallback`. |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges are evaluated. |
///
/// @param graph The graph to traverse.
/// @param pq_compare The comparator instance used to determine priority (highest priority is popped first).
/// @param initial_queue_content A range of initial @ref gl::algorithm::search_node "search nodes" to seed the priority queue.
/// @param visit_vertex_pred Predicate evaluated immediately after popping a vertex. If it returns `false`, the vertex is skipped (often used for late-rejection in Dijkstra).
/// @param visit Callback invoked when a vertex is officially visited. If it returns `false`, the entire PFS immediately aborts.
/// @param enqueue_vertex_pred Predicate evaluated for each outgoing edge. Returns a @ref gl::algorithm::decision "decision":
/// - `accept` to enqueue,
/// - `reject` to skip,
/// - `abort` to terminate the PFS entirely.
/// @param pre_visit Hook executed immediately before the `visit` callback.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been evaluated.
/// @return `true` if the queue was exhausted naturally, `false` if the search was aborted early by a callback or predicate.
template <
    traits::c_graph G,
    traits::c_predicate<search_node<G>, search_node<G>> PQCompare,
    traits::c_forward_range_of<search_node<G>> InitQueueRangeType = std::vector<search_node<G>>,
    traits::c_optional_predicate<typename G::id_type> VisitVertexPredicate = empty_callback,
    traits::c_optional_predicate<typename G::id_type, typename G::id_type> VisitCallback =
        empty_callback,
    traits::c_decision_predicate<typename G::id_type, const typename G::edge_type&>
        EnqueueVertexPred = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
bool pfs(
    const G& graph,
    const PQCompare& pq_compare,
    const InitQueueRangeType& initial_queue_content,
    VisitVertexPredicate visit_vertex_pred = {},
    VisitCallback visit = {},
    EnqueueVertexPred enqueue_vertex_pred = {},
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    if (std::ranges::empty(initial_queue_content))
        return false;

    // prepare the node queue
    using queue_type = std::priority_queue<search_node<G>, std::vector<search_node<G>>, PQCompare>;
    queue_type q(pq_compare);

    for (const auto& node : initial_queue_content)
        q.push(node);

    // search the graph
    while (not q.empty()) {
        const auto node = q.top();
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
            const auto enqueue = enqueue_vertex_pred(target_vertex_id, edge);
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
