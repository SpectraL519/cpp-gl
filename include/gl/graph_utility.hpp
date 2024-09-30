#pragma once

#include "graph.hpp"

namespace gl {

// --- general utility ---

namespace type_traits {

template <typename T>
concept c_graph = c_instantiation_of<T, graph>;

template <c_graph GraphType>
inline constexpr bool is_directed_v<GraphType> = is_directed_v<typename GraphType::edge_type>;

template <c_graph GraphType>
inline constexpr bool is_undirected_v<GraphType> = is_undirected_v<typename GraphType::edge_type>;

template <typename T>
concept c_directed_graph = c_graph<T> and is_directed_v<T>;

template <typename T>
concept c_undirected_graph = c_graph<T> and is_undirected_v<T>;

} // namespace type_traits

// --- utility associated with graph's elements' properties ---

namespace types {

using default_vertex_distance_type = std::int64_t;

template <type_traits::c_graph GraphType>
using vertex_distance_type = std::conditional_t<
    type_traits::c_weight_properties_type<typename GraphType::edge_properties_type>,
    typename GraphType::edge_properties_type::weight_type,
    default_vertex_distance_type>;

} // namespace types

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline types::vertex_distance_type<GraphType> get_weight(
    const typename GraphType::edge_type& edge
) {
    // TODO: add tests
    if constexpr (type_traits::c_weight_properties_type<typename GraphType::edge_properties_type>)
        return edge.properties.weight;
    else
        return types::default_vertex_distance_type{1ll};
}

} // namespace gl
