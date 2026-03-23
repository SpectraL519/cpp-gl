#pragma once

#include <gl/graph.hpp>

#include <doctest.h>

namespace gl_testing::io_common {

template <gl::traits::c_graph GraphType>
void verify_graph_structure(const GraphType& actual, const GraphType& expected) {
    REQUIRE_EQ(actual.order(), expected.order());
    REQUIRE_EQ(actual.size(), expected.size());

    // verify that the edges of the in graph are equivalent to the edges of the out graph
    CHECK(std::ranges::all_of(actual.vertices(), [&](const auto& v_actual) {
        return std::ranges::all_of(actual.adjacent_edges(v_actual), [&](const auto& edge) {
            return expected.has_edge(edge.source(), edge.target());
        });
    }));
}

template <gl::traits::c_graph GraphType>
void verify_vertex_properties(const GraphType& actual, const GraphType& expected) {
    const auto properties_proj = [](const auto& item) { return item.properties(); };

    CHECK(std::ranges::equal(
        actual.vertices(),
        expected.vertices(),
        std::ranges::equal_to{},
        properties_proj,
        properties_proj
    ));
}

template <gl::traits::c_graph GraphType>
void verify_edge_properties(const GraphType& actual, const GraphType& expected) {
    CHECK(std::ranges::all_of(actual.vertices(), [&](const auto& v_actual) {
        return std::ranges::all_of(actual.adjacent_edges(v_actual), [&](const auto& edge) {
            return edge.properties()
                == expected.get_edge(edge.source(), edge.target())->properties();
        });
    }));
}

} // namespace gl_testing::io_common
