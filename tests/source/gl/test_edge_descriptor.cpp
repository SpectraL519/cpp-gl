#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"
#include "testing/gl/types.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_descriptor");

struct test_edge_descriptor {
    using vertex_type = gl::vertex_descriptor<>;

    static constexpr gl::id_type id1 = 0uz;
    static constexpr gl::id_type id2 = id1 + 1uz;

    static constexpr gl::id_type v1 = constants::v1_id;
    static constexpr gl::id_type v2 = constants::v2_id;
    static constexpr gl::id_type v3 = constants::v3_id;
};

TEST_CASE_FIXTURE(
    test_edge_descriptor, "is_directed() should return true only for edges with directed edge tag"
) {
    gl::directed_edge<> directed_edge{id1, v1, v2};
    CHECK(directed_edge.is_directed());

    gl::undirected_edge<> undirected_edge{id2, v1, v2};
    CHECK_FALSE(undirected_edge.is_directed());
}

TEST_CASE_FIXTURE(
    test_edge_descriptor,
    "is_undirected() should return true only for edges with bidirectional edge tag"
) {
    gl::undirected_edge<> undirected_edge{id1, v1, v2};
    CHECK(undirected_edge.is_undirected());

    gl::directed_edge<> directed_edge{id2, v1, v2};
    CHECK_FALSE(directed_edge.is_undirected());
}

TEST_CASE_FIXTURE(
    test_edge_descriptor, "edges should be equal only if their IDs and vertices are the same"
) {
    // directed edges
    gl::directed_edge<> dedge1{id1, v1, v2};
    gl::directed_edge<> dedge2{id1, v1, v2};
    gl::directed_edge<> dedge3{id2, v1, v2};
    gl::directed_edge<> dedge4{id1, v2, v1};

    CHECK_EQ(dedge1, dedge2);
    CHECK_NE(dedge1, dedge3);
    CHECK_NE(dedge1, dedge4);

    // undirected edges
    gl::undirected_edge<> uedge1{id1, v1, v2};
    gl::undirected_edge<> uedge2{id1, v1, v2};
    gl::undirected_edge<> uedge3{id2, v1, v2};
    gl::undirected_edge<> uedge4{id1, v2, v1};

    CHECK_EQ(uedge1, uedge2);
    CHECK_NE(uedge1, uedge3);
    CHECK_EQ(uedge1, uedge4);
}

TEST_CASE_TEMPLATE_DEFINE(
    "properties accessing tests", EdgeType, properties_directional_tag_template
) {
    test_edge_descriptor fixture;
    used_property used{true};

    SUBCASE("properties should be properly initialized for valid edges") {
        const EdgeType sut{fixture.id1, fixture.v1, fixture.v2, used};
        CHECK_EQ(sut.properties(), used);
    }

    SUBCASE("accessing properties should throw for invalid edges") {
        CHECK_THROWS_AS(
            discard_result(
                EdgeType(constants::invalid_id, fixture.v1, fixture.v2, used).properties()
            ),
            std::logic_error
        );

        CHECK_THROWS_AS(
            discard_result(
                EdgeType(fixture.id1, constants::invalid_id, fixture.v2, used).properties()
            ),
            std::logic_error
        );

        CHECK_THROWS_AS(
            discard_result(
                EdgeType(fixture.id1, fixture.v1, constants::invalid_id, used).properties()
            ),
            std::logic_error
        );

        CHECK_THROWS_AS(discard_result(EdgeType::invalid().properties()), std::logic_error);
    }
}

// TODO: fix .clang-format to split such lines
TEST_CASE_TEMPLATE_INSTANTIATE(properties_directional_tag_template, gl::directed_edge<used_property>, gl::undirected_edge<used_property>);

TEST_CASE_TEMPLATE_DEFINE("directional_tag-independent tests", EdgeType, directional_tag_template) {
    test_edge_descriptor fixture{};
    EdgeType sut{fixture.id1, fixture.v1, fixture.v2};

    SUBCASE("an edge should be valid if it has a valid ID and vertices") {
        CHECK(sut.is_valid());
        CHECK_FALSE(EdgeType{constants::invalid_id, constants::invalid_id, constants::invalid_id});
        CHECK_FALSE(EdgeType{constants::invalid_id, fixture.v1, fixture.v2});
        CHECK_FALSE(EdgeType{fixture.id1, constants::invalid_id, fixture.v2});
        CHECK_FALSE(EdgeType{fixture.id1, fixture.v1, constants::invalid_id});
    }

    SUBCASE("id() should return the ID of the edge") {
        CHECK_EQ(sut.id(), fixture.id1);
    }

    SUBCASE("incident_vertices should return the pair of vertex IDS the edge was initialized with"
    ) {
        const auto& vertices = sut.incident_vertices();
        CHECK_EQ(vertices.first, fixture.v1);
        CHECK_EQ(vertices.second, fixture.v2);
    }

    SUBCASE("incident_vertices_r should return the pair of vertex IDS the edge was initialized "
            "with but with switched order") {
        const auto& vertices = sut.incident_vertices_r();
        CHECK_EQ(vertices.first, fixture.v2);
        CHECK_EQ(vertices.second, fixture.v1);
    }

    SUBCASE("first should return the first vertex descriptor the edge was initialized with") {
        CHECK_EQ(sut.source(), fixture.v1);
    }

    SUBCASE("second should return the second vertex descriptor the edge was initialized with") {
        CHECK_EQ(sut.target(), fixture.v2);
    }

    SUBCASE("incident_vertex should throw if input vertex is not incident with the edge") {
        CHECK_THROWS_AS(discard_result(sut.incident_vertex(fixture.v3)), std::invalid_argument);
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

        const EdgeType loop{fixture.id1, fixture.v1, fixture.v1};
        CHECK(loop.is_loop());
    }
}

// TODO: fix .clang-format to split such lines
TEST_CASE_TEMPLATE_INSTANTIATE(
    directional_tag_template,
    gl::directed_edge<>, // default directed edge
    gl::undirected_edge<> // default undirected edge
);

TEST_SUITE_END(); // test_edge_descriptor

} // namespace gl_testing
