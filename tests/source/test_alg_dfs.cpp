#include "alg_common.hpp"
#include "constants.hpp"

#include <gl/algorithms.hpp>
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
    std::deque<lib_t::id_type> expected_previsit_order;

    SUBCASE("empty graph") {
        graph = lib::topology::clique<graph_type>(constants::zero_elements);
        expected_previsit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::one_element);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = lib::topology::clique<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id + constants::one; id < constants::n_elements_alg;
             id++)
            expected_previsit_order.push_front(id);
        expected_previsit_order.push_front(lib::constants::initial_id);
    }

    SUBCASE("path graph") {
        graph = lib::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
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
        graph = lib::topology::biclique<graph_type>(constants::three, constants::two);
        expected_previsit_order = {0, 4, 2, 3, 1};
    }

    CAPTURE(graph);
    CAPTURE(expected_previsit_order);

    std::deque<lib_t::id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<lib_t::id_type> previsit_order, postvisit_order;
    lib::algorithm::depth_first_search(
        graph,
        lib::algorithm::no_root_vertex,
        [&](const auto& vertex) { // previsit
            previsit_order.push_back(vertex.id());
        },
        [&](const auto& vertex) { // postvisit
            postvisit_order.push_back(vertex.id());
            vertex.properties.visited = true;
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
    lib::graph<
        lib::list_graph_traits<lib::directed_t, types::visited_property>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<
        lib::undirected_t,
        types::visited_property>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<
        lib::directed_t,
        types::visited_property>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<
        lib::undirected_t,
        types::visited_property>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    dfs_no_return_with_root_graph_template
) {
    using graph_type = GraphType;

    graph_type graph;
    std::optional<lib_t::id_type> root_vertex_id;
    std::deque<lib_t::id_type> expected_previsit_order;

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::one_element);
        root_vertex_id.emplace(constants::vertex_id_1);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = lib::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id.emplace(constants::vertex_id_3);

        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++) {
            if (id != constants::vertex_id_3)
                expected_previsit_order.push_front(id);
        }
        expected_previsit_order.push_front(constants::vertex_id_3);
    }

    CAPTURE(graph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);

    std::deque<lib_t::id_type> expected_postvisit_order = expected_previsit_order;

    std::vector<lib_t::id_type> previsit_order, postvisit_order;
    lib::algorithm::depth_first_search(
        graph,
        root_vertex_id,
        [&](const auto& vertex) { // previsit
            previsit_order.push_back(vertex.id());
        },
        [&](const auto& vertex) { // postvisit
            postvisit_order.push_back(vertex.id());
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_no_return_with_root_graph_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search with return should properly traverse the graph",
    GraphType,
    dfs_return_graph_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;

    const auto graph = lib::topology::regular_binary_tree<graph_type>(constants::three);
    const auto search_tree = lib::algorithm::depth_first_search<graph_type>(graph);

    REQUIRE_EQ(search_tree.n_vertices(), graph.n_vertices());

    const auto vertex_id_view = std::views::iota(constants::first_element_idx, graph.n_vertices());

    // verify that for each vertex the edges are equivalent to those in the original graph
    CHECK(
        std::ranges::all_of(vertex_id_view, alg_common::are_vertices_equivalent(graph, search_tree))
    );
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_return_graph_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
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
    std::vector<lib_t::id_type> expected_previsit_order;

    SUBCASE("empty graph") {
        graph = lib::topology::clique<graph_type>(constants::zero_elements);
        expected_previsit_order = {};
    }

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::one_element);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = lib::topology::clique<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
            expected_previsit_order.push_back(id);
    }

    SUBCASE("path graph") {
        graph = lib::topology::bidirectional_path<graph_type>(constants::n_elements_alg);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++)
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
        graph = lib::topology::biclique<graph_type>(constants::three, constants::two);
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

    std::vector<lib_t::id_type> previsit_order, postvisit_order;
    lib::algorithm::recursive_depth_first_search(
        graph,
        lib::algorithm::no_root_vertex,
        [&](const auto& vertex) { // previsit
            previsit_order.push_back(vertex.id());
        },
        [&](const auto& vertex) { // postvisit
            postvisit_order.push_back(vertex.id());
            vertex.properties.visited = true;
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
    lib::graph<
        lib::list_graph_traits<lib::directed_t, types::visited_property>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<
        lib::undirected_t,
        types::visited_property>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<
        lib::directed_t,
        types::visited_property>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<
        lib::undirected_t,
        types::visited_property>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "recursive_depth_first_search no return with root vertex should properly traverse the graph",
    GraphType,
    rdfs_no_return_with_root_graph_template
) {
    using graph_type = GraphType;

    graph_type graph;
    std::optional<lib_t::id_type> root_vertex_id;
    std::vector<lib_t::id_type> expected_previsit_order;

    SUBCASE("single vertex graph") {
        graph = lib::topology::clique<graph_type>(constants::one_element);
        root_vertex_id.emplace(constants::vertex_id_1);
        expected_previsit_order = {0};
    }

    SUBCASE("clique") {
        graph = lib::topology::clique<graph_type>(constants::n_elements_alg);
        root_vertex_id.emplace(constants::vertex_id_3);

        expected_previsit_order.push_back(constants::vertex_id_3);
        for (auto id = lib::constants::initial_id; id < constants::n_elements_alg; id++) {
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

    std::vector<lib_t::id_type> previsit_order, postvisit_order;
    lib::algorithm::recursive_depth_first_search(
        graph,
        root_vertex_id,
        [&](const auto& vertex) { // previsit
            previsit_order.push_back(vertex.id());
        },
        [&](const auto& vertex) { // postvisit
            postvisit_order.push_back(vertex.id());
        }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    rdfs_no_return_with_root_graph_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "recursive_depth_first_search with return should properly traverse the graph",
    GraphType,
    rdfs_return_graph_template
) {
    using graph_type = GraphType;
    using vertex_type = typename graph_type::vertex_type;

    const auto graph = lib::topology::regular_binary_tree<graph_type>(constants::three);
    const auto search_tree = lib::algorithm::recursive_depth_first_search<graph_type>(graph);

    REQUIRE_EQ(search_tree.n_vertices(), graph.n_vertices());

    const auto vertex_id_view = std::views::iota(constants::first_element_idx, graph.n_vertices());

    // verify that for each vertex the edges are equivalent to those in the original graph
    CHECK(
        std::ranges::all_of(vertex_id_view, alg_common::are_vertices_equivalent(graph, search_tree))
    );
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    rdfs_return_graph_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adjacency list
    lib::graph<lib::list_graph_traits<lib::undirected_t>>, // undirected adjacency list
    lib::graph<lib::matrix_graph_traits<lib::directed_t>>, // directed adjacency matrix
    lib::graph<lib::matrix_graph_traits<lib::undirected_t>> // undirected adjacency matrix
);

TEST_SUITE_END(); // test_alg_dfs

} // namespace gl_testing
