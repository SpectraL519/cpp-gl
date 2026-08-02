// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/algorithm/forward_search.hpp
/// @brief Concrete forward F-search traversals (F-BFS and F-DFS) for bf-directed hypergraphs.

#pragma once

#include "hgl/algorithm/core.hpp"
#include "hgl/algorithm/templates/bfs.hpp"
#include "hgl/algorithm/templates/dfs.hpp"
#include "hgl/algorithm/util.hpp"
#include "hgl/hypergraph.hpp"

namespace hgl::algorithm {

/// @ingroup HGL-Algorithm
/// @brief Executes a Breadth-First F-Search (F-BFS) traversal over a bf-directed hypergraph.
///
/// This algorithm implements F-reachability semantics using Breadth-First Search for BF-directed hypergraphs.
/// Unlike a standard backward traversal (where reaching a single head vertex is sufficient to traverse an
/// incoming hyperedge), a forward search (F-search) uses a blocking predicate. A hyperedge is only traversed,
/// and its tail vertices enqueued, after **all** of its head (destination) vertices have been visited.
///
/// ### Example Usage
/// ```cpp
/// std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
/// auto search_tree = hgl::algorithm::forward_bfs(hypergraph, roots); // (1)!
///
/// hgl::algorithm::forward_bfs<hgl::algorithm::noret>( // (2)!
///     hypergraph,
///     roots,
///     [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
///     [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
/// );
/// ```
///
/// 1\. Executes an F-BFS returning a search tree mapped to the components F-reachable from the specified roots.
///
/// 2\. Executes an F-BFS purely for side-effects (callbacks) without allocating memory for a search tree.
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
result_type<Result, search_tree<val_t<H>>> forward_bfs(
    H&& hypergraph,
    const RootRange& root_vertices,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited_vertices(hypergraph.n_vertices(), false);
    auto head_unvisited = hypergraph.head_size_map() | std::ranges::to<std::vector>();

    auto stree = init_search_tree<Result>(hypergraph);
    auto root_queue =
        root_vertices
        | std::views::transform([](const id_t<H> root_id) { return root_node<H>(root_id); });

    // clang-format off

    bfs<traversal_direction::backward>(
        hypergraph,
        root_queue,
        default_visit_predicate<H>(visited_vertices),
        default_visit_callback<H, Result>(visited_vertices, stree),
        blocking_traverse_hyperedge_predicate(head_unvisited),
        default_enqueue_predicate<H, true>(visited_vertices),
        pre_visit,
        post_visit
    );

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

/// @ingroup HGL-Algorithm
/// @brief Executes a Depth-First F-Search (F-DFS) traversal over a bf-directed hypergraph.
///
/// This algorithm implements F-reachability semantics using a stack-based Depth-First Search for BF-directed
/// hypergraphs. Unlike a standard backward traversal (where reaching a single head vertex is sufficient to traverse an
/// incoming hyperedge), a forward search (F-search) uses a blocking predicate. A hyperedge is only traversed,
/// and its tail vertices pushed to the stack, after **all** of its head (destination) vertices have been officially visited.
///
/// ### Example Usage
/// ```cpp
/// std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
/// auto search_tree = hgl::algorithm::forward_dfs(hypergraph, roots); // (1)!
///
/// hgl::algorithm::forward_dfs<hgl::algorithm::noret>( // (2)!
///     hypergraph,
///     roots,
///     [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
///     [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
/// );
/// ```
///
/// 1\. Executes an F-DFS returning a search tree mapped to the components F-reachable from the specified roots.
///
/// 2\. Executes an F-DFS purely for side-effects (callbacks) without allocating memory for a search tree.
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
result_type<Result, search_tree<val_t<H>>> forward_dfs(
    H&& hypergraph,
    const RootRange& root_vertices,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    std::vector<bool> visited_vertices(hypergraph.n_vertices(), false);
    auto head_unvisited = hypergraph.head_size_map() | std::ranges::to<std::vector>();

    auto stree = init_search_tree<Result>(hypergraph);
    auto root_queue =
        root_vertices
        | std::views::transform([](const id_t<H> root_id) { return root_node<H>(root_id); });

    // clang-format off

    dfs<traversal_direction::backward>(
        hypergraph,
        root_queue,
        default_visit_predicate<H>(visited_vertices),
        default_visit_callback<H, Result>(visited_vertices, stree),
        blocking_traverse_hyperedge_predicate(head_unvisited),
        default_enqueue_predicate<H, true>(visited_vertices),
        pre_visit,
        post_visit
    );

    // clang-format on

    if constexpr (Result == ret)
        return stree;
}

} // namespace hgl::algorithm
