#include "constants.hpp"
#include "functional.hpp"
#include "transforms.hpp"
#include "types.hpp"

#include <gl/graph.hpp>

#include <doctest.h>

#include <algorithm>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

// TODO: add parametrization with implementation and directional tags

using default_sut_type = lib::graph<>;

template <typename TraitsType>
struct test_graph {
    using sut_type = lib::graph<TraitsType>;

    sut_type sut{};

    template <lib_tt::c_instantiation_of<lib::graph> GraphType>
    typename GraphType::vertex_set_type& get_vertex_list(GraphType& graph) {
        return graph._vertices;
    }
};

TEST_CASE_TEMPLATE_DEFINE("graph structure tests", TraitsType, graph_traits_template) {
    using fixture_type = test_graph<TraitsType>;
    fixture_type fixture;

    using sut_type = typename fixture_type::sut_type;
    sut_type& sut = fixture.sut;

    SUBCASE("graph should be initialized with no vertices and no edges by default") {
        CHECK_EQ(sut.n_vertices(), constants::zero_elements);
        CHECK_EQ(sut.n_unique_edges(), constants::zero_elements);
    }

    SUBCASE("graph constructed with n_vertices parameter should properly initialize the vertex "
            "list with no edges") {
        sut_type sut{constants::n_elements};

        REQUIRE(std::ranges::equal(
            sut.c_vertices() | std::views::transform(transforms::extract_vertex_id<>),
            constants::vertex_id_view
        ));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_range_elemenet_idx)),
            std::out_of_range
        );

        CHECK(std::ranges::all_of(constants::vertex_id_view, [&sut](const lib_t::id_type vertex_id) {
            return not static_cast<bool>(sut.adjacent_edges_c(vertex_id).distance());
        }));
    }

    SUBCASE("add_vertex should return a vertex_descriptor with an incremented id and no edges") {
        constexpr lib_t::size_type target_n_vertices = constants::n_elements;

        for (lib_t::id_type v_id = constants::zero_elements; v_id < target_n_vertices; v_id++) {
            const auto& vertex = sut.add_vertex();
            CHECK_EQ(vertex->id(), v_id);
            CHECK_EQ(sut.n_vertices(), v_id + constants::one_element);
            CHECK_EQ(sut.adjacent_edges_c(v_id).distance(), constants::empty_distance);
        }

        CHECK_EQ(sut.n_vertices(), target_n_vertices);
    }

    /*
    TEST_CASE("add_vertex should initialize a new vertex with the input properties structure") {
        using traits_type = lib::graph_traits<lib::directed_t, types::visited_property>;

        lib::graph<traits_type> sut;
        const types::visited_property visited{true}; // TODO: use constants

        const auto& vertex = sut.add_vertex(visited);
        REQUIRE_EQ(sut.n_vertices(), constants::one_element);

        CHECK_EQ(vertex->id(), 0ull);
        CHECK_EQ(vertex->properties, visited);
    }

    TEST_CASE_FIXTURE(test_graph, "get_vertex should throw if the given id is invalid") {
        CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(sut.n_vertices())), std::out_of_range);
    }

    TEST_CASE_FIXTURE(test_graph, "get_vertex should return a vertex with the given id") {
        const auto& added_vertex = sut.add_vertex();
        CHECK_EQ(*sut.get_vertex(added_vertex->id()), *added_vertex);
    }

    TEST_CASE_FIXTURE(test_graph, "(c_)vertices should return the correct vertex list iterator range") {
        const auto v_range = sut.vertices();
        CHECK(std::ranges::equal(v_range, get_vertex_list(sut)));

        const auto v_crange = sut.c_vertices();
        CHECK(std::ranges::equal(v_crange, get_vertex_list(sut)));
    }

    TEST_CASE("remove_vertex should throw if the given id is invalid") {
        default_sut_type sut{constants::n_elements};

        CHECK_THROWS_AS(sut.remove_vertex(constants::out_of_range_elemenet_idx), std::out_of_range);
    }

    TEST_CASE("remove_vertex should remove the given vertex and align ids of remaining vertices") {
        default_sut_type sut{constants::n_elements};

        sut.remove_vertex(constants::vertex_id_1);

        constexpr lib_t::size_type n_vertices_after_remove =
            constants::n_elements - constants::one_element;

        REQUIRE(std::ranges::equal(
            sut.c_vertices() | std::views::transform(transforms::extract_vertex_id<>),
            std::views::iota(constants::vertex_id_1, n_vertices_after_remove)
        ));

        // TODO: extend with adjacency iterator ranges checks

        CHECK_THROWS_AS(
            func::discard_result(sut.get_vertex(n_vertices_after_remove)), std::out_of_range
        );
    }
    */
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_traits_template,
    lib::list_graph_traits<lib::directed_t>, // directed adjacency list
    lib::list_graph_traits<lib::undirected_t>, // undirected adjacency list
    lib::matrix_graph_traits<lib::directed_t>, // directed adjacency matrix
    lib::matrix_graph_traits<lib::undirected_t> // undirected adjacency matrix
);

TEST_SUITE_END(); // test_graph

} // namespace gl_testing
