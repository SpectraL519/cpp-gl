#include "constants.hpp"
#include "functional.hpp"
#include "transforms.hpp"
#include "types.hpp"

#include <gl/graph.hpp>

#include <doctest.h>

#include <algorithm>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph");

template <typename TraitsType>
struct test_graph {
    using traits_type = TraitsType;
    using sut_type = lib::graph<traits_type>;

    template <lib_tt::c_instantiation_of<lib::graph> GraphType>
    typename GraphType::vertex_set_type& get_vertex_list(GraphType& graph) {
        return graph._vertices;
    }
};

template <lib_tt::c_instantiation_of<lib::graph_traits> TraitsType, lib_tt::c_properties VertexProperties>
using add_vertex_property =
    lib::graph_traits<
        typename TraitsType::edge_directional_tag,
        VertexProperties,
        typename TraitsType::edge_properties_type,
        typename TraitsType::implementation_tag>;

TEST_CASE_TEMPLATE_DEFINE("graph structure tests", TraitsType, graph_traits_template) {
    using fixture_type = test_graph<TraitsType>;
    using sut_type = typename fixture_type::sut_type;
    using traits_type = typename fixture_type::traits_type;

    fixture_type fixture;

    SUBCASE("graph should be initialized with no vertices and no edges by default") {
        sut_type sut;

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
        sut_type sut;

        constexpr lib_t::size_type target_n_vertices = constants::n_elements;
        for (lib_t::id_type v_id = constants::zero_elements; v_id < target_n_vertices; v_id++) {
            const auto& vertex = sut.add_vertex();
            CHECK_EQ(vertex->id(), v_id);
            CHECK_EQ(sut.n_vertices(), v_id + constants::one_element);
            CHECK_EQ(sut.adjacent_edges_c(v_id).distance(), constants::empty_distance);
        }

        CHECK_EQ(sut.n_vertices(), target_n_vertices);
    }

    SUBCASE("add_vertex should initialize a new vertex with the input properties structure") {
        using properties_traits_type = add_vertex_property<traits_type, types::visited_property>;
        lib::graph<properties_traits_type> sut;

        const auto& vertex = sut.add_vertex(constants::visited);
        REQUIRE_EQ(sut.n_vertices(), constants::one_element);

        CHECK_EQ(vertex->id(), constants::vertex_id_1);
        CHECK_EQ(vertex->properties, constants::visited);
    }

    SUBCASE("get_vertex should throw if the given id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(sut.n_vertices())), std::out_of_range);
    }

    SUBCASE("get_vertex should return a vertex with the given id") {
        sut_type sut;
        const auto& added_vertex = sut.add_vertex();
        CHECK_EQ(*sut.get_vertex(added_vertex->id()), *added_vertex);
    }

    SUBCASE("(c_)vertices should return the correct vertex list iterator range") {
        sut_type sut{constants::n_elements};

        const auto v_range = sut.vertices();
        CHECK(std::ranges::equal(v_range, fixture.get_vertex_list(sut)));

        const auto v_crange = sut.c_vertices();
        CHECK(std::ranges::equal(v_crange, fixture.get_vertex_list(sut)));
    }

    SUBCASE("remove_vertex should throw if the given id is invalid") {
        sut_type sut{constants::n_elements};
        CHECK_THROWS_AS(sut.remove_vertex(constants::out_of_range_elemenet_idx), std::out_of_range);
    }

    SUBCASE("remove_vertex should remove the given vertex and align ids of remaining vertices") {
        // TODO: prepare full graph and verify that no vertices are adjacent with the removed vertex

        sut_type sut{constants::n_elements};

        sut.remove_vertex(constants::vertex_id_1);

        constexpr lib_t::size_type n_vertices_after_remove =
            constants::n_elements - constants::one_element;

        REQUIRE(std::ranges::equal(
            sut.c_vertices() | std::views::transform(transforms::extract_vertex_id<>),
            std::views::iota(constants::vertex_id_1, n_vertices_after_remove)
        ));

        CHECK_THROWS_AS(
            func::discard_result(sut.get_vertex(n_vertices_after_remove)), std::out_of_range
        );
    }
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
