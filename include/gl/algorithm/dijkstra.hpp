// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/graph_utility.hpp"
#include "impl/pfs.hpp"

#include <deque>

namespace gl::algorithm {

template <type_traits::c_basic_arithmetic VertexDistanceType>
struct paths_descriptor : public predecessors_descriptor {
    using predecessor_type = typename predecessors_descriptor::predecessor_type;
    using distance_type = VertexDistanceType;

    paths_descriptor(const types::size_type n_vertices)
    : predecessors_descriptor(n_vertices), distances(n_vertices) {
        distances.shrink_to_fit();
    }

    [[nodiscard]] std::pair<const predecessor_type&, const distance_type&> operator[](
        const types::size_type i
    ) const {
        return std::make_pair<const predecessor_type&, const distance_type&>(
            this->predecessors[i], this->distances[i]
        );
    }

    [[nodiscard]] std::pair<predecessor_type&, distance_type&>& operator[](const types::size_type i
    ) {
        return std::make_pair<predecessor_type&, distance_type&>(
            this->predecessors[i], this->distances[i]
        );
    }

    [[nodiscard]] std::pair<const predecessor_type&, const distance_type&>& at(
        const types::size_type i
    ) const {
        return std::make_pair<const predecessor_type&, const distance_type&>(
            this->predecessors.at(i), this->distances.at(i)
        );
    }

    [[nodiscard]] std::pair<predecessor_type&, distance_type&>& at(const types::size_type i) {
        return std::make_pair<predecessor_type&, distance_type&>(
            this->predecessors.at(i), this->distances.at(i)
        );
    }

    std::vector<distance_type> distances;
};

template <type_traits::c_graph GraphType>
using paths_descriptor_type = paths_descriptor<types::vertex_distance_type<GraphType>>;

template <type_traits::c_graph GraphType>
[[nodiscard]] gl_attr_force_inline paths_descriptor_type<GraphType> make_paths_descriptor(
    const GraphType& graph
) {
    return paths_descriptor_type<GraphType>{graph.n_vertices()};
}

template <
    type_traits::c_graph GraphType,
    type_traits::c_optional_vertex_callback<GraphType, void> PreVisitCallback =
        algorithm::empty_callback,
    type_traits::c_optional_vertex_callback<GraphType, void> PostVisitCallback =
        algorithm::empty_callback>
[[nodiscard]] paths_descriptor_type<GraphType> dijkstra_shortest_paths(
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

    std::optional<types::const_ref_wrap<edge_type>> negative_edge;

    impl::pfs(
        graph,
        [&paths](const algorithm::vertex_info& lhs, const algorithm::vertex_info& rhs) {
            return paths.distances[lhs.id] > paths.distances[rhs.id];
        },
        impl::init_range(source_id),
        algorithm::empty_callback{}, // visit predicate
        algorithm::empty_callback{}, // visit callback
        [&paths, &negative_edge](const vertex_type& vertex, const edge_type& in_edge)
            -> std::optional<bool> { // enqueue predicate
            const auto vertex_id = vertex.id();
            const auto source_id = in_edge.incident_vertex(vertex).id();

            const auto edge_weight = get_weight<GraphType>(in_edge);
            if (edge_weight < constants::zero) {
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

template <type_traits::c_random_access_range_of<std::optional<types::id_type>> IdRange>
[[nodiscard]] std::deque<types::id_type> reconstruct_path(
    const IdRange& predecessor_map, const types::id_type vertex_id
) {
    if (not std::ranges::next(predecessor_map.begin(), vertex_id)->has_value())
        throw std::invalid_argument(
            std::format("[alg::reconstruct_path] The given vertex is unreachable: {}", vertex_id)
        );

    std::deque<types::id_type> path;
    types::id_type current_vertex = vertex_id;

    while (true) {
        path.push_front(current_vertex);
        types::id_type predecessor = (predecessor_map.begin() + current_vertex)->value();

        if (predecessor == current_vertex)
            break;

        current_vertex = predecessor;
    }

    path.shrink_to_fit();
    return path;
}

} // namespace gl::algorithm
