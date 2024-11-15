#include "alg_common.hpp"
#include "constants.hpp"
#include "functional.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <cmath>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_mst");

TEST_CASE_TEMPLATE_DEFINE(
    "prim mst finding tests for graphs with explicitly weighted edges",
    TraitsType,
    wieghted_edge_traits_type_template
) {
    using sut_type = lib::graph<TraitsType>;
    using weight_type = typename sut_type::edge_properties_type::weight_type;
    using distance_type = weight_type;

    static_assert(lib_tt::c_weight_properties_type<typename sut_type::edge_properties_type>);

    SUBCASE("should throw if there is an edge with a negative weight") {
        const auto sut = lib::topology::clique<sut_type>(constants::n_elements_alg);
        sut.get_edge(constants::vertex_id_1, constants::vertex_id_2)
            .value()
            .get()
            .properties.weight = -static_cast<weight_type>(constants::n_elements_alg);

        CHECK_THROWS_AS(
            func::discard_result(lib::algorithm::prim_mst(sut, constants::vertex_id_1)),
            std::invalid_argument
        );
    }

    SUBCASE("should return a proper mst descriptor for a valid graph") {
        using vertex_id_pair = std::pair<lib_t::id_type, lib_t::id_type>;

        sut_type sut;
        lib_t::id_type source_id;
        std::vector<vertex_id_pair> expected_edges;
        distance_type expected_weight;

        SUBCASE("regular binary tree") {
            sut = lib::topology::regular_binary_tree<sut_type>(constants::depth);
            source_id = constants::first_element_idx;

            const weight_type edge_weight = constants::three;
            for (const auto vertex_id : sut.vertex_ids()) {
                for (const auto& edge : sut.adjacent_edges(vertex_id)) {
                    edge.properties.weight = edge_weight;
                    expected_edges.emplace_back(edge.first_id(), edge.second_id());
                }
            }

            expected_weight = edge_weight * (sut.n_vertices() - constants::one);
        }

        SUBCASE("custom graph") {
            const fs::path gsf_file_path = alg_common::data_path / "mst_graph.gsf";

            sut = lib::io::load<sut_type>(gsf_file_path);
            source_id = constants::first_element_idx;

            const fs::path edges_file_path = alg_common::data_path / "mst_edges.txt";
            const auto n_vertex_ids = (sut.n_vertices() - constants::one) * constants::two;
            const auto vertex_id_list =
                alg_common::load_list<lib_t::id_type>(n_vertex_ids, edges_file_path);
            for (lib_t::size_type i = 0; i < n_vertex_ids; i += constants::two)
                expected_edges.emplace_back(vertex_id_list[i], vertex_id_list[i + 1]);

            const fs::path weight_file_path = alg_common::data_path / "mst_weight.txt";
            expected_weight =
                alg_common::load_list<weight_type>(constants::one, weight_file_path).front();
        }

        CAPTURE(sut);
        CAPTURE(source_id);
        CAPTURE(expected_edges);
        CAPTURE(expected_weight);

        const auto mst = lib::algorithm::prim_mst(sut, source_id);

        REQUIRE_EQ(mst.edges.size(), sut.n_vertices() - constants::one);
        REQUIRE_EQ(mst.weight, expected_weight);

        CHECK(std::ranges::all_of(mst.edges, [&expected_edges](const auto& edge_ref) {
            const auto& edge = edge_ref.get();
            const auto [first_id, second_id] = edge.incident_vertex_ids();
            return std::find_if(
                       expected_edges.begin(),
                       expected_edges.end(),
                       [&](const vertex_id_pair& vids) {
                           return (first_id == vids.first && second_id == vids.second)
                               or (first_id == vids.second && second_id == vids.first);
                       }
                   )
                != expected_edges.end();
        }));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    wieghted_edge_traits_type_template,
    lib::undirected_graph_traits<
        lib_t::empty_properties,
        lib_t::weight_property<>,
        lib_i::list_t>, // undirected adjacency list graph
    lib::undirected_graph_traits<
        lib_t::empty_properties,
        lib_t::weight_property<>,
        lib_i::matrix_t> // undirected adjacency matrix graph
);

TEST_CASE_TEMPLATE_DEFINE(
    "prim mst finding tests for graphs with unweighted edges",
    TraitsType,
    unwieghted_edge_traits_type_template
) {
    using sut_type = lib::graph<TraitsType>;
    using distance_type = lib_t::default_vertex_distance_type;
    using weight_type = distance_type;
    using vertex_id_pair = std::pair<lib_t::id_type, lib_t::id_type>;

    static_assert(not lib_tt::c_weight_properties_type<typename sut_type::edge_properties_type>);

    const auto sut = lib::topology::regular_binary_tree<sut_type>(constants::depth);
    const lib_t::id_type source_id = constants::first_element_idx;

    std::vector<vertex_id_pair> expected_edges;
    for (const auto vertex_id : sut.vertex_ids())
        for (const auto& edge : sut.adjacent_edges(vertex_id))
            expected_edges.emplace_back(edge.first_id(), edge.second_id());

    const weight_type expected_weight = sut.n_vertices() - constants::one;

    const auto mst = lib::algorithm::prim_mst(sut, source_id);

    REQUIRE_EQ(mst.edges.size(), sut.n_vertices() - constants::one);
    REQUIRE_EQ(mst.weight, expected_weight);

    CHECK(std::ranges::all_of(mst.edges, [&expected_edges](const auto& edge_ref) {
        const auto& edge = edge_ref.get();
        const auto [first_id, second_id] = edge.incident_vertex_ids();
        return std::find_if(
                   expected_edges.begin(),
                   expected_edges.end(),
                   [&](const vertex_id_pair& vids) {
                       return (first_id == vids.first && second_id == vids.second)
                           or (first_id == vids.second && second_id == vids.first);
                   }
               )
            != expected_edges.end();
    }));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    unwieghted_edge_traits_type_template,
    lib::list_graph_traits<lib::undirected_t>, // undirected adjacency list graph
    lib::matrix_graph_traits<lib::undirected_t> // undirected adjacency matrix graph
);

TEST_SUITE_END(); // test_alg_mst

} // namespace gl_testing
