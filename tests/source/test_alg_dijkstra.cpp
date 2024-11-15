#include "alg_common.hpp"
#include "constants.hpp"
#include "functional.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <cmath>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_dijkstra");

TEST_CASE_TEMPLATE_DEFINE(
    "dijkstra shortest path finding tests for graphs with explicitly weighted edges",
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
            func::discard_result(
                lib::algorithm::dijkstra_shortest_paths(sut, constants::vertex_id_1)
            ),
            std::invalid_argument
        );
    }

    SUBCASE("should return a proper paths descriptor for a valid graph") {
        sut_type sut;
        lib_t::id_type source_id;
        std::vector<lib_t::id_type> expected_predecessors;
        std::vector<distance_type> expected_distances;

        const auto source_distance = static_cast<distance_type>(constants::zero);

        SUBCASE("clique") {
            sut = lib::topology::clique<sut_type>(constants::n_elements_alg);
            source_id = constants::first_element_idx;

            expected_predecessors =
                std::vector<lib_t::id_type>(constants::n_elements_alg, source_id);

            expected_distances.push_back(source_distance);
            const auto edge_weight = static_cast<weight_type>(constants::n_elements_alg);
            for (lib_t::id_type id = constants::vertex_id_2; id < constants::n_elements_alg; id++) {
                sut.get_edge(constants::vertex_id_1, id).value().get().properties.weight =
                    edge_weight;
                expected_distances.push_back(edge_weight);
            }
        }

        SUBCASE("regular binary tree") {
            sut = lib::topology::regular_binary_tree<sut_type>(constants::depth);
            source_id = constants::first_element_idx;

            for (const auto id : sut.vertex_ids()) {
                const auto parent_id =
                    id == constants::zero
                        ? constants::zero
                        : (id - constants::one) / constants::two;
                expected_predecessors.push_back(parent_id);

                const auto vertex_depth =
                    constants::zero ? constants::zero
                                    : static_cast<lib_t::size_type>(std::log2(id + constants::one));
                expected_distances.push_back(vertex_depth);
            }
        }

        SUBCASE("custom graph") {
            const std::string file_name_prefix =
                lib_tt::is_directed_v<sut_type> ? "dijkstra_directed_" : "dijkstra_undirected_";

            const fs::path gsf_file_path = alg_common::data_path / (file_name_prefix + "graph.gsf");

            sut = lib::io::load<sut_type>(gsf_file_path);
            source_id = constants::first_element_idx;

            const fs::path predecessors_file_path =
                alg_common::data_path / (file_name_prefix + "predecessors.txt");
            expected_predecessors =
                alg_common::load_list<lib_t::id_type>(sut.n_vertices(), predecessors_file_path);

            const fs::path distances_file_path =
                alg_common::data_path / (file_name_prefix + "distances.txt");
            expected_distances =
                alg_common::load_list<distance_type>(sut.n_vertices(), distances_file_path);
        }

        CAPTURE(sut);
        CAPTURE(source_id);
        CAPTURE(expected_predecessors);
        CAPTURE(expected_distances);

        const auto paths = lib::algorithm::dijkstra_shortest_paths(sut, source_id);

        REQUIRE(std::ranges::all_of(sut.vertex_ids(), [&paths](const auto vertex_id) {
            return paths.is_reachable(vertex_id);
        }));

        CHECK(std::ranges::equal(
            paths.predecessors,
            expected_predecessors,
            std::ranges::equal_to{},
            [](const auto& id_opt) { return id_opt.value(); }
        ));

        CHECK(std::ranges::equal(paths.distances, expected_distances));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    wieghted_edge_traits_type_template,
    lib::list_graph_traits<
        lib::directed_t,
        lib_t::empty_properties,
        lib_t::weight_property<>>, // directed adjacency list graph
    lib::list_graph_traits<
        lib::undirected_t,
        lib_t::empty_properties,
        lib_t::weight_property<>>, // undirected adjacency list graph
    lib::matrix_graph_traits<
        lib::directed_t,
        lib_t::empty_properties,
        lib_t::weight_property<>>, // directed adjacency matrix graph
    lib::matrix_graph_traits<
        lib::undirected_t,
        lib_t::empty_properties,
        lib_t::weight_property<>> // undirected adjacency matrix graph
);

