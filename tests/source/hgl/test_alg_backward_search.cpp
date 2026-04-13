#include "doctest.h"
#include "testing/common/io.hpp"
#include "testing/hgl/constants.hpp"

#include <hgl/algorithm/traversal/backward_search.hpp>
#include <hgl/constants.hpp>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_alg_backward_search");

TEST_CASE_TEMPLATE_DEFINE(
    "backward_bfs should properly enforce tail dependencies during traversal",
    HypergraphTraitsType,
    backward_bfs_directed_hypergraph_template
) {
    using hypergraph_type = hgl::hypergraph<HypergraphTraitsType>;
    using id_type = typename hypergraph_type::id_type;
    using node_type = hgl::algorithm::search_node<hypergraph_type>;

    hypergraph_type hypergraph;
    std::vector<id_type> root_vertices;

    std::vector<id_type> expected_visit_order;
    std::vector<id_type> expected_pred_map;
    std::vector<id_type> expected_in_hyperedges;
    std::vector<id_type> unreachable_vertices;

    /* Topology:
       V = {v0, v1, v2, v3, v4}
       e0 = {v0, v1} -> {v2}
       e1 = {v2} -> {v3, v4}
       e2 = {v1} -> {v4}

       v0                v3
         \              /
          -->-- v2 -->--
         /              \
       v1 ------->------ v4
    */

    const auto order = 5uz;
    hypergraph.add_vertices(order);
    const auto e0 = hypergraph.add_hyperedge({0u, 1u}, {2u}).id();
    const auto e1 = hypergraph.add_hyperedge({2u}, {3u, 4u}).id();
    const auto e2 = hypergraph.add_hyperedge({1u}, {4u}).id();

    SUBCASE("single root v0 (immediate halt)") {
        root_vertices = {0u};
        expected_visit_order = {0u};
        expected_pred_map.resize(order, hgl::invalid_id);
        expected_pred_map[0uz] = 0u;
        expected_in_hyperedges.resize(order, hgl::invalid_id);
        unreachable_vertices = {1u, 2u, 3u, 4u};
    }

    SUBCASE("roots v0 and v1 (complete traversal)") {
        root_vertices = {0u, 1u};
        expected_visit_order = {0u, 1u, 2u, 4u, 3u};
        // v0->v0(root), v1->v1(root), v1->v2 (e0), v2->v3 (e1), v2->v4 (e2)
        expected_pred_map = {0u, 1u, 1u, 2u, 1u};
        expected_in_hyperedges = {hgl::invalid_id, hgl::invalid_id, e0, e1, e2};
        unreachable_vertices = {};
    }

    CAPTURE(hypergraph);
    CAPTURE(root_vertices);
    CAPTURE(expected_visit_order);
    CAPTURE(expected_pred_map);
    CAPTURE(expected_in_hyperedges);
    CAPTURE(unreachable_vertices);

    // --- noret search ---

    std::vector<id_type> previsit_order;
    std::vector<id_type> postvisit_order;
    std::vector<id_type> noret_pred_map(hypergraph.order(), hgl::invalid_id);
    std::vector<id_type> noret_in_hyperedges(hypergraph.order(), hgl::invalid_id);

    hgl::algorithm::backward_bfs<hgl::algorithm::noret>(
        hypergraph,
        root_vertices,
        [&](const auto& node) {
            previsit_order.push_back(node.vertex_id);
            noret_pred_map[node.vertex_id] = node.pred_id;
            noret_in_hyperedges[node.vertex_id] = node.hyperedge_id;
        },
        [&](const auto& node) { postvisit_order.push_back(node.vertex_id); }
    );

    CHECK_EQ(previsit_order, expected_visit_order);
    CHECK_EQ(postvisit_order, expected_visit_order);
    CHECK_EQ(noret_pred_map, expected_pred_map);
    CHECK_EQ(noret_in_hyperedges, expected_in_hyperedges);

    // --- ret search ---

    const auto search_tree =
        hgl::algorithm::backward_bfs<hgl::algorithm::ret>(hypergraph, root_vertices);

    const auto ret_pred_map =
        search_tree | std::views::transform(&node_type::pred_id) | std::ranges::to<std::vector>();
    const auto ret_in_hyperedges =
        search_tree | std::views::transform(&node_type::hyperedge_id)
        | std::ranges::to<std::vector>();

    CHECK_EQ(ret_pred_map, expected_pred_map);
    CHECK_EQ(ret_in_hyperedges, expected_in_hyperedges);
    CHECK(std::ranges::all_of(expected_visit_order, [&search_tree](const auto v_id) {
        return hgl::algorithm::is_reachable(search_tree, v_id);
    }));
    CHECK(std::ranges::all_of(unreachable_vertices, [&search_tree](const auto v_id) {
        return not hgl::algorithm::is_reachable(search_tree, v_id);
    }));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    backward_bfs_directed_hypergraph_template,
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t>, // bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t>, // bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t> // vertex-major flat incidence matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "backward_dfs should properly enforce tail dependencies during traversal",
    HypergraphTraitsType,
    backward_dfs_directed_hypergraph_template
) {
    using hypergraph_type = hgl::hypergraph<HypergraphTraitsType>;
    using id_type = typename hypergraph_type::id_type;
    using node_type = hgl::algorithm::search_node<hypergraph_type>;

    hypergraph_type hypergraph;
    std::vector<id_type> root_vertices;

    std::vector<id_type> expected_visit_order;
    std::vector<id_type> expected_pred_map;
    std::vector<id_type> expected_in_hyperedges;
    std::vector<id_type> unreachable_vertices;

    /* Topology:
       V = {v0, v1, v2, v3, v4}
       e0 = {v0, v1} -> {v2}
       e1 = {v2} -> {v3, v4}
       e2 = {v1} -> {v4}

       v0                v3
         \              /
          -->-- v2 -->--
         /              \
       v1 ------->------ v4
    */

    const auto order = 5uz;
    hypergraph.add_vertices(order);
    const auto e0 = hypergraph.add_hyperedge({0u, 1u}, {2u}).id();
    const auto e1 = hypergraph.add_hyperedge({2u}, {3u, 4u}).id();
    const auto e2 = hypergraph.add_hyperedge({1u}, {4u}).id();

    SUBCASE("single root v0 (immediate halt)") {
        root_vertices = {0u};
        expected_visit_order = {0u};
        expected_pred_map.resize(order, hgl::invalid_id);
        expected_pred_map[0uz] = 0u;
        expected_in_hyperedges.resize(order, hgl::invalid_id);
        unreachable_vertices = {1u, 2u, 3u, 4u};
    }

    SUBCASE("roots v0 and v1 (complete traversal)") {
        root_vertices = {0u, 1u};

        // DFS Stack Trace (LIFO):
        // Init: [v0, v1]
        // Pop v1 -> unlocks e2 -> pushes v4. e0 wait (needs v0).
        // Pop v4 -> no outgoing.
        // Pop v0 -> unlocks e0 -> pushes v2.
        // Pop v2 -> unlocks e1 -> pushes v3, v4(visited).
        // Pop v3 -> no outgoing.
        expected_visit_order = {1u, 4u, 0u, 2u, 3u};

        // v0->v0(root), v1->v1(root), v2->v0 (via e0), v3->v2 (via e1), v4->v1 (via e2)
        expected_pred_map = {0u, 1u, 0u, 2u, 1u};
        expected_in_hyperedges = {hgl::invalid_id, hgl::invalid_id, e0, e1, e2};

        unreachable_vertices = {};
    }

    CAPTURE(hypergraph);
    CAPTURE(root_vertices);
    CAPTURE(expected_visit_order);
    CAPTURE(expected_pred_map);
    CAPTURE(expected_in_hyperedges);
    CAPTURE(unreachable_vertices);

    // --- noret search ---

    std::vector<id_type> previsit_order;
    std::vector<id_type> postvisit_order;
    std::vector<id_type> noret_pred_map(hypergraph.order(), hgl::invalid_id);
    std::vector<id_type> noret_in_hyperedges(hypergraph.order(), hgl::invalid_id);

    hgl::algorithm::backward_dfs<hgl::algorithm::noret>(
        hypergraph,
        root_vertices,
        [&](const auto& node) {
            previsit_order.push_back(node.vertex_id);
            noret_pred_map[node.vertex_id] = node.pred_id;
            noret_in_hyperedges[node.vertex_id] = node.hyperedge_id;
        },
        [&](const auto& node) { postvisit_order.push_back(node.vertex_id); }
    );

    CHECK_EQ(previsit_order, expected_visit_order);
    CHECK_EQ(postvisit_order, expected_visit_order);
    CHECK_EQ(noret_pred_map, expected_pred_map);
    CHECK_EQ(noret_in_hyperedges, expected_in_hyperedges);

    // --- ret search ---

    const auto search_tree =
        hgl::algorithm::backward_dfs<hgl::algorithm::ret>(hypergraph, root_vertices);

    const auto ret_pred_map =
        search_tree | std::views::transform(&node_type::pred_id) | std::ranges::to<std::vector>();
    const auto ret_in_hyperedges =
        search_tree | std::views::transform(&node_type::hyperedge_id)
        | std::ranges::to<std::vector>();

    CHECK_EQ(ret_pred_map, expected_pred_map);
    CHECK_EQ(ret_in_hyperedges, expected_in_hyperedges);
    CHECK(std::ranges::all_of(expected_visit_order, [&search_tree](const auto v_id) {
        return hgl::algorithm::is_reachable(search_tree, v_id);
    }));
    CHECK(std::ranges::all_of(unreachable_vertices, [&search_tree](const auto v_id) {
        return not hgl::algorithm::is_reachable(search_tree, v_id);
    }));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    backward_dfs_directed_hypergraph_template,
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t>, // bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t>, // bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t> // vertex-major flat incidence matrix
);

TEST_SUITE_END(); // test_alg_backward_search

} // namespace hgl_testing
