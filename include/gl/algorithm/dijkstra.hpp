#pragma once

#include "detail/bfs_impl.hpp"
#include "gl/types/properties.hpp"

#include <iostream>

namespace gl::algorithm {

// ? what if the graph is not fully connected
// * predecessors -> vector<opt<id>>
// * error
// * + connected/discovered/... -> vector<bool>

template <type_traits::c_basic_arithmetic EdgeWeithType>
struct paths_descriptor {
    using distance_type = EdgeWeithType;

    paths_descriptor(const types::size_type n_vertices)
    : predecessors(n_vertices), distances(n_vertices) {
        predecessors.shrink_to_fit();
        distances.shrink_to_fit();
    }

    std::vector<types::id_type> predecessors;
    std::vector<distance_type> distances;
};

namespace detail {

template <type_traits::c_graph GraphType>
using graph_vertex_distance_type = std::conditional_t<
    type_traits::c_weight_properties_type<typename GraphType::edge_properties_type>,
    typename GraphType::edge_properties_type::weight_type,
    std::uint64_t>;

template <type_traits::c_graph GraphType>
using paths_descriptor_type = paths_descriptor<graph_vertex_distance_type<GraphType>>;

// TODO: visit_pred/visit/enqueue_pred depending on the properties type

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
void dijkstra_shortest_paths(
    const GraphType& graph,
    const types::id_type source_id,
    detail::paths_descriptor_type<GraphType>& paths,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
}

} // namespace gl::algorithm
