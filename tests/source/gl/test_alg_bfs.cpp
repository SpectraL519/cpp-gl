#include "doctest.h"
#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm.hpp>
#include <gl/topology.hpp>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_bfs");

TEST_CASE_TEMPLATE_DEFINE(
    "breadth_first_search no return should properly traverse the graph",
    GraphType,
    bfs_no_return_graph_template
) {
    using graph_type = GraphType;
    using id_type = typename graph_type::id_type;
    using vertex_type = typename GraphType::vertex_type;

    graph_type graph;
    std::vector<id_type> expected_previsit_order;

    SUBCASE("empty graph") {
        graph = gl::topology::clique<graph_type>(0uz);
        expected_previsit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(1uz);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        for (id_type id = gl::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
    }

    SUBCASE("path graph") {
        graph = gl::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (id_type id = gl::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
    }

    SUBCASE("biclique") {
        /*
        A = {0, 1, 2}
        B = {3, 4}
        root = 0 -> connected to B -> connected to A (root already visited)
        */
        graph = gl::topology::biclique<graph_type>(3uz, 2uz);
        expected_previsit_order = {0, 3, 4, 1, 2};
    }

    CAPTURE(graph);
    CAPTURE(expected_previsit_order);

    std::vector<gl::default_id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<gl::default_id_type> previsit_order, postvisit_order;
    const auto vertex_properties = graph.vertex_properties_map();
    gl::algorithm::breadth_first_search<gl::algorithm::noret>(
        graph,
        gl::algorithm::no_root,
        [&](const auto node) { // previsit
            previsit_order.push_back(node.vertex_id);
        },
        [&](const auto node) { // postvisit
            postvisit_order.push_back(node.vertex_id);
            vertex_properties[node.vertex_id].visited = true;
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
    CHECK(std::ranges::all_of(
        graph.vertices(), std::identity{}, vertex_visited_projection<vertex_type>{}
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bfs_no_return_graph_template,
    gl::graph<gl::list_graph_traits<gl::directed_t, visited_property>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<
        gl::undirected_t,
        visited_property>>, // undirected adjacency list
    gl::graph<gl::flat_list_graph_traits<
        gl::directed_t,
        visited_property>>, // directed flat adjacency list
    gl::graph<gl::flat_list_graph_traits<
        gl::undirected_t,
        visited_property>>, // undirected flat adjacency list
    gl::graph<gl::matrix_graph_traits<
        gl::directed_t,
        visited_property>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<
        gl::undirected_t,
        visited_property>>, // undirected adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<
        gl::directed_t,
        visited_property>>, // directed flat adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<
        gl::undirected_t,
        visited_property>> // undirected flat adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "breadth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    bfs_no_return_with_root_graph_template
) {
    using graph_type = GraphType;
    using id_type = typename graph_type::id_type;

    graph_type graph;
    gl::default_id_type root_vertex_id = gl::invalid_id;
    std::deque<gl::default_id_type> expected_previsit_order;

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(1uz);
        root_vertex_id = constants::v1_id;
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id = constants::v3_id;

        for (id_type id = gl::initial_id; id < constants::n_elements_alg; id++) {
            if (id != constants::v3_id)
                expected_previsit_order.push_back(id);
        }
        expected_previsit_order.push_front(constants::v3_id);
    }

    CAPTURE(graph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);

    std::deque<gl::default_id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<gl::default_id_type> previsit_order, postvisit_order;
    gl::algorithm::breadth_first_search<gl::algorithm::noret>(
        graph,
        root_vertex_id,
        [&](const auto node) { // previsit
            previsit_order.push_back(node.vertex_id);
        },
        [&](const auto node) { // postvisit
            postvisit_order.push_back(node.vertex_id);
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bfs_no_return_with_root_graph_template,
    gl::graph<gl::list_graph_traits<gl::directed_t>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<gl::undirected_t>>, // undirected adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::directed_t>>, // directed flat adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::undirected_t>>, // undirected flat adjacency list
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>>, // undirected adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::directed_t>>, // directed flat adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::undirected_t>> // undirected flat adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "breadth_first_search with return should properly traverse the graph",
    GraphType,
    bfs_return_graph_template
) {
    using graph_type = GraphType;

    const auto graph = gl::topology::regular_binary_tree<graph_type>(constants::depth);
    const auto pred_map = gl::algorithm::breadth_first_search<gl::algorithm::ret>(graph);

    // verify the predecessors of each vertex
    REQUIRE_EQ(pred_map.size(), graph.n_vertices());
    CHECK(std::ranges::all_of(graph.vertex_ids(), has_correct_bin_predecessor(pred_map)));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bfs_return_graph_template,
    gl::graph<gl::list_graph_traits<gl::directed_t>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<gl::undirected_t>>, // undirected adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::directed_t>>, // directed flat adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::undirected_t>>, // undirected flat adjacency list
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>>, // undirected adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::directed_t>>, // directed flat adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::undirected_t>> // undirected flat adjacency matrix
);

TEST_SUITE_END(); // test_alg_bfs

} // namespace gl_testing
