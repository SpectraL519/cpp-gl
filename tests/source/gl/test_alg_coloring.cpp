#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_coloring");

TEST_CASE_TEMPLATE_DEFINE("bipartite coloring tests", TraitsType, traits_type_template) {
    using sut_type = gl::graph<TraitsType>;

    SUBCASE("apply_coloring should return false if the size of coloring is different then "
            "n_vertices") {
        sut_type sut{constants::n_elements_alg};
        std::vector<gl::binary_color> empty_coloring;

        CHECK_FALSE(gl::algorithm::apply_coloring(sut, empty_coloring));
    }

    SUBCASE("bipartite graph") {
        sut_type sut;
        std::vector<gl::binary_color> expected_coloring;

        SUBCASE("biclique") {
            const auto [n_vertices_a, n_vertices_b] = std::make_pair(4ull, 6ull);
            sut = gl::topology::biclique<sut_type>(n_vertices_a, n_vertices_b);

            expected_coloring =
                std::vector<gl::binary_color>(n_vertices_a, gl::bin_color_value::black);
            for (gl::size_type i = 0uz; i < n_vertices_b; i++)
                expected_coloring.emplace_back(gl::bin_color_value::white);
        }

        SUBCASE("regular binary tree") {
            sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);

            gl::size_type n_vertices = 1uz;
            gl::binary_color c{gl::bin_color_value::black};

            for (auto d = 0uz; d < constants::depth; d++) {
                for (auto i = 0uz; i < n_vertices; i++)
                    expected_coloring.push_back(c);

                n_vertices *= 2uz;
                c = c.next();
            }
        }

        SUBCASE("path graph") {
            sut = gl::topology::path<sut_type>(constants::n_elements_alg);

            gl::binary_color c{gl::bin_color_value::black};
            for (auto i = 0uz; i < constants::n_elements_alg; i++) {
                expected_coloring.push_back(c);
                c = c.next();
            }
        }

        SUBCASE("even cycle graph") {
            const auto n_vertices = 2uz * constants::n_elements_alg;
            sut = gl::topology::cycle<sut_type>(n_vertices);

            gl::binary_color c{gl::bin_color_value::black};
            for (auto i = 0uz; i < n_vertices; i++) {
                expected_coloring.push_back(c);
                c = c.next();
            }
        }

        SUBCASE("custom graph") {
            fs::path gsf_file_path =
                data_path
                / (gl::traits::c_directed_graph<sut_type>
                       ? "bicoloring_directed_bipartite_graph.gsf"
                       : "bicoloring_undirected_bipartite_graph.gsf");

            sut = gl::io::load<sut_type>(gsf_file_path);

            fs::path coloring_file_path = data_path / "bicoloring_bipartite_graph_coloring.txt";

            const auto coloring_values = load_list<std::uint16_t>(sut.order(), coloring_file_path);

            std::transform(
                coloring_values.begin(),
                coloring_values.end(),
                std::back_inserter(expected_coloring),
                [](const std::uint16_t value) { return gl::bin_color_value{value}; }
            );
        }

        CAPTURE(sut);
        CAPTURE(expected_coloring);

        const auto coloring_opt = gl::algorithm::bipartite_coloring(sut);

        REQUIRE(coloring_opt.has_value());
        CHECK(std::ranges::equal(coloring_opt.value(), expected_coloring));

        // check the is_bipartite function - formality
        CHECK(gl::algorithm::is_bipartite(sut));

        // --- apply_coloring test ---
        REQUIRE(gl::algorithm::apply_coloring(sut, expected_coloring));
        CHECK(std::ranges::equal(
            sut.vertices(),
            expected_coloring,
            std::ranges::equal_to{},
            [](const auto& vertex) { return vertex.properties().color; }
        ));
    }

    SUBCASE("not bipartite graph") {
        sut_type sut;

        SUBCASE("clique") {
            sut = gl::topology::clique<sut_type>(constants::n_elements_alg);
        }

        SUBCASE("odd cycle graph") {
            sut = gl::topology::cycle<sut_type>(2uz * constants::n_elements_alg + 1uz);
        }

        SUBCASE("regular binary tree with an additional edge between siblings") {
            sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);
            sut.add_edge(constants::v2_id, constants::v3_id);
        }

        SUBCASE("biclique with an additional edge between vertices from the same set") {
            const auto [n_vertices_a, n_vertices_b] = std::make_pair(4ull, 6ull);
            sut = gl::topology::biclique<sut_type>(n_vertices_a, n_vertices_b);
            sut.add_edge(constants::v1_id, constants::v2_id);
        }

        SUBCASE("custom graph") {
            fs::path gsf_file_path =
                data_path
                / (gl::traits::c_directed_graph<sut_type>
                       ? "bicoloring_directed_not_bipartite_graph.gsf"
                       : "bicoloring_undirected_not_bipartite_graph.gsf");

            sut = gl::io::load<sut_type>(gsf_file_path);
        }

        CAPTURE(sut);

        const auto coloring_opt = gl::algorithm::bipartite_coloring(sut);
        CHECK_FALSE(coloring_opt.has_value());

        // check the is_bipartite function - formality
        CHECK_FALSE(gl::algorithm::is_bipartite(sut));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    traits_type_template,
    gl::list_graph_traits<
        gl::directed_t,
        gl::binary_color_property>, // directed adjacency list graph
    gl::list_graph_traits<
        gl::undirected_t,
        gl::binary_color_property>, // undirected adjacency list graph
    gl::flat_list_graph_traits<
        gl::directed_t,
        gl::binary_color_property>, // directed flat adjacency list graph
    gl::flat_list_graph_traits<
        gl::undirected_t,
        gl::binary_color_property>, // undirected flat adjacency list graph
    gl::matrix_graph_traits<
        gl::directed_t,
        gl::binary_color_property>, // directed adjacency matrix graph
    gl::matrix_graph_traits<
        gl::undirected_t,
        gl::binary_color_property>, // undirected adjacency matrix graph
    gl::flat_matrix_graph_traits<
        gl::directed_t,
        gl::binary_color_property>, // directed flat adjacency matrix graph
    gl::flat_matrix_graph_traits<
        gl::undirected_t,
        gl::binary_color_property> // undirected flat adjacency matrix graph
);

TEST_SUITE_END(); // test_alg_coloring

} // namespace gl_testing
