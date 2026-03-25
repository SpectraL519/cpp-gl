#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"

#include <gl/algorithm.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <cmath>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_mst");

TEST_CASE_TEMPLATE_DEFINE(
    "(edge heap) prim mst finding tests for graphs with explicitly weighted edges",
    TraitsType,
    edge_heap_prim_wieghted_edge_traits_type_template
) {
    using sut_type = gl::graph<TraitsType>;
    using weight_type = typename sut_type::edge_properties_type::weight_type;
    using distance_type = weight_type;

    static_assert(gl::traits::c_weight_properties_type<typename sut_type::edge_properties_type>);

    SUBCASE("should return a proper mst descriptor for a valid graph") {
        using vertex_id_pair = std::pair<gl::id_type, gl::id_type>;

        sut_type sut;
        gl::id_type source_id;
        std::vector<vertex_id_pair> expected_edges;
        distance_type expected_weight;

        SUBCASE("regular binary tree") {
            sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);
            source_id = 0uz;

            const weight_type edge_weight = 3;
            for (const auto vertex_id : sut.vertex_ids()) {
                for (const auto& edge : sut.adjacent_edges(vertex_id)) {
                    edge.properties().weight = edge_weight;
                    expected_edges.emplace_back(edge.source(), edge.target());
                }
            }

            expected_weight = edge_weight * (sut.order() - 1uz);
        }

        SUBCASE("custom graph") {
            const fs::path gsf_file_path = data_path / "mst_graph.gsf";

            sut = gl::io::load<sut_type>(gsf_file_path);
            source_id = 0uz;

            const fs::path edges_file_path = data_path / "mst_edges.txt";
            const auto n_vertex_ids = (sut.order() - 1uz) * 2uz;
            const auto vertex_id_list = load_list<gl::id_type>(n_vertex_ids, edges_file_path);
            for (auto i = 0uz; i < n_vertex_ids; i += 2uz)
                expected_edges.emplace_back(vertex_id_list[i], vertex_id_list[i + 1]);

            const fs::path weight_file_path = data_path / "mst_weight.txt";
            expected_weight = load_list<weight_type>(1uz, weight_file_path).front();
        }

        CAPTURE(sut);
        CAPTURE(source_id);
        CAPTURE(expected_edges);
        CAPTURE(expected_weight);

        const auto mst = gl::algorithm::edge_heap_prim_mst(sut, source_id);

        REQUIRE_EQ(mst.edges.size(), sut.order() - 1uz);
        REQUIRE_EQ(mst.weight, expected_weight);

        CHECK(std::ranges::all_of(mst.edges, [&expected_edges](const auto& edge) {
            const auto [source_id, target_id] = edge.incident_vertices();
            return std::find_if(
                       expected_edges.begin(),
                       expected_edges.end(),
                       [&](const vertex_id_pair& vids) {
                           return (source_id == vids.first && target_id == vids.second)
                               or (source_id == vids.second && target_id == vids.first);
                       }
                   )
                != expected_edges.end();
        }));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_heap_prim_wieghted_edge_traits_type_template,
    gl::undirected_graph_traits<
        gl::empty_properties,
        gl::weight_property<>,
        gl::impl::list_t>, // undirected adjacency list graph
    gl::undirected_graph_traits<
        gl::empty_properties,
        gl::weight_property<>,
        gl::impl::flat_list_t>, // undirected flat adjacency list graph
    gl::undirected_graph_traits<
        gl::empty_properties,
        gl::weight_property<>,
        gl::impl::matrix_t> // undirected adjacency matrix graph
);

TEST_CASE_TEMPLATE_DEFINE(
    "(edge heap) prim mst finding tests for graphs with unweighted edges",
    TraitsType,
    edge_heap_prim_unwieghted_edge_traits_type_template
) {
    using sut_type = gl::graph<TraitsType>;
    using distance_type = gl::default_vertex_distance_type;
    using weight_type = distance_type;
    using vertex_id_pair = std::pair<gl::id_type, gl::id_type>;

    static_assert(not gl::traits::c_weight_properties_type<typename sut_type::edge_properties_type>);

    const auto sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);
    const gl::id_type source_id = 0uz;

    std::vector<vertex_id_pair> expected_edges;
    for (const auto vertex_id : sut.vertex_ids())
        for (const auto& edge : sut.adjacent_edges(vertex_id))
            expected_edges.emplace_back(edge.source(), edge.target());

    const weight_type expected_weight = sut.order() - 1uz;

    const auto mst = gl::algorithm::edge_heap_prim_mst(sut, source_id);

    REQUIRE_EQ(mst.edges.size(), sut.order() - 1uz);
    REQUIRE_EQ(mst.weight, expected_weight);

    CHECK(std::ranges::all_of(mst.edges, [&expected_edges](const auto& edge) {
        const auto [source_id, target_id] = edge.incident_vertices();
        return std::find_if(
                   expected_edges.begin(),
                   expected_edges.end(),
                   [&](const vertex_id_pair& vids) {
                       return (source_id == vids.first && target_id == vids.second)
                           or (source_id == vids.second && target_id == vids.first);
                   }
               )
            != expected_edges.end();
    }));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    edge_heap_prim_unwieghted_edge_traits_type_template,
    gl::list_graph_traits<gl::undirected_t>, // undirected adjacency list graph
    gl::flat_list_graph_traits<gl::undirected_t>, // undirected flat adjacency list graph
    gl::matrix_graph_traits<gl::undirected_t> // undirected adjacency matrix graph
);

