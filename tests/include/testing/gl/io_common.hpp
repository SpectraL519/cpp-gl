#pragma once

#include "namespaces.hpp"

#include <gl/graph_utility.hpp>

#include <doctest.h>

namespace gl_testing::io_common {

template <lib_tt::c_graph GraphType>
void verify_graph_structure(const GraphType& actual, const GraphType& expected) {
    REQUIRE_EQ(actual.n_vertices(), expected.n_vertices());
    REQUIRE_EQ(actual.n_unique_edges(), expected.n_unique_edges());

    // verify that the edges of the in graph are equivalent to the edges of the out graph
    CHECK(std::ranges::all_of(actual.vertices(), [&](const auto& v_actual) {
        return std::ranges::all_of(actual.adjacent_edges(v_actual), [&](const auto& edge) {
            return expected.has_edge(edge.first_id(), edge.second_id());
        });
    }));
}

template <lib_tt::c_graph GraphType>
void verify_vertex_properties(const GraphType& actual, const GraphType& expected) {
    const auto properties_proj = [](const auto& vertex) { return vertex.properties; };

    CHECK(std::ranges::equal(
        actual.vertices(),
        expected.vertices(),
        std::ranges::equal_to{},
        properties_proj,
        properties_proj
    ));
}

template <lib_tt::c_graph GraphType>
void verify_edge_properties(const GraphType& actual, const GraphType& expected) {
    const auto properties_proj = [](const auto& vertex) { return vertex.properties; };

    CHECK(std::ranges::all_of(actual.vertices(), [&](const auto& v_actual) {
        return std::ranges::all_of(actual.adjacent_edges(v_actual), [&](const auto& edge) {
            // get edge returns optional<ref_wrap>
            return edge.properties
                == expected.get_edge(edge.first_id(), edge.second_id()).value().get().properties;
        });
    }));
}

} // namespace gl_testing::io_common
