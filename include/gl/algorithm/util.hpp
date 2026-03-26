// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/constants.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"

namespace gl::algorithm {

template <result_discriminator ResultDiscriminator>
[[nodiscard]] gl_attr_force_inline non_void_return_type<ResultDiscriminator, predecessors_map>
init_predecessors_map(const traits::c_graph auto& graph) {
    using return_type = non_void_return_type<ResultDiscriminator, predecessors_map>;
    if constexpr (ResultDiscriminator == algorithm::ret)
        return return_type(graph.order(), constants::invalid_id);
    else
        return return_type();
}

[[nodiscard]] gl_attr_force_inline bool is_reachable(
    const traits::c_random_access_range_of<id_type> auto& pred_map, const id_type vertex_id
) noexcept {
    return pred_map[vertex_id] != constants::invalid_id;
}

template <
    traits::c_forward_range_of<algorithm::vertex_info> InitRangeType =
        std::vector<algorithm::vertex_info>>
[[nodiscard]] gl_attr_force_inline InitRangeType init_range(id_type root_vertex_id) {
    return InitRangeType{algorithm::vertex_info{root_vertex_id}};
}

[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited) {
    return [&](const id_type vertex_id) -> bool {
        return not visited[static_cast<size_type>(vertex_id)];
    };
}

template <result_discriminator ResultDiscriminator>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited,
    non_void_return_type<ResultDiscriminator, predecessors_map>& pred_map
) {
    return [&](const id_type vertex_id, const id_type pred_id) {
        visited[static_cast<size_type>(vertex_id)] = true;
        if constexpr (ResultDiscriminator == algorithm::ret)
            pred_map[static_cast<size_type>(vertex_id)] = pred_id;
        return true;
    };
}

template <traits::c_graph GraphType, bool AsResult = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_vertex_predicate(std::vector<bool>& visited
) {
    using return_type = std::conditional_t<AsResult, predicate_result, bool>;

    return
        [&](const id_type vertex_id, [[maybe_unused]] const typename GraphType::edge_type& in_edge
        ) -> return_type { return not visited[static_cast<size_type>(vertex_id)]; };
}

} // namespace gl::algorithm
