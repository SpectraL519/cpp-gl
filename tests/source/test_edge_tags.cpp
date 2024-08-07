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

TEST_CASE_FIXTURE(test_edge_tags, "is_directed should return true only for directed edges") {
    const auto directed_edge_ptr = lib::make_edge<lib::directed_edge<vertex_type>>(vd_1, vd_2);
    CHECK(lib::is_directed(*directed_edge_ptr));

    const auto directed_properties_edge_ptr =
        lib::make_edge<lib::directed_edge<vertex_type, types::used_property>>(vd_1, vd_2, {});
    CHECK(lib::is_directed(*directed_properties_edge_ptr));

    const auto undirected_edge_ptr = lib::make_edge<lib::undirected_edge<vertex_type>>(vd_1, vd_2);
    CHECK_FALSE(lib::is_directed(*undirected_edge_ptr));

    const auto undirected_properties_edge_ptr =
        lib::make_edge<lib::undirected_edge<vertex_type, types::used_property>>(vd_1, vd_2, {});
    CHECK_FALSE(lib::is_directed(*undirected_properties_edge_ptr));
}

TEST_CASE_FIXTURE(test_edge_tags, "is_undirected should return true only for undirected edges") {
    const auto undirected_edge_ptr = lib::make_edge<lib::undirected_edge<vertex_type>>(vd_1, vd_2);
    CHECK(lib::is_undirected(*undirected_edge_ptr));

    const auto undirected_properties_edge_ptr =
        lib::make_edge<lib::undirected_edge<vertex_type, types::used_property>>(vd_1, vd_2, {});
    CHECK(lib::is_undirected(*undirected_properties_edge_ptr));

    const auto directed_edge_ptr = lib::make_edge<lib::directed_edge<vertex_type>>(vd_1, vd_2);
    CHECK_FALSE(lib::is_undirected(*directed_edge_ptr));

    const auto directed_properties_edge_ptr =
        lib::make_edge<lib::directed_edge<vertex_type, types::used_property>>(vd_1, vd_2, {});
    CHECK_FALSE(lib::is_undirected(*directed_properties_edge_ptr));
}

struct test_directed_edge_tag : test_edge_tags {
    using sut_type = lib::directed_t;
    using edge_type = lib::directed_edge<vertex_type>;

    const std::unique_ptr<edge_type> edge_ptr = lib::make_edge<edge_type>(vd_1, vd_2);
};

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "make_edge should return a unique ptr to a directed edge"
) {
    REQUIRE(lib::is_directed(*edge_ptr));
    REQUIRE_FALSE(lib::is_undirected(*edge_ptr));

    CHECK_EQ(edge_ptr->first(), vd_1);
    CHECK_EQ(edge_ptr->second(), vd_2);
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_from should return true only for the first vertex"
) {
    CHECK(sut_type::is_incident_from(*edge_ptr, vd_1));
    CHECK_FALSE(sut_type::is_incident_from(*edge_ptr, vd_2));
}

TEST_CASE_FIXTURE(
    test_directed_edge_tag, "is_incident_to should return true only for the second vertex"
) {
    CHECK(sut_type::is_incident_to(*edge_ptr, vd_2));
    CHECK_FALSE(sut_type::is_incident_to(*edge_ptr, vd_1));
}

struct test_undirected_edge_tag : test_edge_tags {
    using sut_type = lib::undirected_t;
    using edge_type = lib::undirected_edge<vertex_type>;

    const std::shared_ptr<edge_type> edge_ptr = lib::make_edge<edge_type>(vd_1, vd_2);
};

TEST_CASE_FIXTURE(
    test_undirected_edge_tag, "make_edge should return a shared ptr to a directed edge"
) {
    REQUIRE(lib::is_undirected(*edge_ptr));
    REQUIRE_FALSE(lib::is_directed(*edge_ptr));

    CHECK_EQ(edge_ptr->first(), vd_1);
    CHECK_EQ(edge_ptr->second(), vd_2);
}

TEST_CASE_FIXTURE(
    test_undirected_edge_tag, "is_incident_from should return true for both vertices"
) {
    CHECK(sut_type::is_incident_from(*edge_ptr, vd_1));
    CHECK(sut_type::is_incident_from(*edge_ptr, vd_2));
}

TEST_CASE_FIXTURE(test_undirected_edge_tag, "is_incident_to should return true for both vertices") {
    CHECK(sut_type::is_incident_to(*edge_ptr, vd_1));
    CHECK(sut_type::is_incident_to(*edge_ptr, vd_2));
}

TEST_SUITE_END(); // untest_edge_tags

} // namespace gl_testing
