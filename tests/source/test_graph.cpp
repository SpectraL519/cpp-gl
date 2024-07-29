#include "types.hpp"

#include <doctest.h>
#include <gl/graph.hpp>

using namespace gl;

namespace {

constexpr types::size_type no_vertices = 0ull;
constexpr types::size_type one_vertex = 1ull;

} // namespace

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

struct test_graph_default_traits {
    graph<> sut;
};

TEST_CASE_FIXTURE(
    test_graph_default_traits, "graph should be initialized with no vertices by default"
) {
    CHECK_EQ(sut.no_vertices(), no_vertices);
}

TEST_CASE_FIXTURE(
    test_graph_default_traits, "add_vertex should return a vertex_descriptor with an incremented id"
) {
    constexpr types::size_type target_no_vertices = 3ull;

    for (types::id_type v_id = no_vertices; v_id < target_no_vertices; v_id++) {
        const auto& vertex = sut.add_vertex();
        CHECK_EQ(vertex->id(), v_id);
        CHECK_EQ(sut.no_vertices(), v_id + one_vertex);
    }

    CHECK_EQ(sut.no_vertices(), target_no_vertices);
}

TEST_CASE("add_vertex should initialize a new vertex with the input properties structure") {
    using traits_type = graph_traits<directed_t, vertex_types::visited_property>;

    graph<traits_type> sut;
    const vertex_types::visited_property visited{true};

    const auto& vertex = sut.add_vertex(visited);
    REQUIRE_EQ(sut.no_vertices(), one_vertex);

    CHECK_EQ(vertex->id(), 0ull);
    CHECK_EQ(vertex->properties, visited);
}

TEST_SUITE_END(); // test_graph

} // namespace gl_testing
