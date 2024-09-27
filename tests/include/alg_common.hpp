#pragma once

#include "namespaces.hpp"
#include "types.hpp"

#include <gl/graph.hpp>

namespace gl_testing::alg_common {

template <lib_tt::c_graph GraphType1, lib_tt::c_graph GraphType2>
[[nodiscard]] auto are_vertices_equivalent(const GraphType1& g1, const GraphType2& g2) {
    return [&](const lib_t::id_type vertex_id) {
        const auto adj_edges_1 = g1.adjacent_edges(vertex_id);
        const auto adj_edges_2 = g2.adjacent_edges(vertex_id);

        if (adj_edges_2.distance() != adj_edges_1.distance())
            return false;

        for (const auto& edge : adj_edges_1) {
            if (not g2.has_edge(edge.first_id(), edge.second_id()))
                return false;
        }

        for (const auto& edge : adj_edges_2) {
            if (not g1.has_edge(edge.first_id(), edge.second_id()))
                return false;
        }

        return true;
    };
}

template <lib_tt::c_instantiation_of<lib::vertex_descriptor> VertexType>
requires(std::same_as<typename VertexType::properties_type, types::visited_property>)
struct vertex_visited_projection {
    [[nodiscard]] bool operator()(const VertexType& vertex) const {
        return vertex.properties.visited;
    }
};

} // namespace gl_testing::alg_common
