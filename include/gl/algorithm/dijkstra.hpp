#pragma once

#include "detail/bfs_impl.hpp"
#include "gl/graph_utility.hpp"
#include "gl/types/properties.hpp"

#include <deque>

namespace gl::algorithm {

template <type_traits::c_basic_arithmetic EdgeWeithType>
struct paths_descriptor {
    using distance_type = EdgeWeithType;

    paths_descriptor(const types::size_type n_vertices)
    : predecessors(n_vertices), distances(n_vertices) {
        predecessors.shrink_to_fit();
        distances.shrink_to_fit();
    }

    [[nodiscard]] gl_attr_force_inline bool is_reachable(const types::id_type vertex_id) const {
        return this->predecessors.at(vertex_id).has_value();
    }

    std::vector<std::optional<types::id_type>> predecessors;
    std::vector<distance_type> distances;
};

namespace detail {

template <type_traits::c_graph GraphType>
using paths_descriptor_type = paths_descriptor<types::vertex_distance_type<GraphType>>;

} // namespace detail

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline detail::paths_descriptor_type<GraphType> make_paths_descriptor(
    const GraphType& graph
) {
    return detail::paths_descriptor_type<GraphType>{graph.n_vertices()};
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
[[nodiscard]] detail::paths_descriptor_type<GraphType> dijkstra_shortest_paths(
    const GraphType& graph,
    const types::id_type source_id,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using distance_type = types::vertex_distance_type<GraphType>;

    auto paths = make_paths_descriptor<GraphType>(graph);

    paths.predecessors.at(source_id).emplace(source_id);
    paths.distances[source_id] = distance_type{};

    constexpr distance_type min_edge_weight_threshold = 0ull;
    std::optional<types::const_ref_wrap<edge_type>> negative_edge;

    detail::pq_bfs_impl(
        graph,
        [&paths](const types::vertex_info& lhs, const types::vertex_info& rhs) {
            return paths.distances[lhs.id] > paths.distances[rhs.id];
        },
        detail::init_range(source_id),
        detail::constant_unary_predicate<true>(), // visit pred
        detail::constant_binary_predicate<true>(), // visit callback
        [&paths, &negative_edge](const vertex_type& vertex, const edge_type& in_edge)
            -> std::optional<bool> { // enqueue pred
            const auto vertex_id = vertex.id();
            const auto source_id = in_edge.incident_vertex(vertex).id();

            const auto edge_weight = get_weight<GraphType>(in_edge);
            if (edge_weight < min_edge_weight_threshold) {
                negative_edge = std::cref(in_edge);
                return std::nullopt;
            }

            const auto new_distance = paths.distances[source_id] + edge_weight;
            if (not paths.predecessors[vertex_id].has_value()
                or new_distance < paths.distances[vertex_id]) {
                paths.distances[vertex_id] = new_distance;
                paths.predecessors[vertex_id].emplace(source_id);
                return true;
            }

            return false;
        }
    );

    if (negative_edge.has_value()) {
        const auto& edge = negative_edge.value().get();
        throw std::invalid_argument(std::format(
            "[alg::dijkstra_shortest_paths] Found an edge with a negative weight: [{}, {} | w={}]",
            edge.first_id(),
            edge.second_id(),
            get_weight<GraphType>(edge)
        ));
    }

    return paths;
}

template <type_traits::c_random_access_range_of<types::id_type> IdRange>
[[nodiscard]] std::deque<types::id_type> reconstruct_path(
    const IdRange& predecessor_map, const types::id_type vertex_id
) {
    std::deque<types::id_type> path;
    types::id_type current_vertex = vertex_id;

    while (true) {
        path.push_front(current_vertex);
        types::id_type predecessor = *(predecessor_map.begin() + current_vertex);

        if (predecessor == current_vertex)
            break;

        current_vertex = predecessor;
    }

    return path;
}

} // namespace gl::algorithm
