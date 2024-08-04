#include "constants.hpp"

#include <gl/impl/adjacency_list.hpp>

#include <doctest.h>

#include <algorithm>

namespace gl_testing {

struct test_adjacency_list {
    using vertex_type = lib::vertex_descriptor<>;
    using directed_edge_type = lib::directed_edge<vertex_type>;
    using undirected_edge_type = lib::undirected_edge<vertex_type>;
};

TEST_CASE_TEMPLATE_DEFINE(
    "directional_tag-independent tests", SutType, edge_directional_tag_sut_template
) {
    SUBCASE("should be initialized with no vertices by default") {
        SutType sut{};
        CHECK_EQ(sut.size(), constants::zero_vertices);
    }

    SUBCASE("constructed with no_vertices parameter should properly initialize the adj list") {
        SutType sut{constants::no_vertices};
        CHECK_EQ(sut.size(), constants::no_vertices);

        std::ranges::for_each(
            std::views::iota(constants::vertex_id_1, constants::no_vertices),
            [&sut](const lib_t::id_type vertex_id) {
                CHECK_EQ(sut.adjacent_edges(vertex_id).distance(), constants::zero_edges);
            }
        );
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    lib_i::adjacency_list<lib::graph_traits<lib::directed_t>>, // directed adj list
    lib_i::adjacency_list<lib::graph_traits<lib::undirected_t>> // undirected adj list
);


} // namespace gl_testing
