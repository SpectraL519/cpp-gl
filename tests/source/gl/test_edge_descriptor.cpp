#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"
#include "testing/gl/types.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_descriptor");

struct test_edge_descriptor {
    using vertex_type = gl::vertex_descriptor<>;

    gl::types::id_type v1 = constants::vertex_id_1;
    gl::types::id_type v2 = constants::vertex_id_2;
    gl::types::id_type v3 = constants::vertex_id_3;
};

TEST_CASE_FIXTURE(
    test_edge_descriptor, "is_directed() should return true only for edges with directed edge tag"
) {
    gl::directed_edge<> directed_edge{v1, v2};
    CHECK(directed_edge.is_directed());

    gl::undirected_edge<> undirected_edge{v1, v2};
    CHECK_FALSE(undirected_edge.is_directed());
}

TEST_CASE_FIXTURE(
    test_edge_descriptor,
    "is_undirected() should return true only for edges with bidirectional edge tag"
) {
    gl::undirected_edge<> undirected_edge{v1, v2};
    CHECK(undirected_edge.is_undirected());

    gl::directed_edge<> directed_edge{v1, v2};
    CHECK_FALSE(directed_edge.is_undirected());
}

TEST_CASE_TEMPLATE_DEFINE(
    "properties should be properly initialized", EdgeType, properties_edge_directional_tag_template
) {
    test_edge_descriptor fixture;

    const types::used_property used{true};
    const EdgeType sut{fixture.v1, fixture.v2, used};

    CHECK_EQ(sut.properties(), used);
}

// TODO: fix .clang-format to split such lines
TEST_CASE_TEMPLATE_INSTANTIATE(properties_edge_directional_tag_template, gl::directed_edge<types::used_property>, gl::undirected_edge<types::used_property>);

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", EdgeType, edge_directional_tag_template
) {
    test_edge_descriptor fixture{};

    EdgeType sut{fixture.v1, fixture.v2};

    SUBCASE("incident_vertices should return the pair of vertex IDS the edge was initialized with"
    ) {
        const auto& vertices = sut.incident_vertices();
        CHECK_EQ(vertices.first, fixture.v1);
        CHECK_EQ(vertices.second, fixture.v2);
    }

    SUBCASE("first should return the first vertex descriptor the edge was initialized with") {
        CHECK_EQ(sut.first(), fixture.v1);
    }

    SUBCASE("second should return the second vertex descriptor the edge was initialized with") {
        CHECK_EQ(sut.second(), fixture.v2);
    }

    SUBCASE("incident_vertex should throw if input vertex is not incident with the edge") {
        CHECK_THROWS_AS(
            func::discard_result(sut.incident_vertex(fixture.v3)), std::invalid_argument
        );
    }

    SUBCASE("incident_vertex should return the vertex incident with the input vertex") {
        CHECK_EQ(sut.incident_vertex(fixture.v1), fixture.v2);
        CHECK_EQ(sut.incident_vertex(fixture.v2), fixture.v1);
    }

    SUBCASE("is_incident_with should return true when the given vertex is one of the connected "
            "vertices") {
        CHECK(sut.is_incident_with(fixture.v1));
        CHECK(sut.is_incident_with(fixture.v2));

        CHECK_FALSE(sut.is_incident_with(constants::invalid_id));
        CHECK_FALSE(sut.is_incident_with(constants::invalid_id));
    }

    SUBCASE("is_loop should return true onlyu for edges where both vertices are the same") {
        CHECK_FALSE(sut.is_loop());

        const EdgeType loop{fixture.v1, fixture.v1};
        CHECK(loop.is_loop());
    }
}

// TODO: fix .clang-format to split such lines
TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_template,
    gl::directed_edge<>, // default directed edge
    gl::undirected_edge<> // default undirected edge
);

TEST_SUITE_END(); // test_edge_descriptor

} // namespace gl_testing
