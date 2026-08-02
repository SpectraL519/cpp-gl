#include "doctest.h"
#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm.hpp>
#include <gl/topology.hpp>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_dfs");

// --- iterative dfs tests ---

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search no return should properly traverse the graph",
    GraphType,
    dfs_no_return_graph_template
) {
    using graph_type = GraphType;
    using id_type = typename GraphType::id_type;
    using vertex_type = typename GraphType::vertex_type;

    graph_type graph;
    std::vector<id_type> expected_previsit_order, expected_postvisit_order;

    SUBCASE("empty graph") {
        graph = gl::topology::clique<graph_type>(0uz);
        expected_previsit_order = {};
        expected_postvisit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(1uz);
        expected_previsit_order = {0};
        expected_postvisit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);

        expected_previsit_order.push_back(constants::v1_id);
        for (auto i = constants::n_elements_alg; i > constants::v2_id; i--)
            expected_previsit_order.push_back(static_cast<id_type>(i - 1));

        expected_postvisit_order = expected_previsit_order;
        std::ranges::reverse(expected_postvisit_order);
    }

    SUBCASE("path graph") {
        graph = gl::topology::bidirectional_path<graph_type>(constants::n_elements_alg);

        for (auto id = constants::v1_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);

        expected_postvisit_order = expected_previsit_order;
        std::ranges::reverse(expected_postvisit_order);
    }

    SUBCASE("biclique") {
        /*
        A = {0, 1, 2}
        B = {3, 4}
        [s: <stack state without already visited vertices>]
        -> root = 0 -> connected to B [s: 4 3]
        -> 4 connected to A (root visited) [s: 2 1 3]
        -> 2 connected to B (4 visited) [s: 3 1 3]
        finally: 0 -> 4 -> 2 -> 1 -> 3
        */
        graph = gl::topology::biclique<graph_type>(3uz, 2uz);
        expected_previsit_order = {0, 4, 2, 3, 1};
        expected_postvisit_order = {1, 3, 2, 4, 0};
    }

    SUBCASE("regular binary tree") {
        /*
        Depth = 3 (7 vertices: 0 to 6)
        [s: <stack state without already visited vertices>]
        -> root = 0 -> connected to left (1), right (2). [s: 2 1]
        -> pop 2 -> connected to left (5), right (6). [s: 6 5 1]
        -> pop 6 (leaf) -> pop 5 (leaf)
        -> pop 1 -> connected to left (3), right (4). [s: 4 3]
        -> pop 4 (leaf) -> pop 3 (leaf)
        */
        graph = gl::topology::regular_binary_tree<graph_type>(3uz);
        expected_previsit_order = {0, 2, 6, 5, 1, 4, 3};
        expected_postvisit_order = {6, 5, 2, 4, 3, 1, 0};
    }

    CAPTURE(graph);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);

    std::vector<id_type> previsit_order, postvisit_order;
    const auto vertex_properties = graph.vertex_properties_map();
    gl::algorithm::depth_first_search<gl::algorithm::noret>(
        graph,
        gl::algorithm::no_root,
        [&](const id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const id_type vertex_id) { // postvisit
            postvisit_order.push_back(vertex_id);
            vertex_properties[vertex_id].visited = true;
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
    CHECK(std::ranges::all_of(
        graph.vertices(), std::identity{}, vertex_visited_projection<vertex_type>{}
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_no_return_graph_template,
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
    "depth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    dfs_no_return_with_root_graph_template
) {
    using graph_type = GraphType;
    using id_type = typename GraphType::id_type;

    graph_type graph;
    id_type root_vertex_id;
    std::vector<id_type> expected_previsit_order, expected_postvisit_order;

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(1uz);
        root_vertex_id = constants::v1_id;
        expected_previsit_order = {0};
        expected_postvisit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id = constants::v3_id;

        expected_previsit_order.push_back(constants::v3_id);
        for (auto i = constants::n_elements_alg; i > constants::v1_id; i--) {
            const auto id = static_cast<id_type>(i - 1);
            if (id != constants::v3_id)
                expected_previsit_order.push_back(id);
        }

        expected_postvisit_order = expected_previsit_order;
        std::ranges::reverse(expected_postvisit_order);
    }

    SUBCASE("regular binary tree") {
        graph = gl::topology::regular_binary_tree<graph_type>(3uz);
        root_vertex_id = 0;
        expected_previsit_order = {0, 2, 6, 5, 1, 4, 3};
        expected_postvisit_order = {6, 5, 2, 4, 3, 1, 0};
    }

    CAPTURE(graph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);

    std::vector<id_type> previsit_order, postvisit_order;
    gl::algorithm::depth_first_search<gl::algorithm::noret>(
        graph,
        root_vertex_id,
        [&](const id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const id_type vertex_id) { // postvisit
            postvisit_order.push_back(vertex_id);
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_no_return_with_root_graph_template,
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
    "depth_first_search with return should properly traverse the graph",
    GraphType,
    dfs_return_graph_template
) {
    using graph_type = GraphType;

    const auto graph = gl::topology::regular_binary_tree<graph_type>(constants::depth);
    const auto pred_map = gl::algorithm::depth_first_search<gl::algorithm::ret>(graph);

    // verify the predecessors of each vertex
    REQUIRE_EQ(pred_map.size(), graph.n_vertices());
    CHECK(std::ranges::all_of(graph.vertex_ids(), has_correct_bin_predecessor(pred_map)));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_return_graph_template,
    gl::graph<gl::list_graph_traits<gl::directed_t>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<gl::undirected_t>>, // undirected adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::directed_t>>, // directed flat adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::undirected_t>>, // undirected flat adjacency list
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>>, // undirected adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::directed_t>>, // directed flat adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::undirected_t>> // undirected flat adjacency matrix
);

// --- recursive dfs tests ---

TEST_CASE_TEMPLATE_DEFINE(
    "recursive_depth_first_search no return should properly traverse the graph",
    GraphType,
    rdfs_no_return_graph_template
) {
    using graph_type = GraphType;
    using id_type = typename GraphType::id_type;
    using vertex_type = typename GraphType::vertex_type;

    graph_type graph;
    std::vector<id_type> expected_previsit_order, expected_postvisit_order;

    SUBCASE("empty graph") {
        graph = gl::topology::clique<graph_type>(0uz);
        expected_previsit_order = {};
        expected_postvisit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(1uz);
        expected_previsit_order = {0};
        expected_postvisit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        for (auto id = constants::v1_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);

        expected_postvisit_order = expected_previsit_order;
        std::ranges::reverse(expected_postvisit_order);
    }

    SUBCASE("path graph") {
        graph = gl::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (auto id = constants::v1_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);

        expected_postvisit_order = expected_previsit_order;
        std::ranges::reverse(expected_postvisit_order);
    }

    SUBCASE("biclique") {
        /*
        A = {0, 1, 2}
        B = {3, 4}
        (<min not visited incident vertex>)
        -> root = 0 -> connected to B (3)
        -> 3 connected to A (1)
        -> 1 connected to B (4)
        -> 4 connected to A (2)
        finally: 0 -> 3 -> 1 -> 4 -> 2
        */
        graph = gl::topology::biclique<graph_type>(3uz, 2uz);
        expected_previsit_order = {0, 3, 1, 4, 2};
        expected_postvisit_order = {2, 4, 1, 3, 0};
    }

    SUBCASE("regular binary tree") {
        /*
        Depth = 3 (7 vertices: 0 to 6)
        -> root = 0 -> recursively calls left child (1)
        -> 1 -> recursively calls left child (3)
        -> 3 is leaf -> returns -> 1 calls right child (4)
        -> 4 is leaf -> returns -> 1 returns -> 0 calls right child (2)
        -> 2 -> recursively calls left child (5)
        -> 5 is leaf -> returns -> 2 calls right child (6)
        */
        graph = gl::topology::regular_binary_tree<graph_type>(3uz);
        expected_previsit_order = {0, 1, 3, 4, 2, 5, 6};
        expected_postvisit_order = {3, 4, 1, 5, 6, 2, 0};
    }

    CAPTURE(graph);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);

    std::vector<id_type> previsit_order, postvisit_order;
    const auto vertex_properties = graph.vertex_properties_map();
    gl::algorithm::recursive_depth_first_search<gl::algorithm::noret>(
        graph,
        gl::algorithm::no_root,
        [&](const id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const id_type vertex_id) { // postvisit
            postvisit_order.push_back(vertex_id);
            vertex_properties[vertex_id].visited = true;
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
    CHECK(std::ranges::all_of(
        graph.vertices(), std::identity{}, vertex_visited_projection<vertex_type>{}
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    rdfs_no_return_graph_template,
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
    "recursive_depth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    rdfs_no_return_with_root_graph_template
) {
    using graph_type = GraphType;
    using id_type = typename GraphType::id_type;

    graph_type graph;
    id_type root_vertex_id = gl::invalid_id;
    std::vector<id_type> expected_previsit_order, expected_postvisit_order;

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(1uz);
        root_vertex_id = constants::v1_id;
        expected_previsit_order = {0};
        expected_postvisit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id = constants::v3_id;

        expected_previsit_order.push_back(constants::v3_id);
        for (auto id = constants::v1_id; id < constants::n_elements_alg; id++) {
            if (id != constants::v3_id)
                expected_previsit_order.push_back(id);
        }

        expected_postvisit_order = expected_previsit_order;
        std::ranges::reverse(expected_postvisit_order);
    }

    SUBCASE("regular binary tree") {
        graph = gl::topology::regular_binary_tree<graph_type>(3uz);
        root_vertex_id = 0;
        expected_previsit_order = {0, 1, 3, 4, 2, 5, 6};
        expected_postvisit_order = {3, 4, 1, 5, 6, 2, 0};
    }

    CAPTURE(graph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);

    std::vector<id_type> previsit_order, postvisit_order;
    gl::algorithm::recursive_depth_first_search<gl::algorithm::noret>(
        graph,
        root_vertex_id,
        [&](const id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const id_type vertex_id) { // postvisit
            postvisit_order.push_back(vertex_id);
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    rdfs_no_return_with_root_graph_template,
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
    "recursive_depth_first_search with return should properly traverse the graph",
    GraphType,
    rdfs_return_graph_template
) {
    using graph_type = GraphType;

    const auto graph = gl::topology::regular_binary_tree<graph_type>(constants::depth);
    const auto pred_map = gl::algorithm::recursive_depth_first_search<gl::algorithm::ret>(graph);

    // verify the predecessors of each vertex
    REQUIRE_EQ(pred_map.size(), graph.n_vertices());
    CHECK(std::ranges::all_of(graph.vertex_ids(), has_correct_bin_predecessor(pred_map)));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    rdfs_return_graph_template,
    gl::graph<gl::list_graph_traits<gl::directed_t>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<gl::undirected_t>>, // undirected adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::directed_t>>, // directed flat adjacency list
    gl::graph<gl::flat_list_graph_traits<gl::undirected_t>>, // undirected flat adjacency list
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>>, // undirected adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::directed_t>>, // directed flat adjacency matrix
    gl::graph<gl::flat_matrix_graph_traits<gl::undirected_t>> // undirected flat adjacency matrix
);

TEST_SUITE_END(); // test_alg_dfs

} // namespace gl_testing
