#include "constants.hpp"
#include "functional.hpp"

#include <gl/graph.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_incidence");

struct test_graph_incidence {
    using vertex_type = lib::vertex_descriptor<>;

    std::shared_ptr<vertex_type> vd_1 = std::make_shared<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> vd_2 = std::make_shared<vertex_type>(constants::vertex_id_2);
    std::shared_ptr<vertex_type> vd_3 = std::make_shared<vertex_type>(constants::vertex_id_3);

    std::shared_ptr<vertex_type> invalid_vd_1 =
        std::make_shared<vertex_type>(constants::vertex_id_1);
    std::shared_ptr<vertex_type> invalid_vd_2 =
        std::make_shared<vertex_type>(constants::vertex_id_2);
};

TEST_CASE_TEMPLATE_DEFINE("incidence functions tests", SutType, graph_type_template) {
    test_graph_incidence fixture;

    SutType sut{constants::n_elements};
    const auto& vd_1 = sut.get_vertex(constants::vertex_id_1);
    const auto& vd_2 = sut.get_vertex(constants::vertex_id_2);
    const auto& vd_3 = sut.get_vertex(constants::vertex_id_3);

    SUBCASE("are_incident(vertex and edge pair) should throw if the vertex is invalid") {
        const auto& edge = sut.add_edge(vd_1, vd_2);

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(fixture.invalid_vd_1, edge)), std::logic_error
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(fixture.invalid_vd_2, edge)), std::logic_error
        );

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(edge, fixture.invalid_vd_1)), std::logic_error
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(edge, fixture.invalid_vd_2)), std::logic_error
        );
    }

    SUBCASE("are_incident(vertex and edge pair) should return true only when the edge and the "
            "vertex are incident with each other") {
        const auto& edge = sut.add_edge(vd_1, vd_2);

        CHECK(sut.are_incident(vd_1, edge));
        CHECK(sut.are_incident(vd_2, edge));

        CHECK(sut.are_incident(edge, vd_1));
        CHECK(sut.are_incident(edge, vd_2));
    }

    SUBCASE("are_incident(edge, edge) should return true only when the edges share a common vertex"
    ) {
        const auto& edge_1 = sut.add_edge(vd_1, vd_2);
        const auto& edge_2 = sut.add_edge(vd_2, vd_3);
        const auto& edge_3 = sut.add_edge(vd_3, vd_1);

        CHECK(sut.are_incident(edge_1, edge_2));
        CHECK(sut.are_incident(edge_2, edge_3));
        CHECK(sut.are_incident(edge_3, edge_1));

        const auto invalid_edge =
            lib::make_edge<typename SutType::edge_type>(fixture.invalid_vd_1, fixture.invalid_vd_2);

        CHECK_FALSE(sut.are_incident(edge_1, invalid_edge));
        CHECK_FALSE(sut.are_incident(edge_2, invalid_edge));
        CHECK_FALSE(sut.are_incident(edge_2, invalid_edge));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_type_template,
    lib::graph<lib::graph_traits<lib::directed_t>>, // directed graph
    lib::graph<lib::graph_traits<lib::directed_t>> // undirected graph
);

TEST_SUITE_END(); // test_graph_incidence

} // namespace gl_testing
