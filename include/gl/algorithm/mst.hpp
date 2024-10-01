#pragma once
#pragma once

#include "constants.hpp"
#include "detail/common.hpp"
#include "gl/types/properties.hpp"

#include <queue>
#include <iostream>

namespace gl::algorithm {

template <type_traits::c_graph GraphType>
struct mst_descriptor {
    using graph_type = GraphType;
    using edge_type = typename graph_type::edge_type;
    using weight_type = types::vertex_distance_type<graph_type>;

    mst_descriptor(const types::size_type n_vertices) {
        edges.reserve(n_vertices - 1ull);
    }

    std::vector<types::const_ref_wrap<edge_type>> edges;
    weight_type weight = static_cast<weight_type>(0ll);
};

template <
    type_traits::c_undirected_graph GraphType,
    type_traits::c_vertex_callback<GraphType, void> PreVisitCallback = types::empty_callback,
    type_traits::c_vertex_callback<GraphType, void> PostVisitCallback = types::empty_callback>
[[nodiscard]] mst_descriptor<GraphType> prim_mst(
    const GraphType& graph,
    const std::optional<types::id_type> root_id_opt,
    const PreVisitCallback& pre_visit = {},
    const PostVisitCallback& post_visit = {}
) {
    // type definitions

    using vertex_type = typename GraphType::vertex_type;
    using edge_type = typename GraphType::edge_type;
    using edge_info_type = types::edge_info<edge_type>;
    using distance_type = types::vertex_distance_type<GraphType>;

    struct edge_info_comparator {
        [[nodiscard]] gl_attr_force_inline bool operator()(
            const edge_info_type& lhs, const edge_info_type& rhs
        ) const {
            return get_weight<GraphType>(lhs.edge.get()) > get_weight<GraphType>(rhs.edge.get());
        }
    };

    using queue_type =
        std::priority_queue<edge_info_type, std::vector<edge_info_type>, edge_info_comparator>;

    // prepare the necessary utility
    const auto n_vertices = graph.n_vertices();
    mst_descriptor<GraphType> mst(n_vertices);
    std::vector<bool> visited(n_vertices, false);
    queue_type edge_queue;

    // TODO: replace with edge->incident_vertex_id(id) after it's been added to the edge_descriptor class
    const auto get_other_vertex_id = [](const edge_type& edge, const types::id_type source_id) {
        return edge.first_id() == source_id ? edge.second_id() : edge.first_id();
    };

    // insert the edges adjacent to the root vertex to the queue
    const types::id_type root_id = root_id_opt.value_or(0ull);
    // std::cout << "root: " << root_id << std::endl;

    // std::cout << "initial queue content:";
    for (const auto& edge : graph.adjacent_edges(root_id)) {
        // std::cout << std::format(" [{}, {} | s={}]", edge.first_id(), edge.second_id(), root_id);
        edge_queue.emplace(edge, root_id);
    }
    // std::cout << std::endl;

    // mark the root vertex as visited
    visited[root_id] = true;
    types::size_type n_vertices_in_mst = 1ull;

    // find the mst
    constexpr distance_type min_edge_weight_threshold = 0ull;
    while (n_vertices_in_mst < n_vertices) {
        // std::cout << std::format("loop[n_vertices_in_mst = {}]", n_vertices_in_mst) << std::endl;

        const auto min_edge_info = edge_queue.top();
        edge_queue.pop();

        const auto& min_edge = min_edge_info.edge.get();
        const auto min_weight = get_weight<GraphType>(min_edge);
        // std::cout << std::format("\tmin_edge = [{}, {}, w={}]", min_edge.first_id(), min_edge.second_id(), min_weight) << std::endl;

        if (min_weight < min_edge_weight_threshold)
            throw std::invalid_argument(std::format(
                "[alg::prim_mst] Found an edge with a negative weight: [{}, {} | w={}]",
                min_edge.first_id(),
                min_edge.second_id(),
                min_weight
            ));

        const auto& dest_vertex_id = get_other_vertex_id(min_edge, min_edge_info.source_id);
        // std::cout << "\tdest_vertex_id = " << dest_vertex_id << std::endl;

        if (not visited[dest_vertex_id]) {
            // std::cout << "\tadding min_edge to mst" << std::endl;

            mst.edges.emplace_back(min_edge);
            mst.weight += min_weight;

            // std::cout << "\tnew mst.weight = " << mst.weight << std::endl;

            visited[dest_vertex_id] = true;
            ++n_vertices_in_mst;
        }

        for (const auto& edge : graph.adjacent_edges(dest_vertex_id)) {
            const auto new_dest_vertex_id = get_other_vertex_id(edge, dest_vertex_id);
            if (not visited[new_dest_vertex_id]) {
                // std::cout << std::format("\tpushing edge [{}, {}, d={}] to queue", edge.first_id(), edge.second_id(), new_dest_vertex_id) << std::endl;
                edge_queue.emplace(edge, dest_vertex_id);
            }
        }
    }

    return mst;
}

} // namespace gl::algorithm
