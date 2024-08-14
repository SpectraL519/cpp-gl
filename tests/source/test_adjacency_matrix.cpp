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
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_directional_tag_sut_template,
    lib_i::adjacency_matrix<lib::graph_traits<lib::directed_t>>, // directed adj list
    lib_i::adjacency_matrix<lib::graph_traits<lib::undirected_t>> // undirected adj list
);

TEST_SUITE_END(); // test_adjacency_matrix

} // namespace gl_testing
