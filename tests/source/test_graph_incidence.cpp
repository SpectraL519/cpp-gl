#include "constants.hpp"
#include "functional.hpp"

#include <gl/graph.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_incidence");

struct test_graph_incidence {
    using vertex_type = lib::vertex_descriptor<>;

    vertex_type invalid_vertex{constants::vertex_id_1};
    vertex_type out_of_range_vertex{constants::out_of_range_elemenet_idx};
};

TEST_CASE_TEMPLATE_DEFINE("incidence functions tests", SutType, graph_type_template) {
    test_graph_incidence fixture;

    SutType sut{constants::n_elements};
    const auto& vd_1 = sut.get_vertex(constants::vertex_id_1);
    const auto& vd_2 = sut.get_vertex(constants::vertex_id_2);
    const auto& vd_3 = sut.get_vertex(constants::vertex_id_3);

    SUBCASE("are_incident(vertex_id, vertex_id) should throw for out of range vertex ids") {
        CHECK_THROWS_AS(
            func::discard_result(
                sut.are_incident(constants::out_of_range_elemenet_idx, constants::vertex_id_2)
            ),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(
                sut.are_incident(constants::vertex_id_1, constants::out_of_range_elemenet_idx)
            ),
            std::invalid_argument
        );
    }

    SUBCASE("are_incident(vertex_id, vertex_id) should return true the ids are the same and valid"
    ) {
        CHECK(std::ranges::all_of(constants::vertex_id_view, [&sut](const auto vertex_id) {
            return sut.are_incident(vertex_id, vertex_id);
        }));
    }

    SUBCASE("are_incident(vertex_id, vertex_id) should return true if there is an edge connecting "
            "the given vertices") {
        sut.add_edge(vd_1, vd_2);

        CHECK(sut.are_incident(constants::vertex_id_1, constants::vertex_id_2));
        CHECK(sut.are_incident(constants::vertex_id_2, constants::vertex_id_1));

        CHECK_FALSE(sut.are_incident(constants::vertex_id_1, constants::vertex_id_3));
        CHECK_FALSE(sut.are_incident(constants::vertex_id_2, constants::vertex_id_3));
    }

    SUBCASE("are_incident(vertex, vertex) should throw if at least one of the vertices is invalid"
    ) {
        CHECK_THROWS_AS(
            func::discard_result(
                sut.are_incident(fixture.out_of_range_vertex, fixture.out_of_range_vertex)
            ),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(fixture.out_of_range_vertex, vd_2)),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(vd_1, fixture.out_of_range_vertex)),
            std::out_of_range
        );

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(fixture.invalid_vertex, fixture.invalid_vertex)),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(fixture.invalid_vertex, vd_2)),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(vd_1, fixture.invalid_vertex)),
            std::invalid_argument
        );
    }

    SUBCASE("are_incident(vertex, vertex) should return true if there is an edge connecting the "
            "given vertices") {
        sut.add_edge(vd_1, vd_2);

        CHECK(sut.are_incident(vd_1, vd_2));
        CHECK(sut.are_incident(vd_2, vd_1));

        CHECK_FALSE(sut.are_incident(vd_1, vd_3));
        CHECK_FALSE(sut.are_incident(vd_2, vd_3));
    }

    SUBCASE("are_incident(vertex, vertex) should return true the vertices are the same and valid") {
        CHECK(std::ranges::all_of(sut.vertices(), [&sut](const auto& vertex) {
            return sut.are_incident(vertex, vertex);
        }));
    }

    SUBCASE("are_incident(vertex and edge pair) should throw if the vertex is invalid") {
        const auto& edge = sut.add_edge(vd_1, vd_2);

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(fixture.invalid_vertex, edge)),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(fixture.invalid_vertex, edge)),
            std::invalid_argument
        );

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(edge, fixture.invalid_vertex)),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(edge, fixture.invalid_vertex)),
            std::invalid_argument
        );
    }

    SUBCASE("are_incident(vertex and edge pair) should throw if the edge is invalid") {
        const typename SutType::edge_type invalid_edge{vd_1, vd_2};

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(vd_1, invalid_edge)), std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(vd_1, invalid_edge)), std::invalid_argument
        );

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(invalid_edge, vd_2)), std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(invalid_edge, vd_2)), std::invalid_argument
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

    SUBCASE("are_incident(edge, edge) should throw if either edge is invalid") {
        const auto& edge = sut.add_edge(vd_1, vd_2);
        const typename SutType::edge_type invalid_edge{vd_1, vd_2};

        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(edge, invalid_edge)), std::invalid_argument
        );
        CHECK_THROWS_AS(
            func::discard_result(sut.are_incident(invalid_edge, edge)), std::invalid_argument
        );
    }

    SUBCASE("are_incident(edge, edge) should return true only when the edges share a common vertex"
    ) {
        const auto& edge_1 = sut.add_edge(vd_1, vd_2);
        const auto& edge_2 = sut.add_edge(vd_2, vd_3);
        const auto& loop_3 = sut.add_edge(vd_3, vd_3);

        CHECK(sut.are_incident(edge_1, edge_2));
        CHECK(sut.are_incident(edge_2, edge_1));
        CHECK_FALSE(sut.are_incident(edge_1, loop_3));
        CHECK_FALSE(sut.are_incident(loop_3, edge_1));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_type_template,
    lib::graph<lib::graph_traits<lib::directed_t>>, // directed graph
    lib::graph<lib::graph_traits<lib::undirected_t>> // undirected graph
);

TEST_SUITE_END(); // test_graph_incidence

} // namespace gl_testing
