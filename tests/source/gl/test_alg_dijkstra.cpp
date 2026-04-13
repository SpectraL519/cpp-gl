#include "gl/constants.hpp"
#include "gl/types/core.hpp"
#include "testing/common/functional.hpp"
#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm.hpp>
#include <gl/io/graph_fio.hpp>
#include <gl/topology.hpp>

#include <doctest.h>

#include <cmath>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_dijkstra");

TEST_CASE_TEMPLATE_DEFINE(
    "dijkstra shortest path finding tests for graphs with explicitly weighted edges",
    TraitsType,
    wieghted_edge_traits_type_template
) {
    using sut_type = gl::graph<TraitsType>;
    using weight_type = typename sut_type::edge_properties_type::weight_type;
    using distance_type = weight_type;

    static_assert(gl::traits::c_weight_properties_type<typename sut_type::edge_properties_type>);

    SUBCASE("should throw if there is an edge with a negative weight") {
        const auto sut = gl::topology::clique<sut_type>(constants::n_elements_alg);
        sut.get_edge(constants::v1_id, constants::v2_id)->properties().weight =
            -static_cast<weight_type>(constants::n_elements_alg);

        CHECK_THROWS_AS(
            discard_result(gl::algorithm::dijkstra_shortest_paths(sut, constants::v1_id)),
            std::invalid_argument
        );
    }

    SUBCASE("should return a proper paths descriptor for a valid graph") {
        sut_type sut;
        gl::default_id_type source_id;
        std::vector<gl::default_id_type> expected_predecessors;
        std::vector<distance_type> expected_distances;

        const distance_type source_distance = 0;

        SUBCASE("clique") {
            sut = gl::topology::clique<sut_type>(constants::n_elements_alg);
            source_id = 0uz;

            expected_predecessors =
                std::vector<gl::default_id_type>(constants::n_elements_alg, source_id);

            expected_distances.push_back(source_distance);
            const auto edge_weight = static_cast<weight_type>(constants::n_elements_alg);
            for (auto id = constants::v2_id; id < constants::n_elements_alg; id++) {
                sut.get_edge(constants::v1_id, id)->properties().weight = edge_weight;
                expected_distances.push_back(edge_weight);
            }
        }

        SUBCASE("regular binary tree") {
            sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);
            source_id = 0uz;

            for (const auto id : sut.vertex_ids()) {
                const auto parent_id = id == 0u ? 0u : (id - 1u) / 2u;
                expected_predecessors.push_back(parent_id);

                const auto vertex_depth =
                    static_cast<distance_type>(std::floor(std::log2(id + 1uz)));
                expected_distances.push_back(vertex_depth);
            }
        }

        SUBCASE("custom graph") {
            const std::string file_name_prefix =
                gl::traits::c_directed_graph<sut_type>
                    ? "dijkstra_directed_"
                    : "dijkstra_undirected_";

            const fs::path gsf_file_path = data_path / (file_name_prefix + "graph.gsf");

            sut = gl::io::load<sut_type>(gsf_file_path);
            source_id = constants::v1_id;

            const fs::path predecessors_file_path =
                data_path / (file_name_prefix + "predecessors.txt");
            expected_predecessors =
                load_list<gl::default_id_type>(sut.order(), predecessors_file_path);

            const fs::path distances_file_path = data_path / (file_name_prefix + "distances.txt");
            expected_distances = load_list<distance_type>(sut.order(), distances_file_path);
        }

        CAPTURE(sut);
        CAPTURE(source_id);
        CAPTURE(expected_predecessors);
        CAPTURE(expected_distances);

        const auto paths = gl::algorithm::dijkstra_shortest_paths(sut, source_id);

        REQUIRE(std::ranges::all_of(sut.vertex_ids(), [&paths](const auto vertex_id) {
            return gl::algorithm::is_reachable(paths.predecessors, vertex_id);
        }));

        CHECK(std::ranges::equal(paths.predecessors, expected_predecessors));
        CHECK(std::ranges::equal(paths.distances, expected_distances));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    wieghted_edge_traits_type_template,
    gl::list_graph_traits<
        gl::directed_t,
        gl::empty_properties,
        gl::weight_property<>>, // directed adjacency list graph
    gl::list_graph_traits<
        gl::undirected_t,
        gl::empty_properties,
        gl::weight_property<>>, // undirected adjacency list graph
    gl::flat_list_graph_traits<
        gl::directed_t,
        gl::empty_properties,
        gl::weight_property<>>, // directed flat adjacency list graph
    gl::flat_list_graph_traits<
        gl::undirected_t,
        gl::empty_properties,
        gl::weight_property<>>, // undirected flat adjacency list graph
    gl::matrix_graph_traits<
        gl::directed_t,
        gl::empty_properties,
        gl::weight_property<>>, // directed adjacency matrix graph
    gl::matrix_graph_traits<
        gl::undirected_t,
        gl::empty_properties,
        gl::weight_property<>>, // undirected adjacency matrix graph
    gl::flat_matrix_graph_traits<
        gl::directed_t,
        gl::empty_properties,
        gl::weight_property<>>, // directed flat adjacency matrix graph
    gl::flat_matrix_graph_traits<
        gl::undirected_t,
        gl::empty_properties,
        gl::weight_property<>> // undirected flat adjacency matrix graph
);

