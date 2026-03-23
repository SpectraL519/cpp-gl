// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"

namespace gl::algorithm::impl {

// --- common functions ---

template <result_discriminator ResultDiscriminator, typename ReturnType, traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline alg_return_type_non_void<ResultDiscriminator, ReturnType>
init_return_value(const GraphType& graph) {
    using return_type = alg_return_type_non_void<ResultDiscriminator, ReturnType>;
    if constexpr (ResultDiscriminator == algorithm::ret)
        return return_type(graph.order());
    else
        return return_type();
}

template <
    traits::c_forward_range_of<algorithm::vertex_info> InitRangeType =
        std::vector<algorithm::vertex_info>>
[[nodiscard]] gl_attr_force_inline InitRangeType init_range(types::id_type root_vertex_id) {
    return InitRangeType{algorithm::vertex_info{root_vertex_id}};
}

[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited) {
    return [&](const types::id_type vertex_id) -> bool { return not visited[vertex_id]; };
}

template <result_discriminator ResultDiscriminator>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited,
    alg_return_type_non_void<ResultDiscriminator, predecessors_descriptor>& pd
) {
    return [&](const types::id_type vertex_id, const types::id_type pred_id) {
        visited[vertex_id] = true;
        if constexpr (ResultDiscriminator == algorithm::ret)
            pd[vertex_id].emplace(pred_id);
        return true;
    };
}

template <traits::c_graph GraphType, bool AsResult = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_vertex_predicate(std::vector<bool>& visited
) {
    using return_type = std::conditional_t<AsResult, predicate_result, bool>;

    return [&](const types::id_type vertex_id, const typename GraphType::edge_type& in_edge
           ) -> return_type { return not visited[vertex_id]; };
}

} // namespace gl::algorithm::impl
