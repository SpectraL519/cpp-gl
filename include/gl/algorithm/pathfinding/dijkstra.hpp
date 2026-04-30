// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/pathfinding/dijkstra.hpp
/// @brief Concrete implementation of Dijkstra's single-source shortest path algorithm and path reconstruction utilities.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/pfs.hpp"
#include "gl/algorithm/util.hpp"
#include "gl/constants.hpp"
#include "gl/types/core.hpp"

namespace gl::algorithm {

/// @ingroup GL-Algorithm
/// @brief A descriptor structure holding the results of a single-source shortest path execution.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | VertexDistanceType | The numeric type used to represent accumulated path weights/distances. | Must satisfy the [**c_arithmetic**](gl_concepts.md#gl-traits-c-arithmetic) concept. |
template <traits::c_graph G, traits::c_arithmetic VertexDistanceType>
struct paths_descriptor {
    using id_type = typename G::id_type;
    using distance_type = VertexDistanceType;

    /// @brief Constructs a descriptor sized for the given number of vertices.
    /// @param n_vertices The total number of vertices in the graph.
    paths_descriptor(const size_type n_vertices)
    : predecessors(n_vertices, invalid_id), distances(n_vertices) {}

    /// @brief The predecessor map tracking the optimal path tree.
    predecessors_map<G> predecessors;
    /// @brief The accumulated shortest distances to each vertex from the source.
    std::vector<distance_type> distances;
};

/// @ingroup GL-Algorithm
/// @brief An alias for @ref gl::algorithm::paths_descriptor "paths_descriptor" that automatically deduces the appropriate distance type for the graph.
/// @tparam G The type of the graph.
template <traits::c_graph G>
using paths_descriptor_type = paths_descriptor<G, vertex_distance_type<G>>;

/// @ingroup GL-Algorithm
/// @brief Factory function to create an initialized paths descriptor sized for the given graph.
/// @tparam G The type of the graph.
/// @param graph The graph to size the descriptor against.
/// @return A @ref gl::algorithm::paths_descriptor "paths_descriptor" initialized with invalid predecessors and default-constructed distances.
template <traits::c_graph G>
[[nodiscard]] gl_attr_force_inline paths_descriptor_type<G> make_paths_descriptor(const G& graph) {
    return paths_descriptor_type<G>{graph.n_vertices()};
}

/// @ingroup GL-Algorithm
/// @brief Internal node structure for Dijkstra's algorithm to snapshot distances and preserve heap invariants.
///
/// This structure is used in the @ref gl::algorithm::dijkstra_shortest_paths "dijkstra_shortest_paths" algorithm
/// to capture the state of a vertex at the moment it is enqueued, ensuring that the priority queue remains stable
/// even if the global distance map is updated during traversal.
///
/// @tparam G The type of the graph. Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept.
template <traits::c_graph G>
struct dijkstra_search_node {
    /// @brief The type of the vertex ID.
    using id_type = typename G::id_type;

