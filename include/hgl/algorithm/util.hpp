// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/util.hpp"
#include "hgl/algorithm/core.hpp"

namespace hgl::algorithm {

template <result_discriminator ResultDiscriminator, hgl::traits::c_hypergraph HypergraphType>
[[nodiscard]] gl_attr_force_inline
    non_void_return_type<ResultDiscriminator, search_tree<HypergraphType>>
    init_search_tree(const HypergraphType& hypergraph) {
    using return_t = non_void_return_type<ResultDiscriminator, search_tree<HypergraphType>>;
    if constexpr (ResultDiscriminator == ret)
        return return_t(hypergraph.order());
    else
        return return_t();
}

// TODO: is_reachable

template <hgl::traits::c_hypergraph HypergraphType>
[[nodiscard]] gl_attr_force_inline std::vector<search_node<HypergraphType>> init_range(
    typename HypergraphType::id_type root_vertex_id
) {
    return std::vector<search_node<HypergraphType>>{search_node<HypergraphType>{root_vertex_id}};
}

template <hgl::traits::c_hypergraph HypergraphType>
[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited_v
) {
    return [&](const search_node<HypergraphType>& node) -> bool {
        return not visited_v[to_idx(node.vertex_id)];
    };
}

template <hgl::traits::c_hypergraph HypergraphType, result_discriminator ResultDiscriminator>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited_v,
    non_void_return_type<ResultDiscriminator, search_tree<HypergraphType>>& pred_map
) {
    return [&](const search_node<HypergraphType>& node) {
        const auto vertex_idx = to_idx(node.vertex_id);
        visited_v[vertex_idx] = true;
        if constexpr (ResultDiscriminator == ret)
            pred_map[vertex_idx] = node;
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
    return [&](const search_node<HypergraphType>& node) -> return_t {
        return return_t(not visited_v[gl::to_idx(node.id)]);
    };
}

} // namespace hgl::algorithm

// TODO! Validate `const Callback&` vs `Callback&&` in alg templates
