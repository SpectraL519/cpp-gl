#include "common_constants.hpp"
#include "types.hpp"

#include <doctest.h>
#include <gl/graph.hpp>

using namespace gl;

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

struct test_graph {
    graph<> sut;
};

TEST_CASE_FIXTURE(test_graph, "graph should be initialized with no vertices by default") {
    CHECK_EQ(sut.no_vertices(), zero_vertices);
}

TEST_CASE("graph constructed with no_vertices parameter should properly initialize the vertex list"
) {
    graph<> sut{no_vertices};
    CHECK_EQ(sut.no_vertices(), no_vertices);

    // TODO: replace with iterator range
    for (types::id_type v_id = 0ull; v_id < no_vertices; v_id++) {
        CHECK_NOTHROW(static_cast<void>(sut.get_vertex(v_id)));
        CHECK_EQ(sut.get_vertex(v_id)->id(), v_id);
    }
}

TEST_CASE_FIXTURE(
    test_graph, "add_vertex should return a vertex_descriptor with an incremented id"
) {
    constexpr types::size_type target_no_vertices = 3ull;

    for (types::id_type v_id = zero_vertices; v_id < target_no_vertices; v_id++) {
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

TEST_CASE_FIXTURE(test_graph, "get_vertex should throw if the given id is invalid") {
    CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(sut.no_vertices())), std::out_of_range);
}

TEST_CASE_FIXTURE(test_graph, "get_vertex should return a vertex with the given id") {
    const auto& added_vertex = sut.add_vertex();
    CHECK_EQ(*sut.get_vertex(added_vertex->id()), *added_vertex);
}

TEST_CASE("remove_vertex should remove the given vertex and align ids of remaining vertices") {
    graph<> sut{no_vertices};
    CHECK_EQ(sut.no_vertices(), no_vertices);

    sut.remove_vertex(sut.get_vertex(vertex_id_1));

    constexpr types::id_type last_vertex_id = no_vertices - one_vertex;
    REQUIRE_THROWS_AS(static_cast<void>(sut.get_vertex(last_vertex_id)), std::out_of_range);

    // TODO: replace with iterator range
    for (types::id_type v_id = vertex_id_1; v_id < last_vertex_id; v_id++) {
        CHECK_NOTHROW(static_cast<void>(sut.get_vertex(v_id)));
        CHECK_EQ(sut.get_vertex(v_id)->id(), v_id);
    }
}

TEST_SUITE_END(); // test_graph

} // namespace gl_testing
