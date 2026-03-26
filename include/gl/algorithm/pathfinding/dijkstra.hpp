// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/pfs.hpp"
#include "gl/constants.hpp"

#include <deque>

namespace gl::algorithm {

template <traits::c_arithmetic VertexDistanceType>
struct paths_descriptor {
    paths_descriptor(const size_type n_vertices)
    : predecessors(n_vertices, constants::invalid_id), distances(n_vertices) {}

    predecessors_map predecessors;
    std::vector<VertexDistanceType> distances;
};

template <traits::c_graph GraphType>
using paths_descriptor_type = paths_descriptor<vertex_distance_type<GraphType>>;

template <traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline paths_descriptor_type<GraphType> make_paths_descriptor(
    const GraphType& graph
) {
    return paths_descriptor_type<GraphType>{graph.order()};
}

template <
    traits::c_graph GraphType,
    traits::c_optional_id_callback<void> PreVisitCallback = algorithm::empty_callback,
    traits::c_optional_id_callback<void> PostVisitCallback = algorithm::empty_callback>
[[nodiscard]] paths_descriptor_type<GraphType> dijkstra_shortest_paths(
    const GraphType& graph,
    const id_type source_id,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using edge_type = typename GraphType::edge_type;
    using distance_type = vertex_distance_type<GraphType>;

    auto paths = make_paths_descriptor<GraphType>(graph);

    paths.predecessors[source_id] = source_id;
    paths.distances[source_id] = distance_type{};

    std::optional<edge_type> negative_edge;

    pfs(
        graph,
        [&paths](const algorithm::vertex_info& lhs, const algorithm::vertex_info& rhs) {
            return paths.distances[lhs.id] > paths.distances[rhs.id];
        },
        init_range(source_id),
        algorithm::empty_callback{}, // visit predicate
        algorithm::empty_callback{}, // visit callback
        [&paths, &negative_edge](const id_type vertex_id, const edge_type& in_edge)
            -> predicate_result { // enqueue predicate
            const auto pred_id = in_edge.incident_vertex(vertex_id);

            const auto edge_weight = get_weight<GraphType>(in_edge);
            if (edge_weight < 0) {
                negative_edge.emplace(in_edge);
                return predicate_result::unknown;
            }

            const auto new_distance = paths.distances[pred_id] + edge_weight;
            if (paths.predecessors[vertex_id] == constants::invalid_id
                or new_distance < paths.distances[vertex_id]) {
                paths.distances[vertex_id] = new_distance;
                paths.predecessors[vertex_id] = pred_id;
                return true;
            }

            return false;
        },
        pre_visit,
        post_visit
    );

    if (negative_edge.has_value()) {
        const auto& edge = negative_edge.value();
        throw std::invalid_argument(std::format(
            "[alg::dijkstra_shortest_paths] Found an edge with a negative weight: [{}, {} | w={}]",
            edge.source(),
            edge.target(),
            get_weight<GraphType>(edge)
        ));
    }

    return paths;
}

template <traits::c_random_access_range_of<std::optional<id_type>> IdRange>
[[nodiscard]] std::deque<id_type> reconstruct_path(
    const IdRange& predecessor_map, const id_type vertex_id
) {
    if (not std::ranges::next(predecessor_map.begin(), vertex_id)->has_value())
        throw std::invalid_argument(
            std::format("[alg::reconstruct_path] The given vertex is unreachable: {}", vertex_id)
        );

    std::deque<id_type> path;
    id_type current_vertex = vertex_id;

    while (true) {
        path.push_front(current_vertex);
        id_type predecessor = (predecessor_map.begin() + current_vertex)->value();

        if (predecessor == current_vertex)
            break;

        current_vertex = predecessor;
    }

    path.shrink_to_fit();
    return path;
}

} // namespace gl::algorithm
