#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <numeric>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_topological_sort");

TEST_CASE_TEMPLATE_DEFINE(
    "topological sort tests for directed graphs", TraitsType, directed_traits_type_template
) {
    using sut_type = gl::graph<TraitsType>;

    SUBCASE("acyclic graph") {
        sut_type sut;
        std::vector<gl::types::id_type> expected_topological_order;

        SUBCASE("path graph") {
            sut = gl::topology::path<sut_type>(constants::n_elements_alg);

            for (const auto id : sut.vertex_ids())
                expected_topological_order.push_back(id);
        }

        SUBCASE("path graph with an additional source vertex") {
            sut = gl::topology::path<sut_type>(constants::n_elements_alg);

            const auto& additional_vertex = sut.add_vertex();
            sut.add_edge(additional_vertex.id(), constants::vertex_id_2);

            expected_topological_order.push_back(constants::vertex_id_1);
            expected_topological_order.push_back(additional_vertex.id());
            for (gl::types::id_type id = constants::vertex_id_2; id < constants::n_elements_alg;
                 id++)
                expected_topological_order.push_back(id);
        }

        SUBCASE("regular binary tree") {
            sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);

            for (const auto id : sut.vertex_ids())
                expected_topological_order.push_back(id);
        }

        SUBCASE("custom graph") {
            const fs::path gsf_file_path =
                alg_common::data_path / "topological_sort_directed_acyclic_graph.gsf";
            sut = gl::io::load<sut_type>(gsf_file_path);

            const fs::path order_file_path =
                alg_common::data_path / "topological_sort_directed_acyclic_order.txt";
            expected_topological_order =
                alg_common::load_list<gl::types::id_type>(sut.order(), order_file_path);
        }

        CAPTURE(sut);
        CAPTURE(expected_topological_order);

        const auto topological_order_opt = gl::algorithm::topological_sort(sut);

        REQUIRE(topological_order_opt.has_value());
        CHECK(std::ranges::equal(topological_order_opt.value(), expected_topological_order));
    }

    SUBCASE("not acyclic graph") {
        sut_type sut;

        SUBCASE("cycle graph") {
            sut = gl::topology::cycle<sut_type>(constants::n_elements_alg);
        }

        SUBCASE("clique") {
            sut = gl::topology::clique<sut_type>(constants::n_elements_alg);
        }

        SUBCASE("custom graph") {
            const fs::path gsf_file_path =
                alg_common::data_path / "topological_sort_directed_not_acyclic_graph.gsf";
            sut = gl::io::load<sut_type>(gsf_file_path);
        }

        CAPTURE(sut);

        const auto topological_order_opt = gl::algorithm::topological_sort(sut);
        CHECK_FALSE(topological_order_opt.has_value());
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    directed_traits_type_template,
    gl::list_graph_traits<gl::directed_t>, // adjacency list graph
    gl::matrix_graph_traits<gl::directed_t> // adjacency matrix graph
);

TEST_SUITE_END(); // test_alg_topological_sort

} // namespace gl_testing
