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

        SUBCASE("complete binary tree") {
            sut = lib::topology::complete_binary_tree<sut_type>(constants::depth);

            lib_t::size_type n_vertices = constants::one_element;
            lib_t::binary_color c{color::black};

            for (lib_t::size_type d = constants::zero; d < constants::depth; d++) {
                for (lib_t::size_type i = constants::zero; i < n_vertices; i++)
                    expected_coloring.push_back(c);

                n_vertices *= constants::two;
                c = c.next();
            }
        }

        SUBCASE("path graph") {
            sut = lib::topology::path<sut_type>(constants::n_elements_alg);

            lib_t::binary_color c{color::black};
            for (lib_t::size_type i = constants::zero; i < constants::n_elements_alg; i++) {
                expected_coloring.push_back(c);
                c = c.next();
            }
        }

        SUBCASE("even cycle graph") {
            const auto n_vertices = constants::two * constants::n_elements_alg;
            sut = lib::topology::cycle<sut_type>(n_vertices);

            lib_t::binary_color c{color::black};
            for (lib_t::size_type i = constants::zero; i < n_vertices; i++) {
                expected_coloring.push_back(c);
                c = c.next();
            }
        }

        SUBCASE("custom graph") {
            fs::path gsf_file_path =
                alg_common::data_path
                / (lib_tt::is_directed_v<sut_type>
                       ? "bicoloring_directed_bipartite_graph.gsf"
                       : "bicoloring_undirected_bipartite_graph.gsf");

            sut = lib::io::load<sut_type>(gsf_file_path);

            fs::path coloring_file_path =
                alg_common::data_path / "bicoloring_bipartite_graph_coloring.txt";
            const auto coloring_values =
                alg_common::read_integral_list<std::uint16_t>(sut.n_vertices(), coloring_file_path);

            std::transform(
                coloring_values.begin(),
                coloring_values.end(),
                std::back_inserter(expected_coloring),
                [](const std::uint16_t value) { return color{value}; }
            );
        }

        CAPTURE(sut);
        CAPTURE(expected_coloring);

        const auto coloring_opt = lib::algorithm::bipartite_coloring(sut);

        REQUIRE(coloring_opt.has_value());
        CHECK(std::ranges::equal(coloring_opt.value(), expected_coloring));

        // check the is_bipartite function - formality
        CHECK(lib::algorithm::is_bipartite(sut));
    }

    SUBCASE("not bipartite graph") {
        sut_type sut;

        SUBCASE("clique") {
            sut = lib::topology::clique<sut_type>(constants::n_elements_alg);
        }

        SUBCASE("odd cycle graph") {
            sut = lib::topology::cycle<sut_type>(
                constants::two * constants::n_elements_alg + constants::one
            );
        }

        SUBCASE("complete binary tree with an additional edge between siblings") {
            sut = lib::topology::complete_binary_tree<sut_type>(constants::depth);
            sut.add_edge(constants::vertex_id_2, constants::vertex_id_3);
        }

        SUBCASE("full bipartite graph with an additional edge between vertices from the same set") {
            const auto [n_vertices_a, n_vertices_b] = std::make_pair(4ull, 6ull);
            sut = lib::topology::full_bipartite<sut_type>(n_vertices_a, n_vertices_b);
            sut.add_edge(constants::vertex_id_1, constants::vertex_id_2);
        }

        SUBCASE("custom graph") {
            fs::path gsf_file_path =
                alg_common::data_path
                / (lib_tt::is_directed_v<sut_type>
                       ? "bicoloring_directed_not_bipartite_graph.gsf"
                       : "bicoloring_undirected_not_bipartite_graph.gsf");

            sut = lib::io::load<sut_type>(gsf_file_path);
        }

        CAPTURE(sut);

        const auto coloring_opt = lib::algorithm::bipartite_coloring(sut);
        CHECK_FALSE(coloring_opt.has_value());

        // check the is_bipartite function - formality
        CHECK_FALSE(lib::algorithm::is_bipartite(sut));
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
