#include "constants.hpp"
#include "types.hpp"
#include "utility.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_tags");

struct test_edge_tags {
    using vertex_type = lib::vertex_descriptor<>;

    std::shared_ptr<vertex_type> vd_1 = util::make_vertex<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> vd_2 = util::make_vertex<vertex_type>(constants::vertex_id_2);
};

struct test_directed_edge_tag : test_edge_tags {
    using sut_type = lib::directed_t;
    using edge_type = lib::directed_edge<vertex_type>;

    const std::unique_ptr<edge_type> edge = lib::make_edge<edge_type>(vd_1, vd_2);
};

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "make_edge should return a unique ptr to a directed edge"
) {
    REQUIRE(edge->is_directed());
    REQUIRE_FALSE(edge->is_undirected());

    CHECK_EQ(edge->first(), vd_1);
    CHECK_EQ(edge->second(), vd_2);
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_from should return true only for the first vertex"
) {
    CHECK(sut_type::is_incident_from(*edge, vd_1));
    CHECK_FALSE(sut_type::is_incident_from(*edge, vd_2));
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_to should return true only for the second vertex"
) {
    CHECK(sut_type::is_incident_to(*edge, vd_2));
    CHECK_FALSE(sut_type::is_incident_to(*edge, vd_1));
}

struct test_undirected_edge_tag : test_edge_tags {
    using sut_type = lib::undirected_t;
    using edge_type = lib::undirected_edge<vertex_type>;

    const std::shared_ptr<edge_type> edge = lib::make_edge<edge_type>(vd_1, vd_2);
};

TEST_CASE_FIXTURE(
    test_undirected_edge_tag, "make_edge should return a shared ptr to a directed edge"
) {
    REQUIRE(edge->is_undirected());
    REQUIRE_FALSE(edge->is_directed());

    CHECK_EQ(edge->first(), vd_1);
    CHECK_EQ(edge->second(), vd_2);
}

TEST_CASE_FIXTURE(
    test_undirected_edge_tag, "is_incident_from should return true for both vertices"
) {
    CHECK(sut_type::is_incident_from(*edge, vd_1));
    CHECK(sut_type::is_incident_from(*edge, vd_2));
}

TEST_CASE_FIXTURE(test_undirected_edge_tag, "is_incident_to should return true for both vertices") {
    CHECK(sut_type::is_incident_to(*edge, vd_1));
    CHECK(sut_type::is_incident_to(*edge, vd_2));
}

TEST_SUITE_END(); // untest_edge_tags

} // namespace gl_testing