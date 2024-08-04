#include "constants.hpp"

#include <gl/impl/adjacency_list.hpp>

#include <doctest.h>

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

    SUBCASE("constructed with the no_vertices parameter should properly initialize the adjcacency list") {
        SutType sut{constants::no_vertices};
        CHECK_EQ(sut.size(), constants::no_vertices);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    lib_i::adjacency_list<typename test_adjacency_list::directed_edge_type>, // directed adj list
    lib_i::adjacency_list<typename test_adjacency_list::undirected_edge_type> // undirected adj list
);


} // namespace gl_testing
