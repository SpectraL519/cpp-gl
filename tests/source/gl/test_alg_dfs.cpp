#include "testing/gl/alg_utils.hpp"
#include "testing/gl/constants.hpp"

#include <gl/algorithm.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_dfs");

// --- iterative dfs tests ---

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search no return should properly traverse the graph",
    GraphType,
    dfs_no_return_graph_template
) {
    using graph_type = GraphType;
    using vertex_type = typename GraphType::vertex_type;

    graph_type graph;
    std::deque<gl::types::id_type> expected_previsit_order;

    SUBCASE("empty graph") {
        graph = gl::topology::clique<graph_type>(constants::zero_elements);
        expected_previsit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(constants::one_element);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        for (auto id = gl::constants::initial_id + constants::one; id < constants::n_elements_alg;
             id++)
            expected_previsit_order.push_front(id);
        expected_previsit_order.push_front(gl::constants::initial_id);
    }

    SUBCASE("path graph") {
        graph = gl::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (auto id = gl::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
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
        graph = gl::topology::biclique<graph_type>(constants::three, constants::two);
        expected_previsit_order = {0, 4, 2, 3, 1};
    }

    CAPTURE(graph);
    CAPTURE(expected_previsit_order);

    std::deque<gl::types::id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<gl::types::id_type> previsit_order, postvisit_order;
    const auto vertex_properties = graph.vertex_properties_map();
    gl::algorithm::depth_first_search<gl::algorithm::noret>(
        graph,
        gl::algorithm::no_root_vertex,
        [&](const gl::types::id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const gl::types::id_type vertex_id) { // postvisit
            postvisit_order.push_back(vertex_id);
            vertex_properties[vertex_id].visited = true;
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
    CHECK(std::ranges::all_of(
        graph.vertices(), std::identity{}, alg_common::vertex_visited_projection<vertex_type>{}
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_no_return_graph_template,
    gl::graph<
        gl::list_graph_traits<gl::directed_t, types::visited_property>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<
        gl::undirected_t,
        types::visited_property>>, // undirected adjacency list
    gl::graph<gl::matrix_graph_traits<
        gl::directed_t,
        types::visited_property>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<
        gl::undirected_t,
        types::visited_property>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    dfs_no_return_with_root_graph_template
) {
    using graph_type = GraphType;

    graph_type graph;
    std::optional<gl::types::id_type> root_vertex_id;
    std::deque<gl::types::id_type> expected_previsit_order;

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(constants::one_element);
        root_vertex_id.emplace(constants::vertex_id_1);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id.emplace(constants::vertex_id_3);

        for (auto id = gl::constants::initial_id; id < constants::n_elements_alg; id++) {
            if (id != constants::vertex_id_3)
                expected_previsit_order.push_front(id);
        }
        expected_previsit_order.push_front(constants::vertex_id_3);
    }

    CAPTURE(graph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);

    std::deque<gl::types::id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<gl::types::id_type> previsit_order, postvisit_order;
    gl::algorithm::depth_first_search<gl::algorithm::noret>(
        graph,
        root_vertex_id,
        [&](const gl::types::id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const gl::types::id_type vertex_id) { // postvisit
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
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search with return should properly traverse the graph",
    GraphType,
    dfs_return_graph_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;

    const auto graph = gl::topology::regular_binary_tree<graph_type>(constants::three);
    const auto pd = gl::algorithm::depth_first_search<gl::algorithm::ret, graph_type>(graph);

    // verify the predecessors of each vertex
    REQUIRE_EQ(pd.predecessors.size(), graph.n_vertices());
    CHECK(std::ranges::all_of(graph.vertex_ids(), alg_common::has_correct_bin_predecessor(pd)));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_return_graph_template,
    gl::graph<gl::list_graph_traits<gl::directed_t>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<gl::undirected_t>>, // undirected adjacency list
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>> // undirected adjacency matrix
);

// --- recursive dfs tests ---

TEST_CASE_TEMPLATE_DEFINE(
    "recursive_depth_first_search no return should properly traverse the graph",
    GraphType,
    rdfs_no_return_graph_template
) {
    using graph_type = GraphType;
    using vertex_type = typename GraphType::vertex_type;

    graph_type graph;
    std::vector<gl::types::id_type> expected_previsit_order;

    SUBCASE("empty graph") {
        graph = gl::topology::clique<graph_type>(constants::zero_elements);
        expected_previsit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(constants::one_element);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        for (auto id = gl::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
    }

    SUBCASE("path graph") {
        graph = gl::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (auto id = gl::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
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
        graph = gl::topology::biclique<graph_type>(constants::three, constants::two);
        expected_previsit_order = {0, 3, 1, 4, 2};
    }

    CAPTURE(graph);
    CAPTURE(expected_previsit_order);

    /*
    post visit order should be reverse of pre visit order
    because the algorithm will search the graph recursively and call
    post visit after return from the recursive call
    */
    const auto expected_postvisit_order = std::views::reverse(expected_previsit_order);

    std::vector<gl::types::id_type> previsit_order, postvisit_order;
    const auto vertex_properties = graph.vertex_properties_map();
    gl::algorithm::recursive_depth_first_search<gl::algorithm::noret>(
        graph,
        gl::algorithm::no_root_vertex,
        [&](const gl::types::id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const gl::types::id_type vertex_id) { // postvisit
            postvisit_order.push_back(vertex_id);
            vertex_properties[vertex_id].visited = true;
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
    CHECK(std::ranges::all_of(
        graph.vertices(), std::identity{}, alg_common::vertex_visited_projection<vertex_type>{}
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    rdfs_no_return_graph_template,
    gl::graph<
        gl::list_graph_traits<gl::directed_t, types::visited_property>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<
        gl::undirected_t,
        types::visited_property>>, // undirected adjacency list
    gl::graph<gl::matrix_graph_traits<
        gl::directed_t,
        types::visited_property>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<
        gl::undirected_t,
        types::visited_property>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "recursive_depth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    rdfs_no_return_with_root_graph_template
) {
    using graph_type = GraphType;

    graph_type graph;
    std::optional<gl::types::id_type> root_vertex_id;
    std::vector<gl::types::id_type> expected_previsit_order;

    SUBCASE("single vertex graph") {
        graph = gl::topology::clique<graph_type>(constants::one_element);
        root_vertex_id.emplace(constants::vertex_id_1);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = gl::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id.emplace(constants::vertex_id_3);

        expected_previsit_order.push_back(constants::vertex_id_3);
        for (auto id = gl::constants::initial_id; id < constants::n_elements_alg; id++) {
            if (id != constants::vertex_id_3)
                expected_previsit_order.push_back(id);
        }
    }

    CAPTURE(graph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);

    /*
    post visit order should be reverse of pre visit order
    because the algorithm will search the graph recursively and call
    post visit after return from the recursive call
    */
    const auto expected_postvisit_order = std::views::reverse(expected_previsit_order);

    std::vector<gl::types::id_type> previsit_order, postvisit_order;
    gl::algorithm::recursive_depth_first_search<gl::algorithm::noret>(
        graph,
        root_vertex_id,
        [&](const gl::types::id_type vertex_id) { // previsit
            previsit_order.push_back(vertex_id);
        },
        [&](const gl::types::id_type vertex_id) { // postvisit
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
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "recursive_depth_first_search with return should properly traverse the graph",
    GraphType,
    rdfs_return_graph_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;

    const auto graph = gl::topology::regular_binary_tree<graph_type>(constants::three);
    const auto pd =
        gl::algorithm::recursive_depth_first_search<gl::algorithm::ret, graph_type>(graph);

    // verify the predecessors of each vertex
    REQUIRE_EQ(pd.predecessors.size(), graph.n_vertices());
    CHECK(std::ranges::all_of(graph.vertex_ids(), alg_common::has_correct_bin_predecessor(pd)));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    rdfs_return_graph_template,
    gl::graph<gl::list_graph_traits<gl::directed_t>>, // directed adjacency list
    gl::graph<gl::list_graph_traits<gl::undirected_t>>, // undirected adjacency list
    gl::graph<gl::matrix_graph_traits<gl::directed_t>>, // directed adjacency matrix
    gl::graph<gl::matrix_graph_traits<gl::undirected_t>> // undirected adjacency matrix
);

TEST_SUITE_END(); // test_alg_dfs

} // namespace gl_testing
