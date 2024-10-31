// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/types.hpp"

namespace gl::algorithm::impl {

// --- common functions ---

template <type_traits::c_alg_return_graph_type SearchTreeType, type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline SearchTreeType init_search_tree(const GraphType& graph) {
    if constexpr (type_traits::c_alg_no_return_type<SearchTreeType>)
        return SearchTreeType{};
    else
        return SearchTreeType(graph.n_vertices());
}

template <
    type_traits::c_sized_range_of<types::vertex_info> InitRangeType =
        std::vector<types::vertex_info>>
[[nodiscard]] gl_attr_force_inline InitRangeType init_range(types::id_type root_vertex_id) {
    return InitRangeType{types::vertex_info{root_vertex_id}};
}

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline auto default_visit_vertex_predicate(std::vector<bool>& visited) {
    return [&](const typename GraphType::vertex_type& vertex) -> bool {
        return not visited[vertex.id()];
    };
}

template <type_traits::c_graph GraphType, type_traits::c_alg_return_graph_type SearchTreeType>
[[nodiscard]] gl_attr_force_inline auto default_visit_callback(
    std::vector<bool>& visited, SearchTreeType& search_tree
) {
    return [&](const typename GraphType::vertex_type& vertex, const types::id_type source_id) {
        const auto vertex_id = vertex.id();
        visited[vertex_id] = true;
        if constexpr (not type_traits::c_alg_no_return_type<SearchTreeType>) {
            if (source_id != vertex_id)
                search_tree.add_edge(source_id, vertex_id);
        }
        return true;
    };
}

template <type_traits::c_graph GraphType, bool AsOptional = false>
[[nodiscard]] gl_attr_force_inline auto default_enqueue_vertex_predicate(std::vector<bool>& visited
) {
    using return_type = std::conditional_t<AsOptional, std::optional<bool>, bool>;

    return [&](const typename GraphType::vertex_type& vertex,
               const typename GraphType::edge_type& in_edge) -> return_type {
        return not visited[vertex.id()];
    };
}

} // namespace gl::algorithm::impl
