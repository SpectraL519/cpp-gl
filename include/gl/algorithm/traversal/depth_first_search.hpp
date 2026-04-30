// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/traversal/depth_first_search.hpp
/// @brief Concrete Depth-First Search (DFS) algorithm implementations (iterative and recursive).

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/dfs.hpp"
#include "gl/algorithm/util.hpp"
#include "gl/constants.hpp"

namespace gl::algorithm {

/// @ingroup GL-Algorithm
/// @brief Executes a concrete iterative Depth-First Search (DFS) traversal over the graph.
///
/// This function utilizes the generic @ref gl::algorithm::dfs "dfs" template to perform a standard, stack-based traversal.
/// It automatically manages the visited states and predecessor tracking.
///
/// If a specific `root_vertex_id` is provided, the algorithm explores only the connected component
/// reachable from that root. If `no_root` is used, it iteratively ensures that every disconnected
/// component in the entire graph is fully traversed.
///
/// ### Example Usage
/// ```cpp
/// // Standard execution returning a predecessor map for the whole graph
/// auto pred_map = gl::algorithm::depth_first_search(graph); // (1)!
/// ```
///
/// 1\. Explores every vertex in the graph and builds a complete predecessor forest.
///
/// ### Example Usage
/// ```cpp
/// auto pred_map
///     = gl::algorithm::depth_first_search(graph, start_id); // (1)!
///
/// gl::algorithm::depth_first_search<gl::algorithm::noret>( // (2)!
///     graph,
///     gl::algorithm::no_root, // (3)!
///     [](auto v) { std::cout << "Discovered: " << v << '\n'; } // (4)!
/// );
/// ```
///
/// 1\. Standard execution returning a predecessor map from a specific root. Traverses only the component reachable from `start_id` and builds a predecessor tree.
///
/// 2\. Execution purely for side-effects over the entire graph. Uses the @ref gl::algorithm::result_discriminator "noret" discriminator to completely compile away the predecessor map allocations.
///
/// 3\. Traverses all vertices in the graph, regardless of disconnected components.
///
/// 4\. A custom `PreVisitCallback` executed when a vertex is popped from the stack.
///
/// > [!INFO] Algorithmic Complexity
/// >
/// > The time complexity depends entirely on the underlying representation of `GraphType`:
/// > - **Adjacency List Representations**: \f$O(|V| + |E|)\f$
/// >   - *Includes:* @ref gl::repr::list_t "list_t" and @ref gl::repr::flat_list_t "flat_list_t".
/// > - **Adjacency Matrix Representations**: \f$O(|V|^2)\f$
/// >   - *Includes:* @ref gl::repr::matrix_t "matrix_t" and @ref gl::repr::flat_matrix_t "flat_matrix_t".
/// >   - *Note:* Iterating over adjacent vertices requires scanning the entire \f$|V|\f$-length matrix row.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | Result | @ref gl::algorithm::result_discriminator "Discriminator" dictating if the algorithm should return a predecessor map (`ret`) or `void` (`noret`). | Must be a valid @ref gl::algorithm::result_discriminator "result_discriminator" enum value. |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | PreVisitCallback | Type of the callable executed immediately before a vertex is officially visited. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges of a vertex are evaluated. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param root_vertex_id The starting vertex for the search. Defaults to @ref gl::algorithm::no_root "no_root" to traverse the entire graph.
/// @param pre_visit Hook executed immediately before the internal visit logic.
/// @param post_visit Hook executed after all adjacent edges of the current vertex have been evaluated.
/// @return A @ref gl::algorithm::predecessors_map "predecessors_map" mapping each visited vertex to its parent if `Result == ret`. Returns `void` otherwise.
/// @hideparams
template <
    result_discriminator Result = ret,
    traits::c_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
result_type<Result, predecessors_map<G>> depth_first_search(
    const G& graph,
    const typename G::id_type root_vertex_id = no_root,
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    std::vector<bool> visited(graph.n_vertices(), false);
    std::vector<typename G::id_type> sources(graph.n_vertices());

    auto pred_map = init_predecessors_map<Result>(graph);

    // clang-format off

    if (root_vertex_id != no_root) {
        dfs(
            graph,
            init_node_range<G>(root_vertex_id),
            default_visit_vertex_predicate(visited),
            default_visit_callback<G, Result>(visited, pred_map),
            default_enqueue_node_predicate<G, true>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_id : graph.vertex_ids())
            dfs(
                graph,
                init_node_range<G>(root_id),
                default_visit_vertex_predicate(visited),
                default_visit_callback<G, Result>(visited, pred_map),
                default_enqueue_node_predicate<G, true>(visited),
                pre_visit,
                post_visit
            );
    }

    // clang-format on

    if constexpr (Result == ret)
        return pred_map;
}

/// @ingroup GL-Algorithm
/// @brief Executes a concrete recursive Depth-First Search (DFS) traversal over the graph.
///
/// This function relies on the generic @ref gl::algorithm::r_dfs "r_dfs" template. Instead of a
/// heap-allocated stack, it utilizes the C++ call stack to navigate the graph.
///
/// > [!WARNING] Call Stack Depth
/// >
/// > While recursive DFS can be slightly faster due to the lack of heap allocations, it is
/// > vulnerable to Stack Overflow errors on extremely deep graphs (e.g., long linear paths).
///
/// ### Example Usage
/// ```cpp
/// gl::algorithm::recursive_depth_first_search<gl::algorithm::noret>( // (1)!
///     graph,
///     start_id, // (2)!
///     [](auto v) { std::cout << "Entering subtree of: " << v << '\n'; }, // (3)!
///     [](auto v) { std::cout << "Exiting subtree of: " << v << '\n'; }   // (4)!
/// );
/// ```
///
/// 1\. Execution purely for side-effects. Uses the @ref gl::algorithm::result_discriminator "noret" discriminator to completely compile away the predecessor map allocations.
///
/// 2\. Initiates the recursion from `start_id`.
///
/// 3\. Hook triggered before descending into the current vertex's adjacent neighbors.
///
/// 4\. Hook triggered after returning from all recursive calls for the current vertex.
///
/// > [!INFO] Algorithmic Complexity
/// >
/// > The time complexity depends entirely on the underlying representation of `GraphType`:
/// > - **Adjacency List Representations**: \f$O(|V| + |E|)\f$
/// >   - *Includes:* @ref gl::repr::list_t "list_t" and @ref gl::repr::flat_list_t "flat_list_t".
/// > - **Adjacency Matrix Representations**: \f$O(|V|^2)\f$
/// >   - *Includes:* @ref gl::repr::matrix_t "matrix_t" and @ref gl::repr::flat_matrix_t "flat_matrix_t".
/// >   - *Note:* Iterating over adjacent vertices requires scanning the entire \f$|V|\f$-length matrix row.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | Result | @ref gl::algorithm::result_discriminator "Discriminator" dictating if the algorithm should return a predecessor map (`ret`) or `void` (`noret`). | Must be a valid @ref gl::algorithm::result_discriminator "result_discriminator" enum value. |
/// | G | The type of the graph being traversed. | Must satisfy the [**c_graph**](gl_concepts.md#gl-traits-c-graph) concept. |
/// | PreVisitCallback | Type of the callable executed immediately before a vertex is officially visited. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent edges of a vertex are evaluated. | Must be one of:<br/>- `(id_type) -> void` callable<br/>- An @ref gl::algorithm::empty_callback "empty_callback" |
///
/// @param graph The graph to traverse.
/// @param root_vertex_id The starting vertex for the search. Defaults to @ref gl::algorithm::no_root "no_root" to traverse the entire graph.
/// @param pre_visit Hook executed immediately before the internal visit logic.
/// @param post_visit Hook executed after returning from all recursive calls for the current vertex.
/// @return A @ref gl::algorithm::predecessors_map "predecessors_map" mapping each visited vertex to its parent if `Result == ret`. Returns `void` otherwise.
/// @hideparams
template <
    result_discriminator Result = ret,
    traits::c_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
result_type<Result, predecessors_map<G>> recursive_depth_first_search(
    const G& graph,
    const typename G::id_type root_vertex_id = no_root,
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    std::vector<bool> visited(graph.n_vertices(), false);
    std::vector<typename G::id_type> sources(graph.n_vertices());

    auto pred_map = init_predecessors_map<Result>(graph);

    if (root_vertex_id != no_root) {
        r_dfs(
            graph,
            root_vertex_id,
            root_vertex_id, // pred_id
            default_visit_vertex_predicate(visited),
            default_visit_callback<G, Result>(visited, pred_map),
            default_enqueue_node_predicate<G>(visited),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto& root_id : graph.vertex_ids())
            r_dfs(
                graph,
                root_id,
                root_id, // pred_id
                default_visit_vertex_predicate(visited),
                default_visit_callback<G, Result>(visited, pred_map),
                default_enqueue_node_predicate<G>(visited),
                pre_visit,
                post_visit
            );
    }

    if constexpr (Result == ret)
        return pred_map;
}

} // namespace gl::algorithm
