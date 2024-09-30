#pragma once

#include "gl/algorithm/types.hpp"
#include "gl/graph.hpp"

namespace gl::algorithm::detail {

// --- common types ---
// ? move to algorithm/types

struct vertex_info {
    vertex_info(types::id_type id) : id(id), source_id(id) {}

    vertex_info(types::id_type id, types::id_type source_id) : id(id), source_id(source_id) {}

    // if id == source_id then vertex_id is the id of the starting vertex
    types::id_type id;
    types::id_type source_id;
};

template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
struct edge_info {
    using edge_type = EdgeType;

    types::const_ref_wrap<edge_type> edge;
    types::id_type source_id;
};

// --- common functions ---

template <type_traits::c_alg_return_graph_type SearchTreeType, type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline SearchTreeType init_search_tree(const GraphType& graph) {
    if constexpr (type_traits::c_alg_no_return_type<SearchTreeType>)
        return SearchTreeType{};
    else
        return SearchTreeType(graph.n_vertices());
}

template <type_traits::c_sized_range_of<vertex_info> InitRangeType = std::vector<vertex_info>>
[[nodiscard]] gl_attr_force_inline InitRangeType init_range(types::id_type root_vertex_id) {
    return InitRangeType{vertex_info{root_vertex_id}};
}

template <bool B>
[[nodiscard]] gl_attr_force_inline auto constant_unary_predicate() {
    return [](const auto&) { return B; };
}

template <bool B>
[[nodiscard]] gl_attr_force_inline auto constant_binary_predicate() {
    return [](const auto&, const auto&) { return B; };
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

} // namespace gl::algorithm::detail