TEST_CASE_TEMPLATE_DEFINE(
    "dijkstra shortest path finding tests for graphs with unweighted edges",
    TraitsType,
    unwieghted_edge_traits_type_template
) {
    using sut_type = lib::graph<TraitsType>;
    using distance_type = lib_t::default_vertex_distance_type;

    static_assert(not lib_tt::c_weight_properties_type<typename sut_type::edge_properties_type>);

    SUBCASE("should return a proper paths descriptor for a valid graph") {
        sut_type sut;
        lib_t::id_type source_id;
        std::vector<lib_t::id_type> expected_predecessors;
        std::vector<distance_type> expected_distances;

        const auto source_distance = static_cast<distance_type>(constants::zero);

        SUBCASE("clique") {
            sut = lib::topology::clique<sut_type>(constants::n_elements_alg);
            source_id = constants::first_element_idx;

            expected_predecessors =
                std::vector<lib_t::id_type>(constants::n_elements_alg, source_id);

            expected_distances.push_back(source_distance);
            for (lib_t::id_type id = constants::vertex_id_2; id < constants::n_elements_alg; id++)
                expected_distances.push_back(constants::one);
        }

        SUBCASE("regular binary tree") {
            sut = lib::topology::regular_binary_tree<sut_type>(constants::depth);
            source_id = constants::first_element_idx;

            for (const auto id : sut.vertex_ids()) {
                const auto parent_id =
                    id == constants::zero
                        ? constants::zero
                        : (id - constants::one) / constants::two;
                expected_predecessors.push_back(parent_id);

                const auto vertex_depth =
                    constants::zero ? constants::zero
                                    : static_cast<lib_t::size_type>(std::log2(id + constants::one));
                expected_distances.push_back(vertex_depth);
            }
        }

        CAPTURE(sut);
        CAPTURE(source_id);
        CAPTURE(expected_predecessors);
        CAPTURE(expected_distances);

        const auto paths = lib::algorithm::dijkstra_shortest_paths(sut, source_id);

        REQUIRE(std::ranges::all_of(sut.vertex_ids(), [&paths](const auto vertex_id) {
            return paths.is_reachable(vertex_id);
        }));

        CHECK(std::ranges::equal(
            paths.predecessors,
            expected_predecessors,
            std::ranges::equal_to{},
            [](const auto& id_opt) { return id_opt.value(); }
        ));

        CHECK(std::ranges::equal(paths.distances, expected_distances));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    unwieghted_edge_traits_type_template,
    lib::list_graph_traits<lib::directed_t>, // directed adjacency list graph
    lib::list_graph_traits<lib::undirected_t>, // undirected adjacency list graph
    lib::matrix_graph_traits<lib::directed_t>, // directed adjacency matrix graph
    lib::matrix_graph_traits<lib::undirected_t> // undirected adjacency matrix graph
);

TEST_CASE("reconstruct_path should thow if the vertex is not reachable") {
    const std::vector<std::optional<lib_t::id_type>> predecessor_map = {0, 3, 1, std::nullopt};
    lib_t::id_type vertex_id = predecessor_map.size() - constants::one;

    CHECK_THROWS_AS(
        func::discard_result(lib::algorithm::reconstruct_path(predecessor_map, vertex_id)),
        std::invalid_argument
    );
}

TEST_CASE("reconstruct_path should properly reconstruct the search path to the specified vertex") {
    const std::vector<std::optional<lib_t::id_type>> predecessor_map = {0, 3, 1, 0};

    lib_t::id_type vertex_id;
    std::deque<lib_t::id_type> expected_path;

    SUBCASE("starting vertex = 0") {
        vertex_id = 0;
        expected_path = {0};
    }

    SUBCASE("starting vertex = 1") {
        vertex_id = 1;
        expected_path = {0, 3, 1};
    }

    SUBCASE("starting vertex = 2") {
        vertex_id = 2;
        expected_path = {0, 3, 1, 2};
    }

    SUBCASE("starting vertex = 3") {
        vertex_id = 3;
        expected_path = {0, 3};
    }

    CAPTURE(vertex_id);
    CAPTURE(expected_path);

    CHECK(std::ranges::equal(
        lib::algorithm::reconstruct_path(predecessor_map, vertex_id), expected_path
    ));
}

TEST_SUITE_END(); // test_alg_dijkstra

} // namespace gl_testing
