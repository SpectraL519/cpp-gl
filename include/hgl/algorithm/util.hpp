// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/util.hpp"
#include "hgl/algorithm/core.hpp"

namespace hgl::algorithm {

template <result_discriminator Result, traits::c_hypergraph H>
[[nodiscard]] gl_attr_force_inline non_void_result_type<Result, search_tree<H>> init_search_tree(
    const H& hypergraph
) {
    using return_t = non_void_result_type<Result, search_tree<H>>;
    if constexpr (Result == ret)
        return return_t(hypergraph.n_vertices());
    else
        return return_t();
}

[[nodiscard]] gl_attr_force_inline bool is_reachable(
    const traits::c_search_tree auto& tree, traits::c_id_type auto vertex_id
) noexcept {
    return tree[to_idx(vertex_id)].pred_id != invalid_id;
}

template <traits::c_hypergraph H>
[[nodiscard]] gl_attr_force_inline std::vector<search_node<H>> init_node_range(
    typename H::id_type root_vertex_id
) {
    return std::vector<search_node<H>>{search_node<H>{root_vertex_id}};
}

template <traits::c_hypergraph H>
[[nodiscard]] gl_attr_force_inline auto default_visit_predicate(std::vector<bool>& visited_v) {
    return [&](const search_node<H>& node) -> bool {
        return not visited_v[to_idx(node.vertex_id)];
    };
}

template <traits::c_hypergraph H, result_discriminator Result>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited_v, non_void_result_type<Result, search_tree<H>>& pred_map
) {
    return [&](const search_node<H>& node) {
        const auto vertex_idx = to_idx(node.vertex_id);
        visited_v[vertex_idx] = true;
        if constexpr (Result == ret)
            pred_map[vertex_idx] = node;
        return true;
    };
}

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

[[nodiscard]] gl_attr_force_inline auto blocking_traverse_hyperedge_predicate(
    std::vector<size_type>& counter_map
) {
    return [&](traits::c_id_type auto he_id, traits::c_id_type auto /*source_id*/) {
        return static_cast<decision>(--counter_map[to_idx(he_id)] == 0uz);
    };
}

template <traits::c_hypergraph H, bool AsResult = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_predicate(std::vector<bool>& visited_v) {
    using return_t = std::conditional_t<AsResult, decision, bool>;
    return [&](const search_node<H>& node) -> return_t {
        return return_t(not visited_v[to_idx(node.vertex_id)]);
    };
}

} // namespace hgl::algorithm
