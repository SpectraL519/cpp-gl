#pragma once

#include "namespaces.hpp"

#include <gl/graph.hpp>

#include <doctest.h>

namespace gl_testing::io_common {

template <lib_tt::c_graph GraphType>
void verify_graph_structure(const GraphType& g1, const GraphType& g2) {
    REQUIRE_EQ(g1.n_vertices(), g2.n_vertices());
    REQUIRE_EQ(g1.n_unique_edges(), g2.n_unique_edges());

    // verify that the edges of the in graph are equivalent to the edges of the out graph
    CHECK(std::ranges::all_of(g1.vertices(), [&](const auto& v1) {
        return std::ranges::all_of(g1.adjacent_edges(v1), [&](const auto& edge) {
            return g2.has_edge(edge.first_id(), edge.second_id());
        });
    }));
}

template <lib_tt::c_graph GraphType>
void verify_vertex_properties(const GraphType& g1, const GraphType& g2) {
    const auto properties_proj = [](const auto& vertex) { return vertex.properties; };

    CHECK(std::ranges::equal(
        g1.vertices(), g2.vertices(), std::ranges::equal_to{}, properties_proj, properties_proj
    ));
}

template <lib_tt::c_graph GraphType>
void verify_edge_properties(const GraphType& g1, const GraphType& g2) {
    const auto properties_proj = [](const auto& vertex) { return vertex.properties; };

    CHECK(std::ranges::all_of(g1.vertices(), [&](const auto& v1) {
        return std::ranges::all_of(g1.adjacent_edges(v1), [&](const auto& edge) {
            // get edge returns optional<ref_wrap>
            return edge.properties
                == g2.get_edge(edge.first_id(), edge.second_id()).value().get().properties;
        });
    }));
}

} // namespace gl_testing::io_common
