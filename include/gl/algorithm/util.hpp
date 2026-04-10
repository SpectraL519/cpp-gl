// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/constants.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"

namespace gl::algorithm {

template <result_discriminator Result, traits::c_graph G>
[[nodiscard]] gl_attr_force_inline non_void_return_type<Result, predecessors_map<G>>
init_predecessors_map(const G& graph) {
    using return_t = non_void_return_type<Result, predecessors_map<G>>;
    if constexpr (Result == ret)
        return return_t(graph.order(), invalid_id);
    else
        return return_t();
}

template <traits::c_id_type IdType>
[[nodiscard]] gl_attr_force_inline bool is_reachable(
    const traits::c_random_access_range_of<IdType> auto& pred_map, IdType vertex_id
) noexcept {
    return pred_map[to_idx(vertex_id)] != invalid_id;
}

template <
    traits::c_graph G,
    traits::c_forward_range_of<search_node<G>> InitRangeType = std::vector<search_node<G>>>
[[nodiscard]] gl_attr_force_inline InitRangeType init_range(typename G::id_type root_vertex_id) {
    return InitRangeType{search_node<G>{root_vertex_id}};
}

[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited) {
    return [&](traits::c_id_type auto vertex_id) -> bool { return not visited[to_idx(vertex_id)]; };
}

template <traits::c_graph G, result_discriminator Result>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited, non_void_return_type<Result, predecessors_map<G>>& pred_map
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

template <traits::c_graph G, bool AsResult = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_vertex_predicate(std::vector<bool>& visited
) {
    using return_t = std::conditional_t<AsResult, decision, bool>;
    return [&](typename G::id_type vertex_id, const typename G::edge_type&) -> return_t {
        return not visited[to_idx(vertex_id)];
    };
}

} // namespace gl::algorithm
