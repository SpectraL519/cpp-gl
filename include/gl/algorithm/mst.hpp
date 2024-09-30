#pragma once
#pragma once

#include "detail/bfs_impl.hpp"
#include "detail/common.hpp"
#include "gl/types/properties.hpp"

namespace gl::algorithm {

template <type_traits::c_basic_arithmetic EdgeWeithType>
struct mst_descriptor {
    using distance_type = EdgeWeithType;

    mst_descriptor(const types::size_type n_vertices)
    : predecessors(n_vertices), distances(n_vertices) {
        predecessors.shrink_to_fit();
        distances.shrink_to_fit();
    }
};

template <
    type_traits::c_graph MinimumSpanningTreeType,
    type_traits::c_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
[[nodiscard]] MinimumSpanningTreeType prim_mst(
    const GraphType& graph,
    const types::id_type source_id,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using distance_type = detail::vertex_distance_type<GraphType>;

    detail::pq_bfs_impl(
        graph,
        [&paths](const detail::vertex_info& lhs, const detail::vertex_info& rhs) {
            return paths.distances[lhs.id] > paths.distances[rhs.id];
        },
        detail::init_range(source_id),
        detail::constant_unary_predicate<true>(), // visit pred
        detail::constant_binary_predicate<true>(), // visit callback
        [&paths, &get_edge_weight, &negative_edge](
            const vertex_type& vertex, const edge_type& in_edge
        ) -> std::optional<bool> { // enqueue pred
            const auto vertex_id = vertex.id();
            const auto source_id = in_edge.incident_vertex(vertex).id();

            const auto edge_weight = get_edge_weight(in_edge);
            if (edge_weight < static_cast<distance_type>(0ll)) {
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
            edge.properties.weight
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
