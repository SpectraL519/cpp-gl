#include "alg_common.hpp"
#include "constants.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_coloring");

TEST_CASE_TEMPLATE_DEFINE("bipartite coloring tests", TraitsType, traits_type_template) {
    using sut_type = lib::graph<TraitsType>;
    using color = lib_t::binary_color::value;


    SUBCASE("bipartite graph") {
        // std::cout << "\n\n------------------------\n" << std::endl;

        sut_type sut;
        std::vector<lib_t::binary_color> expected_coloring;

        SUBCASE("full bipartite graph") {
            const auto [n_vertices_a, n_vertices_b] = std::make_pair(4ull, 6ull);
            sut = lib::topology::full_bipartite<sut_type>(n_vertices_a, n_vertices_b);

            expected_coloring = std::vector<lib_t::binary_color>(n_vertices_a, color::black);
            for (lib_t::size_type i = constants::first_element_idx; i < n_vertices_b; i++)
                expected_coloring.emplace_back(color::white);
        }

        // SUBCASE("complete binary tree") {
        // }

        CAPTURE(sut);
        CAPTURE(expected_coloring);

        const auto coloring_opt = lib::algorithm::bipartite_coloring(sut);

        REQUIRE(coloring_opt.has_value());
        CHECK(std::ranges::equal(coloring_opt.value(), expected_coloring));
    }

    SUBCASE("not bipartite graph") {
        sut_type sut;
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    traits_type_template,
    lib::list_graph_traits<lib::directed_t>, // directed adjacency list graph
    lib::list_graph_traits<lib::undirected_t>, // undirected adjacency list graph
    lib::matrix_graph_traits<lib::directed_t>, // directed adjacency matrix graph
    lib::matrix_graph_traits<lib::undirected_t> // undirected adjacency matrix graph
);

TEST_SUITE_END(); // test_alg_coloring

} // namespace gl_testing
