// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/spanning_tree/prim_mst.hpp
/// @brief Concrete implementations of Prim's Minimum Spanning Tree (MST) algorithm.

#pragma once

#include "gl/algorithm/util.hpp"
#include "gl/constants.hpp"

#include <numeric>
#include <queue>

namespace gl::algorithm {

/// @ingroup GL-Algorithm
/// @brief A descriptor structure holding the results of a Minimum Spanning Tree (MST) execution.
///
/// @tparam G The type of the undirected graph. Must satisfy the [**c_undirected_graph**](gl_concepts.md#gl-traits-c-undirected-graph) concept.
template <traits::c_undirected_graph G>
struct mst_descriptor {
    /// @brief The type of the graph.
    using graph_type = traits::graph_val_t<G>;
    /// @brief The type of the edges stored in the graph.
    using edge_type = edge_t<G>;
    /// @brief The numeric type used to represent accumulated tree weights.
    using weight_type = vertex_distance_t<graph_type>;

    /// @brief Constructs a descriptor sized to hold the resulting tree edges.
    /// @param n_vertices The total number of vertices in the graph.
    mst_descriptor(const size_type n_vertices) {
        edges.reserve(n_vertices - 1uz);
    }

    /// @brief The sequence of edges that form the Minimum Spanning Tree.
    std::vector<edge_type> edges;
    /// @brief The accumulated minimum weight/cost of the entire spanning tree.
    weight_type weight = static_cast<weight_type>(0);
};

/// @ingroup GL-Algorithm
/// @brief Computes the Minimum Spanning Tree (MST) of an undirected graph using Prim's algorithm with an edge-based priority queue.
///
/// This implementation uses a standard binary heap (`std::priority_queue`) to store and sort edges based on their weight.
/// It pushes newly discovered adjacent edges into the queue and safely ignores those that lead to already-visited vertices.
///
/// ### Example Usage
/// ```cpp
/// auto mst = gl::algorithm::edge_heap_prim_mst(graph, start_id); // (1)!
/// std::cout << "Total MST Weight: " << mst.weight
///           << "\nMST Edges: " << gl::io::set_formatter(mst.edges) << '\n';
/// ```
///
/// 1\. Computes the MST starting from the given `start_id`. If `invalid_id` is passed, it defaults to the graph's `initial_id`.
///
/// > [!INFO] Algorithmic Complexity
/// >
/// > The time complexity depends on the underlying representation of `GraphType` and the queue overhead:
/// > - **Adjacency List Representations**: \f$O(|E| \log |E|)\f$
/// >   - *Includes:* @ref gl::repr::list_t "list_t" and @ref gl::repr::flat_list_t "flat_list_t".
/// >   - *Note:* In simple graphs, this simplifies to \f$O(|E| \log |V|)\f$. However, because list models allow multigraphs, the queue size and operations scale strictly with \f$|E|\f$.
/// > - **Adjacency Matrix Representations**: \f$O(|V|^2 + |E| \log |V|)\f$
/// >   - *Includes:* @ref gl::repr::matrix_t "matrix_t" and @ref gl::repr::flat_matrix_t "flat_matrix_t".
/// >   - *Note:* Iterating over incident edges requires scanning the entire \f$|V|\f$-length matrix row. Since matrices represent simple graphs, the heap operations safely simplify to \f$O(\log |V|)\f$.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the undirected graph being traversed. | Must satisfy the [**c_undirected_graph**](gl_concepts.md#gl-traits-c-undirected-graph) concept. |
///
/// @param graph The undirected graph to evaluate.
/// @param root_id The starting vertex ID for the MST calculation. Defaults to the graph's `initial_id` if `invalid_id` is passed.
/// @return A @ref gl::algorithm::mst_descriptor "mst_descriptor" containing the accumulated minimum weight and the sequence of edges forming the tree.
/// ### See Also
/// - @ref gl::algorithm::vertex_heap_prim_mst "vertex_heap_prim_mst" For the vertex-heap variant of the Prim's MST finding algorithm.
/// @hideparams
template <traits::c_undirected_graph G>
[[nodiscard]] mst_descriptor<G> edge_heap_prim_mst(G&& graph, id_t<G> root_id) {
    // type definitions
    using edge_type = edge_t<G>;

    struct edge_comparator {
        [[nodiscard]] gl_attr_force_inline bool operator()(
            const edge_type& lhs, const edge_type& rhs
        ) const {
            return get_weight<G>(lhs) > get_weight<G>(rhs);
        }
    };

    using queue_type = std::priority_queue<edge_type, std::vector<edge_type>, edge_comparator>;

    // prepare the necessary utility
    const auto n_vertices = graph.n_vertices();
    mst_descriptor<G> mst(n_vertices);
    std::vector<bool> visited(n_vertices, false);
    queue_type edge_queue;

    // insert the edges incident with the root vertex to the queue
    if (root_id == invalid_id)
        root_id = initial_id;

    for (const auto& edge : graph.out_edges(root_id))
        edge_queue.emplace(edge);

    // mark the root vertex as visited
    visited[to_idx(root_id)] = true;
    size_type n_vertices_in_mst = 1uz;

    // find the mst
    while (n_vertices_in_mst < n_vertices) {
        const auto min_edge = edge_queue.top();
        const auto min_edge_tgt = to_idx(min_edge.target());
        edge_queue.pop();

        if (visited[min_edge_tgt])
            continue;

        // add the minimum weight edge to the mst
        mst.edges.emplace_back(min_edge);
        mst.weight += get_weight<G>(min_edge);

        visited[min_edge_tgt] = true;
        ++n_vertices_in_mst;

        // enqueue all edges incident with the `target` vertex if they lead to unvisited verties
        for (const auto& edge : graph.incident_edges(min_edge.target()))
            if (not visited[to_idx(edge.other(min_edge.target()))])
                edge_queue.emplace(edge);
    }

    return mst;
}

/// @ingroup GL-Algorithm
/// @brief Computes the Minimum Spanning Tree (MST) of an undirected graph using Prim's algorithm with a vertex-based array heap.
///
/// This variation maintains a heap of vertex IDs based on their minimum known connection cost.
/// Because standard C++ heaps do not support a `decrease_key` operation, this implementation
/// dynamically rebuilds the heap (`std::make_heap`) at the end of each iteration to reflect updated distances.
///
/// ### Example Usage
/// ```cpp
/// auto mst = gl::algorithm::vertex_heap_prim_mst(graph, start_id); // (1)!
/// std::cout << "Total MST Weight: " << mst.weight
///           << "\nMST Edges: " << gl::io::set_formatter(mst.edges) << '\n';
/// ```
///
/// 1\. Computes the MST starting from the given `start_id`. Highly optimal for dense matrix graphs.
///
/// > [!INFO] Algorithmic Complexity
/// >
/// > Due to rebuilding the heap (\f$O(|V|)\f$) up to \f$|V|\f$ times, combined with evaluating every edge, the strict time complexity is \f$O(|V|^2 + |E|)\f$:
/// > - **Adjacency Matrix Representations**: \f$O(|V|^2)\f$
/// >   - *Note:* Since matrix models inherently represent simple graphs (where \f$|E| \le |V|^2\f$), the complexity strictly simplifies to \f$O(|V|^2)\f$. This makes the vertex heap approach highly suitable for dense graphs.
/// > - **Adjacency List Representations**: \f$O(|V|^2 + |E|)\f$
/// >   - *Note:* For multigraphs, the edge count \f$|E|\f$ can exceed \f$|V|^2\f$, meaning the edge traversal phase will dictate the overall performance.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the undirected graph being traversed. | Must satisfy the [**c_undirected_graph**](gl_concepts.md#gl-traits-c-undirected-graph) concept and its @ref gl::vertex_distance_t "distance type" must satisfy [**c_has_numeric_limits_max**](gl_concepts.md#gl-traits-c-has-numeric-limits-max).
///
/// @param graph The undirected graph to evaluate.
/// @param root_id The starting vertex ID for the MST calculation. Defaults to the graph's `initial_id` if `invalid_id` is passed.
/// @return A @ref gl::algorithm::mst_descriptor "mst_descriptor" containing the accumulated minimum weight and the sequence of edges forming the tree.
/// ### See Also
/// - @ref gl::algorithm::edge_heap_prim_mst "edge_heap_prim_mst" For the vertex-heap variant of the Prim's MST finding algorithm.
/// @hideparams
template <traits::c_undirected_graph G>
requires(traits::c_has_numeric_limits_max<vertex_distance_t<G>>)
[[nodiscard]] mst_descriptor<G> vertex_heap_prim_mst(G&& graph, id_t<G> root_id) {
    // type definitions
    using id_type = id_t<G>;
    using edge_type = edge_t<G>;
    using distance_type = vertex_distance_t<G>;

    // Prepare the necessary utility
    const auto n_vertices = graph.n_vertices();
    mst_descriptor<G> mst(n_vertices);

    std::vector<bool> in_mst(n_vertices, false);
    std::vector<distance_type> min_cost(n_vertices, std::numeric_limits<distance_type>::max());
    std::vector<std::optional<edge_type>> min_cost_edges(n_vertices, std::nullopt);

    // set the distance to the root vertex to 0
    if (root_id == invalid_id)
        root_id = initial_id;

    min_cost.at(root_id) = static_cast<distance_type>(0);

    auto heap_comparator = [&min_cost](const id_type lhs, const id_type rhs) {
        return min_cost[lhs] > min_cost[rhs]; // min-heap based on min_cost
    };

    // Initialize the vertex info and the heap
    std::vector<id_type> heap(n_vertices);
    std::iota(heap.begin(), heap.end(), initial_id_v<id_type>);
    std::make_heap(heap.begin(), heap.end(), heap_comparator);

    while (not heap.empty()) {
        // Extract the vertex with the smallest cost
        std::pop_heap(heap.begin(), heap.end(), heap_comparator);
        const auto vertex_id = heap.back();
        heap.pop_back();

        if (in_mst[vertex_id])
            continue;

        in_mst[vertex_id] = true;

        const auto min_cost_edge = min_cost_edges[vertex_id];
        if (min_cost_edge.has_value()) { // Add the corresponding edge to MST
            mst.edges.emplace_back(*min_cost_edge);
            mst.weight += min_cost[vertex_id];
        }

        // Update adjacent vertices
        for (const auto& edge : graph.incident_edges(vertex_id)) {
            const auto edge_weight = get_weight<G>(edge);
            const auto target_vertex_idx = to_idx(edge.other(vertex_id));

            if (not in_mst[target_vertex_idx] and edge_weight < min_cost[target_vertex_idx]) {
                min_cost[target_vertex_idx] = edge_weight;
                min_cost_edges[target_vertex_idx].emplace(edge);
            }
        }

        // Rebuild the heap for the updated vertices
        std::make_heap(heap.begin(), heap.end(), heap_comparator);
    }

    return mst;
}

} // namespace gl::algorithm
