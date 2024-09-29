#pragma once

#include "detail/bfs_impl.hpp"
#include "gl/types/properties.hpp"

#include <iostream>

namespace gl::algorithm {

template <type_traits::c_basic_arithmetic EdgeWeithType>
struct paths_descriptor {
    using distance_type = EdgeWeithType;

    paths_descriptor(const types::size_type n_vertices)
    : predecessors(n_vertices), distances(n_vertices) {
        predecessors.shrink_to_fit();
        distances.shrink_to_fit();
    }

    std::vector<std::optional<types::id_type>> predecessors;
    std::vector<distance_type> distances;
};

namespace detail {

using default_vertex_distance_type = std::uint64_t;

template <type_traits::c_graph GraphType>
using graph_vertex_distance_type = std::conditional_t<
    type_traits::c_weight_properties_type<typename GraphType::edge_properties_type>,
    typename GraphType::edge_properties_type::weight_type,
    default_vertex_distance_type>;

template <type_traits::c_graph GraphType>
using paths_descriptor_type = paths_descriptor<graph_vertex_distance_type<GraphType>>;

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline auto dijkstra_get_edge_weight() {
    if constexpr (type_traits::c_weight_properties_type<typename GraphType::edge_properties_type>) {
        return [](const typename GraphType::edge_type& edge) { return edge.properties.weight; };
    }
    else {
        return [](const typename GraphType::edge_type& edge) {
            return default_vertex_distance_type{1ull};
        };
    }
}

} // namespace detail

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline detail::paths_descriptor_type<GraphType> make_paths_descriptor(
    const GraphType& graph
) {
    return detail::paths_descriptor_type<GraphType>{graph.n_vertices()};
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = empty_callback>
[[nodiscard]] detail::paths_descriptor_type<GraphType> dijkstra_shortest_paths(
    const GraphType& graph,
    const types::id_type source_id,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using distance_type = typename detail::paths_descriptor_type<GraphType>::distance_type;

    auto paths = make_paths_descriptor<GraphType>(graph);

    paths.predecessors.at(source_id).emplace(source_id);
    paths.distances[source_id] = distance_type{};

    const auto get_edge_weight = detail::dijkstra_get_edge_weight<GraphType>();

    const bool found_negative_weight = detail::pq_bfs_impl(
        graph,
        [&paths](const detail::vertex_info& lhs, const detail::vertex_info& rhs) {
            return paths.distances[lhs.id] > paths.distances[rhs.id];
        },
        detail::init_range(source_id),
        [&paths](const vertex_type& vertex) { // visit pred
            return not paths.predecessors[vertex.id()].has_value();
        },
        detail::constant_binary_predicate<true>(), // visit callback
        [&paths, &get_edge_weight](const vertex_type& vertex, const edge_type& in_edge)
            -> std::optional<bool> { // enqueue pred
            const auto vertex_id = vertex.id();
            const auto source_id = in_edge.incident_vertex(vertex).id();

            const auto edge_weight = get_edge_weight(in_edge);
            if (edge_weight < static_cast<distance_type>(0))
                return std::nullopt;

            const auto new_distance = paths.distances[source_id] + edge_weight;
            if (new_distance < paths.distances[source_id]) {
                paths.distances[vertex_id] = new_distance;
                paths.predecessors[vertex_id].emplace(source_id);
                return true;
            }

            return false;
        }
    );

    if (found_negative_weight)
        throw std::invalid_argument("[alg::dijkstra_shortest_paths] Found an edge with a negative "
                                    "weight");

    return paths;
}

} // namespace gl::algorithm