TEST_CASE_TEMPLATE_DEFINE(
    "dijkstra shortest path finding tests for graphs with unweighted edges",
    TraitsType,
    unwieghted_edge_traits_type_template
) {
    using sut_type = gl::graph<TraitsType>;
    using distance_type = gl::default_vertex_distance_type;

    static_assert(not gl::traits::c_weight_properties_type<typename sut_type::edge_properties_type>);

    SUBCASE("should return a proper paths descriptor for a valid graph") {
        sut_type sut;
        gl::default_id_type source_id;
        std::vector<gl::default_id_type> expected_predecessors;
        std::vector<distance_type> expected_distances;

        const distance_type source_distance = 0;

        SUBCASE("clique") {
            sut = gl::topology::clique<sut_type>(constants::n_elements_alg);
            source_id = 0uz;

            expected_predecessors =
                std::vector<gl::default_id_type>(constants::n_elements_alg, source_id);

            expected_distances.push_back(source_distance);
            for (auto id = constants::v2_id; id < constants::n_elements_alg; id++)
                expected_distances.push_back(1uz);
        }

        SUBCASE("regular binary tree") {
            sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);
            source_id = 0uz;

            for (const auto id : sut.vertex_ids()) {
                const auto parent_id = id == 0u ? 0u : (id - 1u) / 2u;
                expected_predecessors.push_back(parent_id);

                const auto vertex_depth =
                    static_cast<distance_type>(std::floor(std::log2(id + 1uz)));
                expected_distances.push_back(vertex_depth);
            }
        }

        CAPTURE(sut);
        CAPTURE(source_id);
        CAPTURE(expected_predecessors);
        CAPTURE(expected_distances);

        const auto paths = gl::algorithm::dijkstra_shortest_paths(sut, source_id);

        REQUIRE(std::ranges::all_of(sut.vertex_ids(), [&paths](const auto vertex_id) {
            return gl::algorithm::is_reachable(paths.predecessors, vertex_id);
        }));

        CHECK(std::ranges::equal(paths.predecessors, expected_predecessors));
        CHECK(std::ranges::equal(paths.distances, expected_distances));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    unwieghted_edge_traits_type_template,
    gl::list_graph_traits<gl::directed_t>, // directed adjacency list graph
    gl::list_graph_traits<gl::undirected_t>, // undirected adjacency list graph
    gl::flat_list_graph_traits<gl::directed_t>, // directed flat adjacency list graph
    gl::flat_list_graph_traits<gl::undirected_t>, // undirected flat adjacency list graph
    gl::matrix_graph_traits<gl::directed_t>, // directed adjacency matrix graph
    gl::matrix_graph_traits<gl::undirected_t>, // undirected adjacency matrix graph
    gl::flat_matrix_graph_traits<gl::directed_t>, // directed flat adjacency matrix
    gl::flat_matrix_graph_traits<gl::undirected_t> // undirected flat adjacency matrix
);

TEST_CASE("reconstruct_path should thow if the vertex is not reachable") {
    const std::vector<gl::default_id_type> predecessor_map = {0, 3, 1, gl::invalid_id};
    const auto vertex_id = static_cast<gl::default_id_type>(predecessor_map.size() - 1uz);

    CHECK_THROWS_AS(
        discard_result(gl::algorithm::reconstruct_path(predecessor_map, vertex_id)),
        std::invalid_argument
    );
}

TEST_CASE("reconstruct_path should properly reconstruct the search path to the specified vertex") {
    const std::vector<gl::default_id_type> predecessor_map = {0, 3, 1, 0};

    gl::default_id_type vertex_id;
    std::deque<gl::default_id_type> expected_path;

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
        gl::algorithm::reconstruct_path(predecessor_map, vertex_id), expected_path
    ));
}

TEST_SUITE_END(); // test_alg_dijkstra

} // namespace gl_testing
