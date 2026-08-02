// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/algorithm/backward_search.hpp
/// @brief Concrete backward B-search traversals (B-BFS and B-DFS) for bf-directed hypergraphs.

#pragma once

#include "hgl/algorithm/core.hpp"
#include "hgl/algorithm/templates/bfs.hpp"
#include "hgl/algorithm/templates/dfs.hpp"
#include "hgl/algorithm/util.hpp"
#include "hgl/hypergraph.hpp"

namespace hgl::algorithm {

/// @ingroup HGL-Algorithm
/// @brief Executes a Breadth-First B-Search (B-BFS) traversal over a bf-directed hypergraph.
///
/// This algorithm implements B-reachability semantics using Breadth-First Search for BF-directed hypergraphs.
/// Unlike a standard traversal (where reaching a single tail vertex is sufficient to traverse an outgoing
/// hyperedge), a backward search (B-search) uses a blocking predicate. A hyperedge is only traversed, and
/// its head vertices enqueued, after **all** of its tail (source) vertices have been visited.
///
/// ### Example Usage
/// ```cpp
/// std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
/// auto search_tree = hgl::algorithm::backward_bfs(hypergraph, roots); // (1)!
///
/// hgl::algorithm::backward_bfs<hgl::algorithm::noret>( // (2)!
///     hypergraph,
///     roots,
///     [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
///     [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
/// );
/// ```
///
/// 1\. Executes a B-BFS returning a search tree mapped to the components B-reachable from the specified roots.
///
/// 2\. Executes a B-BFS purely for side-effects (callbacks) without allocating memory for a search tree.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | Result | Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`). | Must be a valid @ref hgl::algorithm::result_discriminator "result_discriminator" enum value. |
/// | H | The type of the hypergraph being searched. | Must satisfy the [**c_bf_directed_hypergraph**](hgl_concepts.md#hgl-traits-c-bf-directed-hypergraph) concept. |
/// | RootRange | The type of the container providing the initial roots to enqueue. | Must satisfy [**c_forward_range_of**](gl_concepts.md#gl-traits-c-forward-range-of) over the hypergraph's `id_type`. |
/// | PreVisitCallback | Type of the callable executed immediately before visiting a vertex. | Must be one of:<br/>- A `(const search_node<val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent elements are evaluated. | Must be one of:<br/>- A `(const search_node<val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
///
/// @param hypergraph The bf-directed hypergraph to traverse.
/// @param root_vertices A range of initial vertex IDs to start the search from.
/// @param pre_visit Hook executed immediately before visiting the vertex.
/// @param post_visit Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated.
/// @return A @ref hgl::algorithm::search_tree "search_tree" if `Result == ret`, otherwise nothing (`void`).
/// @hideparams
template <
    result_discriminator Result = ret,
    traits::c_bf_directed_hypergraph H,
    traits::c_forward_range_of<id_t<H>> RootRange = std::vector<id_t<H>>,
    traits::c_optional_callback<void, const search_node<val_t<H>>&> PreVisitCallback =
        empty_callback,
    traits::c_optional_callback<void, const search_node<val_t<H>>&> PostVisitCallback =
        empty_callback>
result_type<Result, search_tree<val_t<H>>> backward_bfs(
    H&& hypergraph,
    const RootRange& root_vertices,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited_vertices(hypergraph.n_vertices(), false);
    auto tail_unvisited = hypergraph.tail_size_map() | std::ranges::to<std::vector>();

    auto stree = init_search_tree<Result>(hypergraph);
    auto root_queue =
        root_vertices
        | std::views::transform([](const id_t<H> root_id) { return root_node<H>(root_id); });

    // clang-format off

    bfs<traversal_direction::forward>(
        hypergraph,
        root_queue,
        default_visit_predicate<H>(visited_vertices),
        default_visit_callback<H, Result>(visited_vertices, stree),
        blocking_traverse_hyperedge_predicate(tail_unvisited),
        default_enqueue_predicate<H, true>(visited_vertices),
        pre_visit,
        post_visit
    );

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

/// @ingroup HGL-Algorithm
/// @brief Executes a Depth-First B-Search (B-DFS) traversal over a bf-directed hypergraph.
///
/// This algorithm implements B-reachability semantics using a stack-based Depth-First Search for BF-directed
/// hypergraphs. Unlike a standard traversal (where reaching a single tail vertex is sufficient to traverse an
/// outgoing hyperedge), a backward search (B-search) uses a blocking predicate. A hyperedge is only traversed,
/// and its head vertices enqueued, after **all** of its tail (source) vertices have been visited.
///
/// ### Example Usage
/// ```cpp
/// std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
/// auto search_tree = hgl::algorithm::backward_dfs(hypergraph, roots); // (1)!
///
/// hgl::algorithm::backward_dfs<hgl::algorithm::noret>( // (2)!
///     hypergraph,
///     roots,
///     [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
///     [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
/// );
/// ```
///
/// 1\. Executes a B-DFS returning a search tree mapped to the components B-reachable from the specified roots.
///
/// 2\. Executes a B-DFS purely for side-effects (callbacks) without allocating memory for a search tree.
///
/// ### Template Parameters
/// | Parameter | Description | Constraint |
/// | :-------- | :--- | :--- |
/// | Result | Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`). | Must be a valid @ref hgl::algorithm::result_discriminator "result_discriminator" enum value. |
/// | H | The type of the hypergraph being searched. | Must satisfy the [**c_bf_directed_hypergraph**](hgl_concepts.md#hgl-traits-c-bf-directed-hypergraph) concept. |
/// | RootRange | The type of the container providing the initial roots to enqueue. | Must satisfy [**c_forward_range_of**](gl_concepts.md#gl-traits-c-forward-range-of) over the hypergraph's `id_type`. |
/// | PreVisitCallback | Type of the callable executed immediately before officially visiting a vertex. | Must be one of:<br/>- A `(const search_node<val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
/// | PostVisitCallback | Type of the callable executed after all adjacent elements are evaluated. | Must be one of:<br/>- A `(const search_node<val_t<H>>&) -> void` callable<br/>- An @ref hgl::algorithm::empty_callback "empty_callback" |
///
/// @param hypergraph The bf-directed hypergraph to traverse.
/// @param root_vertices A range of initial vertex IDs to start the search from.
/// @param pre_visit Hook executed immediately before officially visiting the vertex.
/// @param post_visit Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated.
/// @return A @ref hgl::algorithm::search_tree "search_tree" if `Result == ret`, otherwise nothing (`void`).
/// @hideparams
template <
    result_discriminator Result = ret,
    traits::c_bf_directed_hypergraph H,
    traits::c_forward_range_of<id_t<H>> RootRange = std::vector<id_t<H>>,
    traits::c_optional_callback<void, const search_node<val_t<H>>&> PreVisitCallback =
        empty_callback,
    traits::c_optional_callback<void, const search_node<val_t<H>>&> PostVisitCallback =
        empty_callback>
result_type<Result, search_tree<val_t<H>>> backward_dfs(
    H&& hypergraph,
    const RootRange& root_vertices,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited_vertices(hypergraph.n_vertices(), false);
    auto tail_unvisited = hypergraph.tail_size_map() | std::ranges::to<std::vector>();

    auto stree = init_search_tree<Result>(hypergraph);
    auto root_queue =
        root_vertices
        | std::views::transform([](const id_t<H> root_id) { return root_node<H>(root_id); });

    // clang-format off

    dfs<traversal_direction::forward>(
        hypergraph,
        root_queue,
        default_visit_predicate<H>(visited_vertices),
        default_visit_callback<H, Result>(visited_vertices, stree),
        blocking_traverse_hyperedge_predicate(tail_unvisited),
        default_enqueue_predicate<H, true>(visited_vertices),
        pre_visit,
        post_visit
    );

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

} // namespace hgl::algorithm
