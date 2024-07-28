#include <doctest.h>
#include <gl/graph.hpp>

using namespace gl;

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

struct test_graph_default_traits {
    graph<> sut;
};

TEST_CASE_FIXTURE(test_graph_default_traits, "graph should be initialized with no vertices by default") {
    CHECK_EQ(sut.no_vertices(), 0ull);
}

TEST_CASE_FIXTURE(test_graph_default_traits, "add_vertex should return a vertex_descriptor with an incremented id") {
    constexpr std::size_t target_no_vertices = 3ull;

    for (std::size_t v_id = 0ull; v_id < target_no_vertices; v_id++) {
        const auto& vertex = sut.add_vertex();
        CHECK_EQ(vertex->id(), v_id);
        CHECK_EQ(sut.no_vertices(), v_id + 1ull);
    }

    CHECK_EQ(sut.no_vertices(), target_no_vertices);
}

TEST_SUITE_END(); // test_graph

} // namespace gl_testing
