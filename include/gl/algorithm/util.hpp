// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/algorithm/util.hpp
/// @brief Internal utilities and default behaviors used by the graph traversal algorithms.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/constants.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"

namespace gl::algorithm {

/// @ingroup GL GL-Algorithm
/// @brief Initializes a predecessor map based on the static result discriminator.
/// @tparam Result The compilation tag determining if the map should actually be built.
/// @tparam G The type of the graph.
/// @param graph The graph instance to size the map against.
/// @return A fully sized and initialized `predecessors_map` if `Result == ret`, otherwise a dummy `std::monostate`.
template <result_discriminator Result, traits::c_graph G>
[[nodiscard]] gl_attr_force_inline non_void_result_type<Result, predecessors_map<G>>
init_predecessors_map(const G& graph) {
    using return_t = non_void_result_type<Result, predecessors_map<G>>;
    if constexpr (Result == ret)
        return return_t(graph.n_vertices(), invalid_id);
    else
        return return_t();
}

/// @ingroup GL GL-Algorithm
/// @brief Checks if a specific vertex was reached during a traversal.
/// @tparam IdType The integral type of the vertex ID.
/// @param pred_map The predecessor map populated by the traversal.
/// @param vertex_id The vertex ID to query.
/// @return `true` if the vertex has a valid assigned predecessor, `false` otherwise.
/// @hideparams
template <traits::c_id_type IdType>
[[nodiscard]] gl_attr_force_inline bool is_reachable(
    const traits::c_random_access_range_of<IdType> auto& pred_map, IdType vertex_id
) noexcept {
    return pred_map[to_idx(vertex_id)] != invalid_id;
}

/// @ingroup GL GL-Algorithm
/// @brief Initializes a search container (queue or stack) with the starting root vertex.
/// @tparam G The type of the graph.
/// @tparam InitRangeType The underlying container type for the container.
/// @param root_vertex_id The ID of the starting vertex.
/// @return A container initialized with a single @ref search_node for the root vertex.
template <
    traits::c_graph G,
    traits::c_forward_range_of<search_node<G>> InitRangeType = std::vector<search_node<G>>>
[[nodiscard]] gl_attr_force_inline InitRangeType init_range(typename G::id_type root_vertex_id) {
    return InitRangeType{search_node<G>{root_vertex_id}};
}

/// @ingroup GL GL-Algorithm
/// @brief Generates a default lambda predicate that checks if a vertex has not yet been visited.
/// @param visited A reference to the boolean array tracking visited vertices.
/// @return A callable predicate evaluating to `true` if the vertex is unvisited.
[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited) {
    return [&](traits::c_id_type auto vertex_id) -> bool { return not visited[to_idx(vertex_id)]; };
}

/// @ingroup GL GL-Algorithm
/// @brief Generates a default lambda callback that marks a vertex as visited and updates the predecessor map.
/// @tparam G The type of the graph.
/// @tparam Result The static discriminator indicating if the predecessor map should be updated.
/// @param visited A reference to the boolean array tracking visited vertices.
/// @param pred_map A reference to the active predecessor map.
/// @return A callable callback that executes state updates upon visiting a vertex.
/// @hideparams
template <traits::c_graph G, result_discriminator Result>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited, non_void_result_type<Result, predecessors_map<G>>& pred_map
) {
    using id_type = typename G::id_type;
    return [&](id_type vertex_id, id_type pred_id) {
        const auto vertex_idx = to_idx(vertex_id);
        visited[vertex_idx] = true;
        if constexpr (Result == ret)
            pred_map[vertex_idx] = pred_id;
        return true;
    };
}

/// @ingroup GL GL-Algorithm
/// @brief Generates a default lambda predicate that checks if a node corresponding to an adjacent vertex should be enqueued into the search container.
/// @tparam G The type of the graph.
/// @tparam AsDecision If `true`, the generated predicate returns a @ref gl::algorithm::decision "decision" instead of a raw boolean.
/// @param visited A reference to the boolean array tracking visited vertices.
/// @return A callable predicate that returns `true` (or `decision::accept`) if the adjacent vertex has not been visited.
template <traits::c_graph G, bool AsDecision = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_node_predicate(std::vector<bool>& visited) {
    using return_t = std::conditional_t<AsDecision, decision, bool>;
    return [&](typename G::id_type vertex_id, const typename G::edge_type&) -> return_t {
        return not visited[to_idx(vertex_id)];
    };
}

} // namespace gl::algorithm
