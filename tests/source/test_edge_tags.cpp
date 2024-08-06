#include "constants.hpp"
#include "types.hpp"

#include <gl/edge_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_tags");

struct test_edge_tags {
    using vertex_type = lib::vertex<>;

    std::shared_ptr<vertex_type> vd_1 = std::make_shared<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> vd_2 = std::make_shared<vertex_type>(constants::vertex_id_2);

    std::shared_ptr<lib::directed_edge<vertex_type>> directed_edge =
        std::make_shared<lib::directed_edge<vertex_type>>(vd_1, vd_2);
    std::shared_ptr<lib::undirected_edge<vertex_type>> undirected_edge =
        std::make_shared<lib::undirected_edge<vertex_type>>(vd_1, vd_2);
};

TEST_CASE_FIXTURE(test_edge_tags, "is_directed should return true only for directed edges") {
    CHECK(lib::is_directed(directed_edge));
    CHECK_FALSE(lib::is_directed(undirected_edge));
}

TEST_CASE_FIXTURE(test_edge_tags, "is_undirected should return true only for undirected edges") {
    CHECK(lib::is_undirected(undirected_edge));
    CHECK_FALSE(lib::is_undirected(directed_edge));
}

TEST_CASE_FIXTURE(test_edge_tags, "directed_t method tests") {
    SUBCASE("is_incident_from should return true only for the first vertex") {
        CHECK(lib::directed_t::is_incident_from(*directed_edge, vd_1));
        CHECK_FALSE(lib::directed_t::is_incident_from(*directed_edge, vd_2));
    }

    SUBCASE("is_incident_to should return true only for the second vertex") {
        CHECK(lib::directed_t::is_incident_to(*directed_edge, vd_2));
        CHECK_FALSE(lib::directed_t::is_incident_to(*directed_edge, vd_1));
    }
}

TEST_CASE_FIXTURE(test_edge_tags, "undirected_t method tests") {
    SUBCASE("is_incident_from should return true for both vertices") {
        CHECK(lib::undirected_t::is_incident_from(*undirected_edge, vd_1));
        CHECK(lib::undirected_t::is_incident_from(*undirected_edge, vd_2));
    }

    SUBCASE("is_incident_to should return true for both vertices") {
        CHECK(lib::undirected_t::is_incident_to(*undirected_edge, vd_1));
        CHECK(lib::undirected_t::is_incident_to(*undirected_edge, vd_2));
    }
}

TEST_SUITE_END(); // untest_edge_tags

} // namespace gl_testing
