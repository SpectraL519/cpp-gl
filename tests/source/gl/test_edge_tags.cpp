#include "testing/gl/constants.hpp"
#include "testing/gl/types.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_tags");

struct test_edge_tags {
    gl::types::id_type v1 = constants::vertex_id_1;
    gl::types::id_type v2 = constants::vertex_id_2;
};

struct test_directed_edge_tag : test_edge_tags {
    using sut_type = gl::directed_t;
    using edge_type = gl::directed_edge<>;

    const std::unique_ptr<edge_type> edge = gl::detail::make_edge<edge_type>(v1, v2);
};

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "make_edge should return a unique ptr to a directed edge"
) {
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(edge)>, std::unique_ptr<edge_type>>);

    REQUIRE(edge->is_directed());
    REQUIRE_FALSE(edge->is_undirected());

    CHECK_EQ(edge->first(), v1);
    CHECK_EQ(edge->second(), v2);
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag,
    "make_edge should return a unique ptr to a directed edge with the given properties"
) {
    using property_edge_type = gl::directed_edge<types::used_property>;

    const types::used_property used{true};
    const auto property_edge = gl::detail::make_edge<property_edge_type>(v1, v2, used);

    static_assert(std::is_same_v<
                  std::remove_cvref_t<decltype(property_edge)>,
                  std::unique_ptr<property_edge_type>>);

    REQUIRE(property_edge->is_directed());
    REQUIRE_FALSE(property_edge->is_undirected());

    CHECK_EQ(property_edge->first(), v1);
    CHECK_EQ(property_edge->second(), v2);
    CHECK_EQ(property_edge->properties(), used);
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_from should return true only for the first vertex"
) {
    CHECK(sut_type::is_incident_from(*edge, v1));
    CHECK_FALSE(sut_type::is_incident_from(*edge, v2));
    CHECK_FALSE(sut_type::is_incident_from(*edge, constants::invalid_id));
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_to should return true only for the second vertex"
) {
    CHECK(sut_type::is_incident_to(*edge, v2));
    CHECK_FALSE(sut_type::is_incident_to(*edge, v1));
    CHECK_FALSE(sut_type::is_incident_to(*edge, constants::invalid_id));
}

struct test_undirected_edge_tag : test_edge_tags {
    using sut_type = gl::undirected_t;
    using edge_type = gl::undirected_edge<>;

    const std::shared_ptr<edge_type> edge = gl::detail::make_edge<edge_type>(v1, v2);
};

TEST_CASE_FIXTURE(
    test_undirected_edge_tag, "make_edge should return a shared ptr to a directed edge"
) {
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(edge)>, std::shared_ptr<edge_type>>);

    REQUIRE(edge->is_undirected());
    REQUIRE_FALSE(edge->is_directed());

    CHECK_EQ(edge->first(), v1);
    CHECK_EQ(edge->second(), v2);
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag,
    "make_edge should return a shared ptr to a directed edge with the given properties"
) {
    using property_edge_type = gl::undirected_edge<types::used_property>;

    const types::used_property used{true};
    const auto property_edge = gl::detail::make_edge<property_edge_type>(v1, v2, used);

    static_assert(std::is_same_v<
                  std::remove_cvref_t<decltype(property_edge)>,
                  std::shared_ptr<property_edge_type>>);

    REQUIRE(property_edge->is_undirected());
    REQUIRE_FALSE(property_edge->is_directed());

    CHECK_EQ(property_edge->first(), v1);
    CHECK_EQ(property_edge->second(), v2);
    CHECK_EQ(property_edge->properties(), used);
}

TEST_CASE_FIXTURE(
    test_undirected_edge_tag, "is_incident_from should return true for both vertices"
) {
    CHECK(sut_type::is_incident_from(*edge, v1));
    CHECK(sut_type::is_incident_from(*edge, v2));

    CHECK_FALSE(sut_type::is_incident_from(*edge, constants::invalid_id));
    CHECK_FALSE(sut_type::is_incident_from(*edge, constants::invalid_id));
}

TEST_CASE_FIXTURE(test_undirected_edge_tag, "is_incident_to should return true for both vertices") {
    CHECK(sut_type::is_incident_to(*edge, v1));
    CHECK(sut_type::is_incident_to(*edge, v2));

    CHECK_FALSE(sut_type::is_incident_to(*edge, constants::invalid_id));
    CHECK_FALSE(sut_type::is_incident_to(*edge, constants::invalid_id));
}

TEST_SUITE_END(); // untest_edge_tags

} // namespace gl_testing
