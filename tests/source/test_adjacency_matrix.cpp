#include "constants.hpp"
#include "functional.hpp"
#include "transforms.hpp"
#include "utility.hpp"

#include <gl/impl/adjacency_matrix.hpp>

#include <doctest.h>

#include <algorithm>
#include <functional>

namespace gl_testing {

TEST_SUITE_BEGIN("test_adjacency_matrix");

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", SutType, edge_directional_tag_sut_template
) {
    SUBCASE("should be initialized with no vertices and no edges by default") {
        SutType sut{};
        CHECK_EQ(sut.no_vertices(), constants::zero_elements);
        CHECK_EQ(sut.no_unique_edges(), constants::zero_elements);
    }

    SUBCASE("constructed with the no_vertices parameter should properly initialize the adjacency "
            "matrix") {
        SutType sut{constants::no_elements};
        REQUIRE_EQ(sut.no_vertices(), constants::no_elements);
        REQUIRE_EQ(sut.no_unique_edges(), constants::zero_elements);

        std::ranges::for_each(
            constants::vertex_id_view,
            [&sut](const lib_t::id_type vertex_id) {
                CHECK_EQ(sut.adjacent_edges_c(vertex_id).distance(), constants::zero_elements);
            }
        );
    }

    SUBCASE("add_vertex should properly extend the current adjacency matrix") {
        SutType sut{};
        constexpr lib_t::size_type target_no_vertices = constants::no_elements;

        for (lib_t::size_type no_vertices = constants::one_element; no_vertices <= target_no_vertices;
             no_vertices++) {
            sut.add_vertex();
            CHECK_EQ(sut.no_vertices(), no_vertices);
        }

        CHECK_EQ(sut.no_vertices(), target_no_vertices);
        CHECK_EQ(sut.no_unique_edges(), constants::zero_elements);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    lib_i::adjacency_matrix<lib::graph_traits<lib::directed_t>>, // directed adj list
    lib_i::adjacency_matrix<lib::graph_traits<lib::undirected_t>> // undirected adj list
);

TEST_SUITE_END(); // test_adjacency_matrix

} // namespace gl_testing
