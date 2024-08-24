#include "constants.hpp"

#include <gl/incidence.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_incidence");

struct test_incidence {
    using vertex_type = lib::vertex_descriptor<>;

    std::shared_ptr<vertex_type> vd_1 = std::make_shared<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> vd_2 = std::make_shared<vertex_type>(constants::vertex_id_2);
    std::shared_ptr<vertex_type> vd_3 = std::make_shared<vertex_type>(constants::vertex_id_3);

    std::shared_ptr<vertex_type> invalid_vd_1 =
        std::make_shared<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> invalid_vd_2 =
        std::make_shared<vertex_type>(constants::vertex_id_2);
};

TEST_CASE_TEMPLATE_DEFINE("incidence functions tests", GraphType, graph_type_template) {
    test_incidence fixture;

    SUBCASE("are_incident(vertex and edge pair) should return true only when the edge and the "
            "vertex are incident with each other") {
        const auto edge = lib::make_edge<typename GraphType::edge_type>(fixture.vd_1, fixture.vd_2);

        CHECK(lib::are_incident<GraphType>(fixture.vd_1, edge));
        CHECK(lib::are_incident<GraphType>(fixture.vd_2, edge));

        CHECK(lib::are_incident<GraphType>(edge, fixture.vd_1));
        CHECK(lib::are_incident<GraphType>(edge, fixture.vd_2));

        CHECK_FALSE(lib::are_incident<GraphType>(fixture.invalid_vd_1, edge));
        CHECK_FALSE(lib::are_incident<GraphType>(fixture.invalid_vd_2, edge));

        CHECK_FALSE(lib::are_incident<GraphType>(edge, fixture.invalid_vd_1));
        CHECK_FALSE(lib::are_incident<GraphType>(edge, fixture.invalid_vd_2));
    }

    SUBCASE("are_incident(edge, edge) should return true only when the edges share a common vertex"
    ) {
        const auto edge_1 =
            lib::make_edge<typename GraphType::edge_type>(fixture.vd_1, fixture.vd_2);
        const auto edge_2 =
            lib::make_edge<typename GraphType::edge_type>(fixture.vd_2, fixture.vd_3);
        const auto edge_3 =
            lib::make_edge<typename GraphType::edge_type>(fixture.vd_3, fixture.vd_1);

        CHECK(lib::are_incident<GraphType>(edge_1, edge_2));
        CHECK(lib::are_incident<GraphType>(edge_2, edge_3));
        CHECK(lib::are_incident<GraphType>(edge_3, edge_1));

        const auto invalid_edge = lib::make_edge<typename GraphType::edge_type>(
            fixture.invalid_vd_1, fixture.invalid_vd_2
        );

        CHECK_FALSE(lib::are_incident<GraphType>(edge_1, invalid_edge));
        CHECK_FALSE(lib::are_incident<GraphType>(edge_2, invalid_edge));
        CHECK_FALSE(lib::are_incident<GraphType>(edge_2, invalid_edge));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_type_template,
    lib::graph<lib::graph_traits<lib::directed_t>>, // directed graph
    lib::graph<lib::graph_traits<lib::directed_t>> // undirected graph
);

TEST_SUITE_END(); // test_incidence

} // namespace gl_testing
