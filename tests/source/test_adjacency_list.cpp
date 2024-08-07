#include "constants.hpp"
#include "functional.hpp"
#include "utility.hpp"

#include <gl/impl/adjacency_list.hpp>

#include <doctest.h>

#include <algorithm>
#include <functional>

namespace gl_testing {

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", SutType, edge_directional_tag_sut_template
) {
    SUBCASE("should be initialized with no vertices by default") {
        SutType sut{};
        CHECK_EQ(sut.size(), constants::zero_vertices);
    }

    SUBCASE("constructed with the no_vertices parameter should properly initialize the adjacency "
            "list") {
        SutType sut{constants::no_vertices};
        REQUIRE_EQ(sut.size(), constants::no_vertices);

        std::ranges::for_each(
            std::views::iota(constants::vertex_id_1, constants::no_vertices),
            [&sut](const lib_t::id_type vertex_id) {
                CHECK_EQ(sut.adjacent_edges(vertex_id).distance(), constants::zero_edges);
            }
        );
    }

    SUBCASE("add_vertex should properly extend the current adjacency list") {
        SutType sut{};
        constexpr lib_t::size_type target_no_vertices = constants::no_vertices;

        for (lib_t::id_type no_vertices = constants::one_vertex; no_vertices <= target_no_vertices;
             no_vertices++) {
            sut.add_vertex();
            CHECK_EQ(sut.size(), no_vertices);
        }

        CHECK_EQ(sut.size(), target_no_vertices);
    }

    SUBCASE("remove_vertex should remove the vertex at the given id and all edges adjacent to that "
            "vertex") {
        SutType sut{constants::no_vertices};

        sut.remove_vertex(constants::vertex_id_1);

        constexpr lib_t::size_type no_vertices_after_remove =
            constants::no_vertices - constants::one_vertex;

        std::ranges::for_each(
            std::views::iota(constants::vertex_id_1, no_vertices_after_remove),
            [&sut](const lib_t::id_type& vertex_id) {
                CHECK_NOTHROW(func::discard_result(sut.adjacent_edges(vertex_id)));
            }
        );

        CHECK_THROWS_AS(
            func::discard_result(sut.adjacent_edges(no_vertices_after_remove)), std::out_of_range
        );
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    lib_i::adjacency_list<lib::graph_traits<lib::directed_t>>, // directed adj list
    lib_i::adjacency_list<lib::graph_traits<lib::undirected_t>> // undirected adj list
);

struct test_directed_adjacency_list {
    using vertex_type = lib::vertex_descriptor<>;
    using edge_type = lib::directed_edge<vertex_type>;
    using sut_type = lib_i::adjacency_list<lib::graph_traits<lib::directed_t>>;

    sut_type sut{constants::no_vertices};
    std::vector<std::shared_ptr<vertex_type>> vertices;

    test_directed_adjacency_list() : vertices(constants::no_vertices) {
        for (const auto id : constants::vertex_id_view)
            vertices.push_back(util::make_vertex<vertex_type>(id));
    }

    /*
    ? Why does this not compile ?
    void prepare_full_graph() {
        for (const auto first_id : constants::vertex_id_view) {
            for (const auto second_id : std::views::iota(constants::vertex_id_1, first_id)) {
                sut.add_edge(lib::make_edge<edge_type>(vertices[first_id], vertices[second_id]));
                sut.add_edge(lib::make_edge<edge_type>(vertices[second_id], vertices[first_id]));
            }
        }
    }
    */
};

TEST_CASE_FIXTURE(
    test_directed_adjacency_list, "add_edge should add the edge only to the source vertex list"
) {}

} // namespace gl_testing
