// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/util.hpp"
#include "hgl/algorithm/core.hpp"

namespace hgl::algorithm {

template <result_discriminator ResultDiscriminator, hgl::traits::c_hypergraph HypergraphType>
[[nodiscard]] gl_attr_force_inline
    non_void_return_type<ResultDiscriminator, predecessors_map<HypergraphType>>
    init_predecessors_map(const HypergraphType& hypergraph) {
    using return_t = non_void_return_type<ResultDiscriminator, predecessors_map<HypergraphType>>;
    if constexpr (ResultDiscriminator == ret)
        return return_t(hypergraph.order(), traversal_context<HypergraphType>{no_root});
    else
        return return_t();
}

// TODO: is_reachable

template <hgl::traits::c_hypergraph HypergraphType>
[[nodiscard]] gl_attr_force_inline std::vector<traversal_context<HypergraphType>> init_range(
    typename HypergraphType::id_type root_vertex_id
) {
    return std::vector<traversal_context<HypergraphType>>{
        traversal_context<HypergraphType>{root_vertex_id}
    };
}

template <hgl::traits::c_hypergraph HypergraphType>
[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited_v
) {
    return [&](const traversal_context<HypergraphType>& ctx) -> bool {
        return not visited_v[to_idx(ctx.id)];
    };
}

template <hgl::traits::c_hypergraph HypergraphType, result_discriminator ResultDiscriminator>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited_v,
    non_void_return_type<ResultDiscriminator, predecessors_map<HypergraphType>>& pred_map
) {
    return [&](const traversal_context<HypergraphType>& ctx) {
        const auto vertex_idx = to_idx(ctx.id);
        visited_v[vertex_idx] = true;
        if constexpr (ResultDiscriminator == ret)
            pred_map[vertex_idx] = ctx;
        return true;
    };
}

[[nodiscard]] gl_attr_force_inline auto default_traverse_hyperedge_predicate(
    std::vector<bool>& visited_he
) {
    return [&](auto he_id, auto /*source_id*/) {
        const auto he_idx = gl::to_idx(he_id);
        if (visited_he[he_idx])
            return decision::reject;

        visited_he[he_idx] = true;
        return decision::accept;
    };
}

template <hgl::traits::c_hypergraph HypergraphType, bool AsResult = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_vertex_predicate(
    std::vector<bool>& visited_v
) {
    using return_t = std::conditional_t<AsResult, decision, bool>;
    return [&](const traversal_context<HypergraphType>& ctx) -> return_t {
        return return_t(not visited_v[gl::to_idx(ctx.id)]);
    };
}

} // namespace hgl::algorithm

// TODO! Validate `const Callback&` vs `Callback&&` in alg templates
