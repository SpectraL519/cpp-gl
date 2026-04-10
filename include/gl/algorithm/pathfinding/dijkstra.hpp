// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/templates/pfs.hpp"
#include "gl/algorithm/util.hpp"
#include "gl/constants.hpp"
#include "gl/types/core.hpp"

#include <deque>

namespace gl::algorithm {

template <traits::c_graph G, traits::c_arithmetic VertexDistanceType>
struct paths_descriptor {
    using id_type = typename G::id_type;
    using distance_type = VertexDistanceType;

    paths_descriptor(const size_type n_vertices)
    : predecessors(n_vertices, invalid_id), distances(n_vertices) {}

    predecessors_map<G> predecessors;
    std::vector<distance_type> distances;
};

template <traits::c_graph G>
using paths_descriptor_type = paths_descriptor<G, vertex_distance_type<G>>;

template <traits::c_graph G>
[[nodiscard]] gl_attr_force_inline paths_descriptor_type<G> make_paths_descriptor(const G& graph) {
    return paths_descriptor_type<G>{graph.order()};
}

template <
    traits::c_graph G,
    traits::c_optional_callback<void, typename G::id_type> PreVisitCallback = empty_callback,
    traits::c_optional_callback<void, typename G::id_type> PostVisitCallback = empty_callback>
[[nodiscard]] paths_descriptor_type<G> dijkstra_shortest_paths(
    const G& graph,
    typename G::id_type source_id,
    PreVisitCallback pre_visit = {},
    PostVisitCallback post_visit = {}
) {
    using id_type = typename G::id_type;
    using edge_type = typename G::edge_type;
    using distance_type = vertex_distance_type<G>;

    auto paths = make_paths_descriptor<G>(graph);

    paths.predecessors[source_id] = source_id;
    paths.distances[source_id] = distance_type{};

    std::optional<edge_type> negative_edge;

    pfs(
        graph,
        [&paths](const search_node<G>& lhs, const search_node<G>& rhs) {
            return paths.distances[lhs.vertex_id] > paths.distances[rhs.vertex_id];
        },
        init_range<G>(source_id),
        empty_callback{}, // visit predicate
        empty_callback{}, // visit callback
        [&paths, &negative_edge](id_type vertex_id, const edge_type& in_edge)
            -> decision { // enqueue predicate
            const auto pred_id = in_edge.other(vertex_id);

            const auto edge_weight = get_weight<G>(in_edge);
            if (edge_weight < 0) {
                negative_edge.emplace(in_edge);
                return decision::abort;
            }

            const auto new_distance = paths.distances[to_idx(pred_id)] + edge_weight;
            auto& v_pred = paths.predecessors[to_idx(vertex_id)];
            auto& v_dist = paths.distances[to_idx(vertex_id)];

            if (v_pred == invalid_id or new_distance < v_dist) {
                v_dist = new_distance;
                v_pred = pred_id;
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
            get_weight<G>(edge)
        ));
    }

    return paths;
}

template <traits::c_id_type IdType, traits::c_random_access_range_of<IdType> IdRange>
[[nodiscard]] std::deque<IdType> reconstruct_path(
    const IdRange& predecessor_map, const IdType vertex_id
) {
    if (not is_reachable(predecessor_map, vertex_id))
        throw std::invalid_argument(
            std::format("[alg::reconstruct_path] The given vertex is unreachable: {}", vertex_id)
        );

    std::deque<IdType> path;
    IdType current_vertex = vertex_id;

    while (true) {
        path.push_front(current_vertex);
        IdType predecessor = predecessor_map[to_idx(current_vertex)];
        if (predecessor == current_vertex)
            break;
        current_vertex = predecessor;
    }

    path.shrink_to_fit();
    return path;
}

} // namespace gl::algorithm
