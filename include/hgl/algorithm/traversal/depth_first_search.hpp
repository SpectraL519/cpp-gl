// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/algorithm/traversal/depth_first_search.hpp
/// @brief Concrete Depth-First Search (DFS) traversal algorithm implementation for hypergraphs.

#pragma once

#include "hgl/algorithm/core.hpp"
#include "hgl/algorithm/templates/dfs.hpp"
#include "hgl/algorithm/util.hpp"

namespace hgl::algorithm {

/// @ingroup HGL-Algorithm
/// @brief Executes a concrete Depth-First Search (DFS) traversal over the hypergraph.
///
/// This function utilizes the generic @ref hgl::algorithm::dfs "dfs" template to perform a standard stack-based traversal.
/// It automatically manages the visited states (for both vertices and hyperedges), search tree tracking, and stack initialization.
///
/// If a specific `root_vertex_id` is provided, the algorithm explores only the connected component reachable from that root.
/// If `no_root` is used, it iteratively ensures that every disconnected component in the entire hypergraph is fully traversed.
///
/// ### Example Usage
/// ```cpp
/// auto search_tree
///     = hgl::algorithm::depth_first_search(hypergraph, start_id); // (1)!
///
/// hgl::algorithm::depth_first_search<hgl::algorithm::noret>( // (2)!
///     hypergraph,
///     hgl::algorithm::no_root, // (3)!
///     [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
///     [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
/// );
/// ```
///
/// 1\. Executes a standard DFS returning a search tree mapped to the components reachable from `start_id`.
///
/// 2\. Executes a DFS purely for side-effects (callbacks) without allocating memory for a search tree.
///
/// 3\. Passing `no_root` forces the algorithm to iterate over all vertices, ensuring disjoint components are traversed.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | Result | Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`). | Must be a valid @ref hgl::algorithm::result_discriminator "result_discriminator" enum value. |
/// | H | The type of the hypergraph being searched. | Must satisfy the [**c_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph) concept. |
/// | PreVisitCallback | Type of the callable executed immediately before visiting a vertex. | Must be one of:<br/>- A `(const search_node<hypergraph_val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent elements are evaluated. | Must be one of:<br/>- A `(const search_node<hypergraph_val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
///
/// @param hypergraph The hypergraph to traverse.
/// @param root_vertex_id The ID of the vertex to start the search from. If `no_root`, searches the entire hypergraph.
/// @param pre_visit Hook executed immediately before visiting the vertex.
/// @param post_visit Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated.
/// @return A @ref hgl::algorithm::search_tree "search_tree" if `Result == ret`, otherwise nothing (`void`).
/// @hideparams
template <
    result_discriminator Result = ret,
    traits::c_hypergraph H,
    traits::c_optional_callback<void, const search_node<hypergraph_val_t<H>>&> PreVisitCallback =
        empty_callback,
    traits::c_optional_callback<void, const search_node<hypergraph_val_t<H>>&> PostVisitCallback =
        empty_callback>
result_type<Result, search_tree<H>> depth_first_search(
    H&& hypergraph,
    const id_t<H> root_vertex_id = no_root,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited_vertices(hypergraph.n_vertices(), false);
    std::vector<bool> visited_hyperedges(hypergraph.n_hyperedges(), false);

    auto stree = init_search_tree<Result>(hypergraph);

    // clang-format off

    if (root_vertex_id != no_root) {
        dfs(
            hypergraph,
            init_node_range<H>(root_vertex_id),
            default_visit_predicate<H>(visited_vertices),
            default_visit_callback<H, Result>(visited_vertices, stree),
            default_traverse_hyperedge_predicate(visited_hyperedges),
            default_enqueue_predicate<H, true>(visited_vertices),
            pre_visit,
            post_visit
        );
    }
    else {
        for (const auto root_id : hypergraph.vertex_ids())
            dfs(
                hypergraph,
                init_node_range<H>(root_id),
                default_visit_predicate<H>(visited_vertices),
                default_visit_callback<H, Result>(visited_vertices, stree),
                default_traverse_hyperedge_predicate(visited_hyperedges),
                default_enqueue_predicate<H, true>(visited_vertices),
                pre_visit,
                post_visit
            );
    }

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

} // namespace hgl::algorithm
