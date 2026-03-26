// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/constants.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"

namespace gl::algorithm {

template <result_discriminator ResultDiscriminator, traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline
    non_void_return_type<ResultDiscriminator, predecessors_map<GraphType>>
    init_predecessors_map(const GraphType& graph) {
    using return_type = non_void_return_type<ResultDiscriminator, predecessors_map<GraphType>>;
    if constexpr (ResultDiscriminator == algorithm::ret)
        return return_type(graph.order(), constants::invalid_id<typename GraphType::id_type>);
    else
        return return_type();
}

template <traits::c_id_type IdType>
[[nodiscard]] gl_attr_force_inline bool is_reachable(
    const traits::c_random_access_range_of<IdType> auto& pred_map, IdType vertex_id
) noexcept {
    return pred_map[to_idx(vertex_id)] != constants::invalid_id<IdType>;
}

template <
    traits::c_graph GraphType,
    traits::c_forward_range_of<algorithm::vertex_info<GraphType>> InitRangeType =
        std::vector<algorithm::vertex_info<GraphType>>>
[[nodiscard]] gl_attr_force_inline InitRangeType
init_range(typename GraphType::id_type root_vertex_id) {
    return InitRangeType{algorithm::vertex_info<GraphType>{root_vertex_id}};
}

[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited) {
    return [&](traits::c_id_type auto vertex_id) -> bool { return not visited[to_idx(vertex_id)]; };
}

template <traits::c_graph GraphType, result_discriminator ResultDiscriminator>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited,
    non_void_return_type<ResultDiscriminator, predecessors_map<GraphType>>& pred_map
) {
    using id_type = typename GraphType::id_type;
    return [&](id_type vertex_id, id_type pred_id) {
        const auto vertex_idx = to_idx(vertex_id);
        visited[vertex_idx] = true;
        if constexpr (ResultDiscriminator == algorithm::ret)
            pred_map[vertex_idx] = pred_id;
        return true;
    };
}

template <traits::c_graph GraphType, bool AsResult = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_vertex_predicate(std::vector<bool>& visited
) {
    using return_type = std::conditional_t<AsResult, decision, bool>;
    return [&](typename GraphType::id_type vertex_id,
               [[maybe_unused]] const typename GraphType::edge_type& in_edge) -> return_type {
        return not visited[to_idx(vertex_id)];
    };
}

} // namespace gl::algorithm
