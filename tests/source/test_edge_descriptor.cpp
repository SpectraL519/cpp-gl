#include "constants.hpp"
#include "types.hpp"
#include "utility.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_descriptor");

struct test_edge_descriptor {
    using vertex_type = lib::vertex_descriptor<>;

    std::shared_ptr<vertex_type> vd_1 = util::make_vertex<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> vd_2 = util::make_vertex<vertex_type>(constants::vertex_id_2);
    std::shared_ptr<vertex_type> vd_3 = util::make_vertex<vertex_type>(constants::vertex_id_3);
};

TEST_CASE_FIXTURE(
    test_edge_descriptor, "is_directed() should return true only for edges with directed edge tag"
) {
    lib::directed_edge<lib::vertex<>> directed_edge{vd_1, vd_2};
    CHECK(directed_edge.is_directed());

    lib::undirected_edge<lib::vertex<>> undirected_edge{vd_1, vd_2};
    CHECK_FALSE(undirected_edge.is_directed());
}

TEST_CASE_FIXTURE(
    test_edge_descriptor,
    "is_undirected() should return true only for edges with bidirectional edge tag"
) {
    lib::undirected_edge<lib::vertex<>> undirected_edge{vd_1, vd_2};
    CHECK(undirected_edge.is_undirected());

    lib::directed_edge<lib::vertex<>> directed_edge{vd_1, vd_2};
    CHECK_FALSE(directed_edge.is_undirected());
}

TEST_CASE_TEMPLATE_DEFINE(
    "properties should be properly initialized", EdgeType, properties_edge_directional_tag_template
) {
    test_edge_descriptor fixture;

    const types::used_property used{true};
    const EdgeType sut{fixture.vd_1, fixture.vd_2, used};

    CHECK_EQ(sut.properties, used);
}

// TODO: fix .clang-format to split such lines
TEST_CASE_TEMPLATE_INSTANTIATE(properties_edge_directional_tag_template, lib::directed_edge<lib::vertex<>, types::used_property>, lib::undirected_edge<lib::vertex<>, types::used_property>);

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", EdgeType, edge_directional_tag_template
) {
    test_edge_descriptor fixture{};

    EdgeType sut{fixture.vd_1, fixture.vd_2};

    SUBCASE("incident_vertices should return the pair of vertices the edge was initialized with") {
        const auto& vertices = sut.incident_vertices();
        CHECK_EQ(*vertices.first, *fixture.vd_1);
        CHECK_EQ(*vertices.second, *fixture.vd_2);
    }

    SUBCASE("first should return the first vertex descriptor the edge was initialized with") {
        CHECK_EQ(*sut.first(), *fixture.vd_1);
    }

    SUBCASE("second should return the second vertex descriptor the edge was initialized with") {
        CHECK_EQ(*sut.second(), *fixture.vd_2);
    }

    SUBCASE("incident_vertex should return nullptr if input vertex is not incident with the edge") {
        CHECK_FALSE(sut.incident_vertex(fixture.vd_3));
    }

    SUBCASE("incident_vertex should return the vertex incident with the input vertex") {
        CHECK_EQ(*sut.incident_vertex(fixture.vd_1), *fixture.vd_2);
        CHECK_EQ(*sut.incident_vertex(fixture.vd_2), *fixture.vd_1);
    }
}

// TODO: fix .clang-format to split such lines
TEST_CASE_TEMPLATE_INSTANTIATE(edge_directional_tag_template, lib::directed_edge<lib::vertex<>>, lib::undirected_edge<lib::vertex<>>);

TEST_CASE_FIXTURE(test_edge_descriptor, "is_incident_from tests") {
    SUBCASE("[undirected_edge] should return true for both vertices") {
        lib::undirected_edge<lib::vertex<>> sut{vd_1, vd_2};
        CHECK(sut.is_incident_from(vd_1));
        CHECK(sut.is_incident_from(vd_2));
    }

    SUBCASE("[directed_edge] should return true only for the first vertex") {
        lib::directed_edge<lib::vertex<>> sut{vd_1, vd_2};
        CHECK(sut.is_incident_from(vd_1));
        CHECK_FALSE(sut.is_incident_from(vd_2));
    }
}

TEST_CASE_FIXTURE(test_edge_descriptor, "is_incident_to tests") {
    SUBCASE("[undirected_edge] should return true for both vertices") {
        lib::undirected_edge<lib::vertex<>> sut{vd_1, vd_2};
        CHECK(sut.is_incident_to(vd_1));
        CHECK(sut.is_incident_to(vd_2));
    }

    SUBCASE("[directed_edge] should return true only for the second vertex") {
        lib::directed_edge<lib::vertex<>> sut{vd_1, vd_2};
        CHECK(sut.is_incident_to(vd_2));
        CHECK_FALSE(sut.is_incident_to(vd_1));
    }
}

TEST_SUITE_END(); // test_edge_descriptor

} // namespace gl_testing
