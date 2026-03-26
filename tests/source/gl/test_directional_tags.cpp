#include "testing/gl/constants.hpp"
#include "testing/gl/types.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_directional_tags");

struct test_directional_tags {
    static constexpr gl::default_id_type id = 0uz;
    static constexpr gl::default_id_type v1 = constants::v1_id;
    static constexpr gl::default_id_type v2 = constants::v2_id;
};

struct test_directed_edge_tag : test_directional_tags {
    using sut_type = gl::directed_t;
    using edge_type = gl::directed_edge<>;

    const edge_type edge{id, v1, v2};
};

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_from should return true only for the first vertex"
) {
    CHECK(sut_type::is_incident_from(edge, v1));
    CHECK_FALSE(sut_type::is_incident_from(edge, v2));
    CHECK_FALSE(sut_type::is_incident_from(edge, constants::invalid_id_v));
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_to should return true only for the second vertex"
) {
    CHECK(sut_type::is_incident_to(edge, v2));
    CHECK_FALSE(sut_type::is_incident_to(edge, v1));
    CHECK_FALSE(sut_type::is_incident_to(edge, constants::invalid_id_v));
}

struct test_undirected_edge_tag : test_directional_tags {
    using sut_type = gl::undirected_t;
    using edge_type = gl::undirected_edge<>;

    const edge_type edge{id, v1, v2};
};

TEST_CASE_FIXTURE(
    test_undirected_edge_tag, "is_incident_from should return true for both vertices"
) {
    CHECK(sut_type::is_incident_from(edge, v1));
    CHECK(sut_type::is_incident_from(edge, v2));

    CHECK_FALSE(sut_type::is_incident_from(edge, constants::invalid_id_v));
    CHECK_FALSE(sut_type::is_incident_from(edge, constants::invalid_id_v));
}

TEST_CASE_FIXTURE(test_undirected_edge_tag, "is_incident_to should return true for both vertices") {
    CHECK(sut_type::is_incident_to(edge, v1));
    CHECK(sut_type::is_incident_to(edge, v2));

    CHECK_FALSE(sut_type::is_incident_to(edge, constants::invalid_id_v));
    CHECK_FALSE(sut_type::is_incident_to(edge, constants::invalid_id_v));
}

TEST_SUITE_END(); // untest_directional_tags

} // namespace gl_testing
