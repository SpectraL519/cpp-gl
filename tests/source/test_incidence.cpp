#include "constants.hpp"

#include <gl/incidence.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_incidence");

struct test_incidence {
    using vertex_type = lib::vertex_descriptor<>;

    std::shared_ptr<vertex_type> vd_1 = std::make_shared<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> vd_2 = std::make_shared<vertex_type>(constants::vertex_id_2);

    std::shared_ptr<vertex_type> invalid_vd_1 =
        std::make_shared<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> invalid_vd_2 =
        std::make_shared<vertex_type>(constants::vertex_id_2);
};

TEST_CASE_TEMPLATE_DEFINE(
    "are_incident should return true only when an edge and a vertex are incident",
    GraphType,
    graph_type_template
) {
    test_incidence fixture;
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

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_type_template,
    lib::graph<lib::graph_traits<lib::directed_t>>, // directed graph
    lib::graph<lib::graph_traits<lib::directed_t>> // undirected graph
);

TEST_SUITE_END(); // test_incidence

} // namespace gl_testing
