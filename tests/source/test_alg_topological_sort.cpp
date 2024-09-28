#include "alg_common.hpp"
#include "constants.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <numeric>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_topological_sort");

TEST_CASE_TEMPLATE_DEFINE(
    "topological sort tests for directed graphs", TraitsType, directed_traits_type_template
) {
    using sut_type = lib::graph<TraitsType>;

    SUBCASE("acyclic graph") {
        sut_type sut;
        std::vector<lib_t::id_type> expected_topological_order;

        SUBCASE("path graph") {
            sut = lib::topology::path<sut_type>(constants::n_elements_alg);

            for (const auto id : sut.vertex_ids())
                expected_topological_order.push_back(id);
        }

        SUBCASE("path graph with an additional source vertex") {
            sut = lib::topology::path<sut_type>(constants::n_elements_alg);

            const auto& additional_vertex = sut.add_vertex();
            sut.add_edge(additional_vertex.id(), constants::vertex_id_2);

            expected_topological_order.push_back(constants::vertex_id_1);
            expected_topological_order.push_back(additional_vertex.id());
            for (lib_t::id_type id = constants::vertex_id_2; id < constants::n_elements_alg; id++)
                expected_topological_order.push_back(id);
        }

        SUBCASE("complete binary tree") {
            sut = lib::topology::complete_binary_tree<sut_type>(constants::depth);

            for (const auto id : sut.vertex_ids())
                expected_topological_order.push_back(id);
        }

        SUBCASE("custom graph") {
            const fs::path gsf_file_path =
                alg_common::data_path / "topological_sort_directed_acyclic_graph.gsf";
            sut = lib::io::load<sut_type>(gsf_file_path);

            const fs::path order_file_path =
                alg_common::data_path / "topological_sort_directed_acyclic_order.txt";
            expected_topological_order =
                alg_common::read_integral_list<lib_t::id_type>(sut.n_vertices(), order_file_path);
        }

        CAPTURE(sut);
        CAPTURE(expected_topological_order);

        const auto topological_order_opt = lib::algorithm::topological_sort(sut);

        REQUIRE(topological_order_opt.has_value());
        CHECK(std::ranges::equal(topological_order_opt.value(), expected_topological_order));
    }

    SUBCASE("not acyclic graph") {
        sut_type sut;

        SUBCASE("cycle graph") {
            sut = lib::topology::cycle<sut_type>(constants::n_elements_alg);
        }

        SUBCASE("clique") {
            sut = lib::topology::clique<sut_type>(constants::n_elements_alg);
        }

        SUBCASE("custom graph") {
            const fs::path gsf_file_path =
                alg_common::data_path / "topological_sort_directed_not_acyclic_graph.gsf";
            sut = lib::io::load<sut_type>(gsf_file_path);
        }

        CAPTURE(sut);

        const auto topological_order_opt = lib::algorithm::topological_sort(sut);
        CHECK_FALSE(topological_order_opt.has_value());
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    directed_traits_type_template,
    lib::list_graph_traits<lib::directed_t>, // adjacency list graph
    lib::matrix_graph_traits<lib::directed_t> // adjacency matrix graph
);

TEST_SUITE_END(); // test_alg_topological_sort

} // namespace gl_testing