TEST_CASE_TEMPLATE_DEFINE(
    "(vertex heap) prim mst finding tests for graphs with explicitly weighted edges",
    TraitsType,
    vertex_heap_prim_wieghted_edge_traits_type_template
) {
    using sut_type = gl::graph<TraitsType>;
    using weight_type = typename sut_type::edge_properties_type::weight_type;
    using distance_type = weight_type;

    static_assert(gl::traits::c_weight_properties_type<typename sut_type::edge_properties_type>);

    SUBCASE("should return a proper mst descriptor for a valid graph") {
        using vertex_id_pair = std::pair<gl::id_type, gl::id_type>;

        sut_type sut;
        gl::id_type source_id;
        std::vector<vertex_id_pair> expected_edges;
        distance_type expected_weight;

        SUBCASE("regular binary tree") {
            sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);
            source_id = 0uz;

            const weight_type edge_weight = 3;
            for (const auto vertex_id : sut.vertex_ids()) {
                for (const auto& edge : sut.adjacent_edges(vertex_id)) {
                    edge.properties().weight = edge_weight;
                    expected_edges.emplace_back(edge.source(), edge.target());
                }
            }

            expected_weight = edge_weight * (sut.order() - 1uz);
        }

        SUBCASE("custom graph") {
            const fs::path gsf_file_path = data_path / "mst_graph.gsf";

            sut = gl::io::load<sut_type>(gsf_file_path);
            source_id = 0uz;

            const fs::path edges_file_path = data_path / "mst_edges.txt";
            const auto n_vertex_ids = (sut.order() - 1uz) * 2uz;
            const auto vertex_id_list = load_list<gl::id_type>(n_vertex_ids, edges_file_path);
            for (auto i = 0uz; i < n_vertex_ids; i += 2uz)
                expected_edges.emplace_back(vertex_id_list[i], vertex_id_list[i + 1]);

            const fs::path weight_file_path = data_path / "mst_weight.txt";
            expected_weight = load_list<weight_type>(1uz, weight_file_path).front();
        }

        CAPTURE(sut);
        CAPTURE(source_id);
        CAPTURE(expected_edges);
        CAPTURE(expected_weight);

        const auto mst = gl::algorithm::vertex_heap_prim_mst(sut, source_id);

        REQUIRE_EQ(mst.edges.size(), sut.order() - 1uz);
        REQUIRE_EQ(mst.weight, expected_weight);

        CHECK(std::ranges::all_of(mst.edges, [&expected_edges](const auto& edge) {
            const auto [source_id, target_id] = edge.incident_vertices();
            return std::find_if(
                       expected_edges.begin(),
                       expected_edges.end(),
                       [&](const vertex_id_pair& vids) {
                           return (source_id == vids.first && target_id == vids.second)
                               or (source_id == vids.second && target_id == vids.first);
                       }
                   )
                != expected_edges.end();
        }));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    vertex_heap_prim_wieghted_edge_traits_type_template,
    gl::undirected_graph_traits<
        gl::empty_properties,
        gl::weight_property<>,
        gl::impl::list_t>, // undirected adjacency list graph
    gl::undirected_graph_traits<
        gl::empty_properties,
        gl::weight_property<>,
        gl::impl::flat_list_t>, // undirected flat adjacency list graph
    gl::undirected_graph_traits<
        gl::empty_properties,
        gl::weight_property<>,
        gl::impl::matrix_t> // undirected adjacency matrix graph
);

TEST_CASE_TEMPLATE_DEFINE(
    "(vertex heap) prim mst finding tests for graphs with unweighted edges",
    TraitsType,
    vertex_heap_prim_unwieghted_edge_traits_type_template
) {
    using sut_type = gl::graph<TraitsType>;
    using distance_type = gl::default_vertex_distance_type;
    using weight_type = distance_type;
    using vertex_id_pair = std::pair<gl::id_type, gl::id_type>;

    static_assert(not gl::traits::c_weight_properties_type<typename sut_type::edge_properties_type>);

    const auto sut = gl::topology::regular_binary_tree<sut_type>(constants::depth);
    const gl::id_type source_id = 0uz;

    std::vector<vertex_id_pair> expected_edges;
    for (const auto vertex_id : sut.vertex_ids())
        for (const auto& edge : sut.adjacent_edges(vertex_id))
            expected_edges.emplace_back(edge.source(), edge.target());

    const weight_type expected_weight = sut.order() - 1uz;

    const auto mst = gl::algorithm::vertex_heap_prim_mst(sut, source_id);

    REQUIRE_EQ(mst.edges.size(), sut.order() - 1uz);
    REQUIRE_EQ(mst.weight, expected_weight);

    CHECK(std::ranges::all_of(mst.edges, [&expected_edges](const auto& edge) {
        const auto [source_id, target_id] = edge.incident_vertices();
        return std::find_if(
                   expected_edges.begin(),
                   expected_edges.end(),
                   [&](const vertex_id_pair& vids) {
                       return (source_id == vids.first && target_id == vids.second)
                           or (source_id == vids.second && target_id == vids.first);
                   }
               )
            != expected_edges.end();
    }));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    vertex_heap_prim_unwieghted_edge_traits_type_template,
    gl::list_graph_traits<gl::undirected_t>, // undirected adjacency list graph
    gl::flat_list_graph_traits<gl::undirected_t>, // undirected flat adjacency list graph
    gl::matrix_graph_traits<gl::undirected_t> // undirected adjacency matrix graph
);

TEST_SUITE_END(); // test_alg_mst

} // namespace gl_testing
