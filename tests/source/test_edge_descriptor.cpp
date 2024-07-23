#include <doctest.h>
#include <gl/edge_descriptor.hpp>

using namespace gl;

namespace {

constexpr std::size_t vertex_id_1 = 1111ull;
constexpr std::size_t vertex_id_2 = 2222ull;
constexpr std::size_t vertex_id_3 = 3333ull;

} // namespace

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_descriptor");

struct test_edge_descriptor {
    std::shared_ptr<vertex<>> vd_1 = std::make_shared<vertex<>>(vertex_id_1);
    std::shared_ptr<vertex<>> vd_2 = std::make_shared<vertex<>>(vertex_id_2);
    std::shared_ptr<vertex<>> vd_3 = std::make_shared<vertex<>>(vertex_id_3);
};

TEST_CASE_FIXTURE(
    test_edge_descriptor, "is_directed() should return true only for edges with directed edge tag"
) {
    edge_descriptor<vertex<>, directed_t> directed_edge{vd_1, vd_2};
    CHECK(directed_edge.is_directed());

    edge_descriptor<vertex<>, undirected_t> bidirectional_edge{vd_1, vd_2};
    CHECK_FALSE(bidirectional_edge.is_directed());
}

TEST_CASE_FIXTURE(
    test_edge_descriptor,
    "is_undirected() should return true only for edges with bidirectional edge tag"
) {
    edge_descriptor<vertex<>, undirected_t> bidirectional_edge{vd_1, vd_2};
    CHECK(bidirectional_edge.is_undirected());

    edge_descriptor<vertex<>, directed_t> directed_edge{vd_1, vd_2};
    CHECK_FALSE(directed_edge.is_undirected());
}

TEST_CASE_FIXTURE(
    test_edge_descriptor,
    "incident_vertices() should return the pair of vertices the edge was initialized with"
) {
    edge_descriptor<vertex<>> sut{vd_1, vd_2};

    const auto& vertices = sut.incident_vertices();
    CHECK_EQ(*vertices.first, *vd_1);
    CHECK_EQ(*vertices.second, *vd_2);
}

TEST_CASE_TEMPLATE_DEFINE("incident_vertex() tests", EdgeType, parametric_edge_tag_template) {
    test_edge_descriptor fixture{};

    EdgeType sut{fixture.vd_1, fixture.vd_2};

    SUBCASE("should return nullptr if input vertex is not incident with the edge") {
        CHECK_FALSE(sut.incident_vertex(fixture.vd_3));
    }

    SUBCASE("should return the vertex incident to the input vertex") {
        CHECK_EQ(*sut.incident_vertex(fixture.vd_1), *fixture.vd_2);
        CHECK_EQ(*sut.incident_vertex(fixture.vd_2), *fixture.vd_1);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(parametric_edge_tag_template, directed_edge<vertex<>>, undirected_edge<vertex<>>);

TEST_SUITE_END(); // test_edge_descriptor

} // namespace gl_testing
