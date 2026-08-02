// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/algorithm/util.hpp
/// @brief Internal utilities and default behaviors used by hypergraph traversal algorithms.

#pragma once

#include "gl/algorithm/util.hpp"
#include "hgl/algorithm/core.hpp"

namespace hgl::algorithm {

/// @ingroup HGL-Algorithm
/// @brief Initializes a search tree based on the static result discriminator.
/// @tparam Result The compilation tag determining if the tree should actually be built.
/// @tparam H The type of the hypergraph.
/// @param hypergraph The hypergraph instance to size the search tree against.
/// @return A fully sized and initialized `search_tree` if `Result == ret`, otherwise a dummy `std::monostate`.
template <result_discriminator Result, traits::c_hypergraph H>
[[nodiscard]] gl_attr_force_inline non_void_result_type<Result, search_tree<val_t<H>>>
init_search_tree(H&& hypergraph) {
    using return_t = non_void_result_type<Result, search_tree<val_t<H>>>;
    if constexpr (Result == ret)
        return return_t{hypergraph.n_vertices()};
    else
        return return_t{};
}

/// @ingroup HGL-Algorithm
/// @brief Generates a default lambda predicate that checks if a popped search node has already been visited.
/// @tparam H The type of the hypergraph.
/// @param visited_v A reference to the boolean array tracking visited vertices.
/// @return A callable predicate that returns `true` if the vertex in the node has not been visited, `false` otherwise.
template <traits::c_hypergraph H>
[[nodiscard]] gl_attr_force_inline auto default_visit_predicate(std::vector<bool>& visited_v) {
    return [&](const search_node<val_t<H>>& node) -> bool {
        return not visited_v[to_idx(node.vertex_id)];
    };
}

/// @ingroup HGL-Algorithm
/// @brief Generates a default lambda callback executed upon officially visiting a vertex.
///
/// Marks the vertex as visited. If `Result == ret`, it also records the search node into the provided search tree.
///
/// @tparam H The type of the hypergraph.
/// @tparam Result The compilation tag dictating whether to populate the search tree.
/// @param visited_v A reference to the boolean array tracking visited vertices.
/// @param stree A reference to the search tree being populated (or a dummy if `Result == noret`).
/// @return A callable callback returning `true` to unconditionally continue the traversal.
/// @hideparams
template <traits::c_hypergraph H, result_discriminator Result>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited_v, non_void_result_type<Result, search_tree<val_t<H>>>& stree
) {
    return [&](const search_node<val_t<H>>& node) {
        visited_v[node.vertex_id] = true;
        if constexpr (Result == ret)
            stree.nodes[node.vertex_id] = {node.pred_id, node.hyperedge_id};
        return true;
    };
}

/// @ingroup HGL-Algorithm
/// @brief Generates a default lambda predicate that tracks traversed hyperedges to prevent redundant exploration.
///
/// Records hyperedges as they are encountered and rejects any that have already been traversed during the search.
///
/// @param visited_he A reference to the boolean array tracking visited hyperedges.
/// @return A callable predicate returning a @ref hgl::algorithm::decision "decision" (`accept` if not previously visited, `reject` if already visited).
[[nodiscard]] gl_attr_force_inline auto default_traverse_hyperedge_predicate(
    std::vector<bool>& visited_he
) {
    return [&](traits::c_id_type auto he_id, traits::c_id_type auto /*source_id*/) {
        const auto he_idx = to_idx(he_id);
        if (visited_he[he_idx])
            return decision::reject;

        visited_he[he_idx] = true;
        return decision::accept;
    };
}

/// @ingroup HGL-Algorithm
/// @brief Generates a lambda predicate that blocks hyperedge traversal until its associated counter reaches zero.
///
/// This predicate is particularly useful in topological sorting or multi-dependency algorithms for *BF-directed*
/// hypergraphs, where a hyperedge should only be evaluated once all of its dependencies (e.g., all vertices in
/// its tail) have been saturated or visited.
///
/// @param counter_map A reference to an array of size or dependency counters mapped to hyperedge IDs.
/// @return A callable predicate returning a @ref hgl::algorithm::decision "decision" (`accept` if the decremented counter reaches 0, `reject` otherwise).
[[nodiscard]] gl_attr_force_inline auto blocking_traverse_hyperedge_predicate(
    std::vector<size_type>& counter_map
) {
    return [&](traits::c_id_type auto he_id, traits::c_id_type auto /*source_id*/) {
        return static_cast<decision>(--counter_map[to_idx(he_id)] == 0uz);
    };
}

/// @ingroup HGL-Algorithm
/// @brief Generates a default lambda predicate that checks if a node corresponding to an adjacent vertex should be enqueued.
/// @tparam H The type of the hypergraph.
/// @tparam AsResult If `true`, the generated predicate returns a @ref hgl::algorithm::decision "decision" instead of a raw boolean.
/// @param visited_v A reference to the boolean array tracking visited vertices.
/// @return A callable predicate that returns `true` (or `decision::accept`) if the adjacent vertex has not been visited.
template <traits::c_hypergraph H, bool AsResult = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_predicate(std::vector<bool>& visited_v) {
    using return_t = std::conditional_t<AsResult, decision, bool>;
    return [&](const search_node<val_t<H>>& node) -> return_t {
        return return_t(not visited_v[to_idx(node.vertex_id)]);
    };
}

} // namespace hgl::algorithm
