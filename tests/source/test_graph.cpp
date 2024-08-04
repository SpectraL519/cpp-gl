#include "constants.hpp"
#include "transforms.hpp"
#include "types.hpp"

#include <gl/graph.hpp>

#include <doctest.h>

#include <algorithm>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

struct test_graph {
    lib::graph<> sut;

    template <lib_tt::c_instantiation_of<lib::graph> GraphType>
    typename GraphType::vertex_set_type& get_vertex_list(GraphType& graph) {
        return graph._vertices;
    }
};

TEST_CASE_FIXTURE(test_graph, "graph should be initialized with no vertices by default") {
    CHECK_EQ(sut.no_vertices(), constants::zero_vertices);
}

TEST_CASE("graph constructed with no_vertices parameter should properly initialize the vertex list"
) {
    lib::graph<> sut{constants::no_vertices};

    REQUIRE(std::ranges::equal(
        sut.vertex_crange() | std::views::transform(transforms::extract_vertex_id<>),
        std::views::iota(constants::vertex_id_1, constants::no_vertices)
    ));

    CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(constants::no_vertices)), std::out_of_range);
}

TEST_CASE_FIXTURE(
    test_graph, "add_vertex should return a vertex_descriptor with an incremented id"
) {
    constexpr lib_t::size_type target_no_vertices = constants::no_vertices;

    for (lib_t::id_type v_id = constants::zero_vertices; v_id < target_no_vertices; v_id++) {
        const auto& vertex = sut.add_vertex();
        CHECK_EQ(vertex->id(), v_id);
        CHECK_EQ(sut.no_vertices(), v_id + constants::one_vertex);
    }

    CHECK_EQ(sut.no_vertices(), target_no_vertices);
}

TEST_CASE("add_vertex should initialize a new vertex with the input properties structure") {
    using traits_type = lib::graph_traits<lib::directed_t, types::visited_property>;

    lib::graph<traits_type> sut;
    const types::visited_property visited{true};

    const auto& vertex = sut.add_vertex(visited);
    REQUIRE_EQ(sut.no_vertices(), constants::one_vertex);

    CHECK_EQ(vertex->id(), 0ull);
    CHECK_EQ(vertex->properties, visited);
}

TEST_CASE_FIXTURE(test_graph, "get_vertex should throw if the given id is invalid") {
    CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(sut.no_vertices())), std::out_of_range);
}

TEST_CASE_FIXTURE(test_graph, "get_vertex should return a vertex with the given id") {
    const auto& added_vertex = sut.add_vertex();
    CHECK_EQ(*sut.get_vertex(added_vertex->id()), *added_vertex);
}

TEST_CASE_FIXTURE(
    test_graph, "vertex_(c)range should return the correct vertex list iterator range"
) {
    const auto v_range = sut.vertex_range();
    CHECK(std::ranges::equal(v_range, get_vertex_list(sut)));

    const auto v_crange = sut.vertex_crange();
    CHECK(std::ranges::equal(v_crange, get_vertex_list(sut)));
}

TEST_CASE_FIXTURE(
    test_graph, "vertex_(c)rrange should return the correct vertex list reverse iterator range"
) {
    const auto v_rrange = sut.vertex_rrange();
    CHECK(std::ranges::equal(v_rrange, get_vertex_list(sut)));

    const auto v_crrange = sut.vertex_crrange();
    CHECK(std::ranges::equal(v_crrange, get_vertex_list(sut)));
}

TEST_CASE("remove_vertex should remove the given vertex and align ids of remaining vertices") {
    lib::graph<> sut{constants::no_vertices};
    CHECK_EQ(sut.no_vertices(), constants::no_vertices);

    sut.remove_vertex(sut.get_vertex(constants::vertex_id_1));

    constexpr lib_t::id_type no_vertices_after_remove =
        constants::no_vertices - constants::one_vertex;

    REQUIRE(std::ranges::equal(
        sut.vertex_range() | std::views::transform(transforms::extract_vertex_id<>),
        std::views::iota(constants::vertex_id_1, no_vertices_after_remove)
    ));

    CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(no_vertices_after_remove)), std::out_of_range);
}

TEST_SUITE_END(); // test_graph

} // namespace gl_testing
