#include "constants.hpp"
#include "functional.hpp"
#include "types.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_descriptor");

struct test_edge_descriptor {
    using vertex_type = lib::vertex_descriptor<>;

    vertex_type vd_1{constants::vertex_id_1};
    vertex_type vd_2{constants::vertex_id_2};
    vertex_type vd_3{constants::vertex_id_3};

    vertex_type invalid_vd{constants::vertex_id_1};
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
        CHECK_EQ(vertices.first, fixture.vd_1);
        CHECK_EQ(vertices.second, fixture.vd_2);
    }

    SUBCASE("first should return the first vertex descriptor the edge was initialized with") {
        CHECK_EQ(sut.first(), fixture.vd_1);
    }

    SUBCASE("second should return the second vertex descriptor the edge was initialized with") {
        CHECK_EQ(sut.second(), fixture.vd_2);
    }

    SUBCASE("incident_vertex_ids should return the pair of ids of the vertices the edge was "
            "initialized with") {
        const auto& vertex_ids = sut.incident_vertex_ids();
        CHECK_EQ(vertex_ids.first, fixture.vd_1.id());
        CHECK_EQ(vertex_ids.second, fixture.vd_2.id());
    }

    SUBCASE("first_id should return the id of the first vertex descriptor the edge was initialized "
            "with") {
        CHECK_EQ(sut.first_id(), fixture.vd_1.id());
    }

    SUBCASE("second should return the id of the second vertex descriptor the edge was initialized "
            "with") {
        CHECK_EQ(sut.second_id(), fixture.vd_2.id());
    }

    SUBCASE("incident_vertex should throw if input vertex is not incident with the edge") {
        CHECK_THROWS_AS(
            func::discard_result(sut.incident_vertex(fixture.vd_3)), std::invalid_argument
        );
    }

    SUBCASE("incident_vertex should return the vertex incident with the input vertex") {
        CHECK_EQ(sut.incident_vertex(fixture.vd_1), fixture.vd_2);
        CHECK_EQ(sut.incident_vertex(fixture.vd_2), fixture.vd_1);
    }

    SUBCASE("incident_vertex_id should throw if input vertex id is not valid") {
        CHECK_THROWS_AS(
            func::discard_result(sut.incident_vertex_id(fixture.vd_3.id())), std::invalid_argument
        );
    }

    SUBCASE("incident_vertex_id should return the id of the vertex incident with the input vertex"
    ) {
        CHECK_EQ(sut.incident_vertex_id(fixture.vd_1.id()), fixture.vd_2.id());
        CHECK_EQ(sut.incident_vertex_id(fixture.vd_2.id()), fixture.vd_1.id());
    }

    SUBCASE("is_incident_with should return true when the given vertex is one of the connected "
            "vertices") {
        CHECK(sut.is_incident_with(fixture.vd_1));
        CHECK(sut.is_incident_with(fixture.vd_2));

        CHECK_FALSE(sut.is_incident_with(fixture.invalid_vd));
        CHECK_FALSE(sut.is_incident_with(fixture.invalid_vd));
    }

    SUBCASE("is_loop should return true onlyu for edges where both vertices are the same") {
        CHECK_FALSE(sut.is_loop());

        const EdgeType loop{fixture.vd_1, fixture.vd_1};
        CHECK(loop.is_loop());
    }
}

// TODO: fix .clang-format to split such lines
TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_template,
    lib::directed_edge<lib::vertex<>>, // default directed edge
    lib::undirected_edge<lib::vertex<>> // default undirected edge
);

TEST_SUITE_END(); // test_edge_descriptor

} // namespace gl_testing