    id_type vertex_id; ///< @brief The ID of the vertex represented by this node.
    id_type pred_id; ///< The ID of the predecessor vertex used to reach this node.
    vertex_distance_type<G>
        distance; ///< The accumulated distance from the source to this vertex at the time of enqueueing.
};

/// @ingroup GL-Algorithm
/// @brief Computes the shortest paths from a single source vertex to all reachable vertices using Dijkstra's algorithm.
///
/// This algorithm utilizes the generic @ref gl::algorithm::pfs "pfs" template using the dedicated
/// @ref gl::algorithm::dijkstra_search_node "serch node type" to perform a priority-first search based
/// on accumulated edge weights. It strictly requires non-negative edge weights; if a negative weight is
/// encountered during traversal, the algorithm immediately throws an exception.
///
/// ### Example Usage
/// ```cpp
/// auto paths = gl::algorithm::dijkstra_shortest_paths(graph, source_id); // (1)!
///
/// auto path_to_target
///     = gl::algorithm::reconstruct_path(paths.predecessors, target_id); // (2)!
/// std::cout << "Path: "
///           << gl::io::range_formatter(path_to_target, " -> ", "", "") // (3)!
///           << "\nDistance: " << paths.distances[target_id] << '\n'; // (4)!
/// ```
///
/// 1\. Executes the shortest path calculation from the given `source_id`.
///
/// 2\. Reconstructs the exact sequence of vertices from the source to the `target_id` using the @ref gl::algorithm::reconstruct_path "reconstruct_path" function.
///
/// 3\. Prints the path to the target vertex using the @ref gl::io::range_formatter "range_formatter" helper.
///
/// 4\. Retrievs the total distance to the target vertex from the @ref gl::algorithm::paths_descriptor "paths descriptor" object returned by Dijkstra's algorithm.
///
/// > [!INFO] Algorithmic Complexity
/// >
/// > The time complexity depends on the underlying representation of `GraphType` and the priority queue overhead:
/// > - **Adjacency List Representations**: \f$O((|V| + |E|) \log |V|)\f$
/// >   - *Includes:* @ref gl::repr::list_t "list_t" and @ref gl::repr::flat_list_t "flat_list_t".
/// > - **Adjacency Matrix Representations**: \f$O(|V|^2 + |E| \log |V|)\f$
/// >   - *Includes:* @ref gl::repr::matrix_t "matrix_t" and @ref gl::repr::flat_matrix_t "flat_matrix_t".
/// >   - *Note:* Iterating over adjacent vertices requires scanning the entire \f$|V|\f$-length matrix row.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | G | The type of the graph being traversed. Must define a valid distance/weight property. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | PreVisitCallback | Type of the callable executed immediately before a vertex is officially visited. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges of a vertex are evaluated. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to evaluate.
/// @param source_id The starting vertex ID for the shortest path calculation.
/// @param pre_visit Hook executed immediately before the internal visit logic.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been enqueued.
/// @return A @ref gl::algorithm::paths_descriptor "paths_descriptor" containing the accumulated distances and predecessor map.
/// @throws std::invalid_argument If an edge with a negative weight is encountered.
/// @hideparams
template <
    traits::c_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
[[nodiscard]] paths_descriptor_type<G> dijkstra_shortest_paths(
    const G& graph,
    typename G::id_type source_id,
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    using id_type = typename G::id_type;
    using edge_type = typename G::edge_type;
    using distance_type = vertex_distance_type<G>;

    auto paths = make_paths_descriptor<G>(graph);

    paths.predecessors[source_id] = source_id;
    paths.distances[source_id] = distance_type{};

    std::optional<edge_type> negative_edge;

    // Seed the queue with the custom snapshot node
    std::vector<dijkstra_search_node<G>> init_queue{
        {source_id, source_id, distance_type{}}
    };

    pfs(
        graph,
        [](const dijkstra_search_node<G>& lhs, const dijkstra_search_node<G>& rhs
        ) { // pq comparator
            return lhs.distance > rhs.distance;
        },
        init_queue,
        [&paths](const dijkstra_search_node<G>& node) { // visit_vertex_pred (stale node rejection)
            return node.distance <= paths.distances[to_idx(node.vertex_id)];
        },
        empty_callback{}, // visit callback
        [&paths, &negative_edge](id_type vertex_id, const edge_type& in_edge)
            -> decision { // enqueue predicate
            const auto pred_id = in_edge.other(vertex_id);
            const auto edge_weight = get_weight<G>(in_edge);

            if (edge_weight < 0) {
                negative_edge.emplace(in_edge);
                return decision::abort;
            }

            const auto new_distance = paths.distances[to_idx(pred_id)] + edge_weight;
            auto& v_pred = paths.predecessors[to_idx(vertex_id)];
            auto& v_dist = paths.distances[to_idx(vertex_id)];

            if (v_pred == invalid_id or new_distance < v_dist) {
                v_dist = new_distance;
                v_pred = pred_id;
                return true;
            }

            return false;
        },
        [&paths](id_type target_id, id_type pred_id, const edge_type&) { // make_node callback
            return dijkstra_search_node<G>{target_id, pred_id, paths.distances[to_idx(target_id)]};
        },
        pre_visit,
        post_visit
    );

    if (negative_edge.has_value()) {
        const auto& edge = negative_edge.value();
        throw std::invalid_argument(std::format(
            "[alg::dijkstra_shortest_paths] Found an edge with a negative weight: [{}, {} | w={}]",
            edge.source(),
            edge.target(),
            get_weight<G>(edge)
        ));
    }

    return paths;
}

/// @ingroup GL-Algorithm
/// @brief Reconstructs the sequence of vertices forming a path to a specific target.
///
/// This utility walks backward through a predecessor map, starting from the `vertex_id`
/// until it reaches the root vertex (a vertex that is its own predecessor).
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | IdType | The type of the vertex IDs. | Must satisfy the [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) concept. |
/// | IdRange | The type of the random-access range containing the predecessor map. | Must satisfy the [**c_random_access_range_of**](gl_concepts.md#gl-traits-c-random-access-range-of) concept for `IdType`. |
///
/// @param predecessor_map The predecessor map generated by a traversal algorithm (e.g., BFS, DFS, Dijkstra).
/// @param vertex_id The target vertex ID to reconstruct the path for.
/// @return A `std::vector` containing the sequence of vertex IDs from the source to the target.
/// @throws std::invalid_argument If the target `vertex_id` is unreachable (its predecessor is invalid).
/// @hideparams
template <traits::c_id_type IdType, traits::c_random_access_range_of<IdType> IdRange>
[[nodiscard]] std::vector<IdType> reconstruct_path(
    const IdRange& predecessor_map, const IdType vertex_id
) {
    if (not is_reachable(predecessor_map, vertex_id))
        throw std::invalid_argument(
            std::format("[alg::reconstruct_path] The given vertex is unreachable: {}", vertex_id)
        );

    std::vector<IdType> path;
    auto curr = vertex_id;

    while (true) {
        path.push_back(curr);
        auto pred = predecessor_map[to_idx(curr)];
        if (pred == curr)
            break;
        curr = pred;
    }

    std::ranges::reverse(path);
    path.shrink_to_fit();
    return path;
}

} // namespace gl::algorithm
