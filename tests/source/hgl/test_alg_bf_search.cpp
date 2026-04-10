#include "doctest.h"
#include "testing/common/io.hpp"
#include "testing/hgl/constants.hpp"

#include <hgl/algorithm/traversal/bf_search.hpp>
#include <hgl/constants.hpp>

#include <algorithm>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_alg_bfs");

TEST_CASE_TEMPLATE_DEFINE(
    "backward_search should properly enforce tail dependencies during traversal",
    HypergraphTraitsType,
    backward_search_directed_hypergraph_template
) {
    using hypergraph_type = hgl::hypergraph<HypergraphTraitsType>;
    using id_type = typename hypergraph_type::id_type;
    using node_type = hgl::algorithm::search_node<hypergraph_type>;

    hypergraph_type hypergraph;
    std::vector<id_type> root_vertices;

    std::vector<id_type> expected_previsit_order;
    std::vector<id_type> expected_postvisit_order;
    std::vector<id_type> expected_pred_map;
    std::vector<id_type> expected_in_hyperedges;

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

    const auto e0 = hypergraph.add_hyperedge().id();
    hypergraph.bind_tail(0uz, e0);
    hypergraph.bind_tail(1uz, e0);
    hypergraph.bind_head(2uz, e0);

    const auto e1 = hypergraph.add_hyperedge().id();
    hypergraph.bind_tail(2uz, e1);
    hypergraph.bind_head(3uz, e1);
    hypergraph.bind_head(4uz, e1);

    const auto e2 = hypergraph.add_hyperedge().id();
    hypergraph.bind_tail(1uz, e2);
    hypergraph.bind_head(4uz, e2);

    SUBCASE("single root v0 (immediate halt)") {
        root_vertices = {0u};
        expected_previsit_order = {0u};
        expected_postvisit_order = {0u};
        expected_pred_map.resize(order, hgl::invalid_id);
        expected_pred_map[0uz] = 0u;
        expected_in_hyperedges.resize(order, hgl::invalid_id);
    }

    SUBCASE("roots v0 and v1 (complete traversal)") {
        root_vertices = {0u, 1u};
        expected_previsit_order = {0u, 1u, 2u, 4u, 3u};
        expected_postvisit_order = expected_previsit_order;
        // v0->v0(root), v1->v1(root), v1->v2 (e0), v2->v3 (e1), v2->v4 (e2)
        expected_pred_map = {0u, 1u, 1u, 2u, 1u};
        expected_in_hyperedges = {hgl::invalid_id, hgl::invalid_id, e0, e1, e2};
    }

    CAPTURE(hypergraph);
    CAPTURE(root_vertices);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);
    CAPTURE(expected_pred_map);
    CAPTURE(expected_in_hyperedges);

    // --- noret search ---

    std::vector<id_type> previsit_order;
    std::vector<id_type> postvisit_order;
    std::vector<id_type> noret_pred_map(hypergraph.order(), hgl::invalid_id);
    std::vector<id_type> noret_in_hyperedges(hypergraph.order(), hgl::invalid_id);

    hgl::algorithm::backward_search<gl::algorithm::noret>(
        hypergraph,
        root_vertices,
        [&](const auto& node) {
            previsit_order.push_back(node.vertex_id);
            noret_pred_map[node.vertex_id] = node.pred_id;
            noret_in_hyperedges[node.vertex_id] = node.hyperedge_id;
        },
        [&](const auto& node) { postvisit_order.push_back(node.vertex_id); }
    );

    CHECK_EQ(previsit_order, expected_previsit_order);
    CHECK_EQ(postvisit_order, expected_postvisit_order);
    CHECK_EQ(noret_pred_map, expected_pred_map);
    CHECK_EQ(noret_in_hyperedges, expected_in_hyperedges);

    // --- ret search ---

    const auto search_tree =
        hgl::algorithm::backward_search<gl::algorithm::ret>(hypergraph, root_vertices);

    const auto ret_pred_map =
        search_tree | std::views::transform(&node_type::pred_id) | std::ranges::to<std::vector>();
    const auto ret_in_hyperedges =
        search_tree | std::views::transform(&node_type::hyperedge_id)
        | std::ranges::to<std::vector>();

    CHECK_EQ(ret_pred_map, expected_pred_map);
    CHECK_EQ(ret_in_hyperedges, expected_in_hyperedges);
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    backward_search_directed_hypergraph_template,
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
    "forward_search should properly enforce head dependencies during traversal",
    HypergraphTraitsType,
    forward_search_directed_hypergraph_template
) {
    using hypergraph_type = hgl::hypergraph<HypergraphTraitsType>;
    using id_type = typename hypergraph_type::id_type;
    using node_type = hgl::algorithm::search_node<hypergraph_type>;

    hypergraph_type hypergraph;
    std::vector<id_type> root_vertices;

    std::vector<id_type> expected_previsit_order;
    std::vector<id_type> expected_postvisit_order;
    std::vector<id_type> expected_pred_map;
    std::vector<id_type> expected_in_hyperedges;

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

    const auto e0 = hypergraph.add_hyperedge().id();
    hypergraph.bind_tail(0uz, e0);
    hypergraph.bind_tail(1uz, e0);
    hypergraph.bind_head(2uz, e0);

    const auto e1 = hypergraph.add_hyperedge().id();
    hypergraph.bind_tail(2uz, e1);
    hypergraph.bind_head(3uz, e1);
    hypergraph.bind_head(4uz, e1);

    const auto e2 = hypergraph.add_hyperedge().id();
    hypergraph.bind_tail(1uz, e2);
    hypergraph.bind_head(4uz, e2);

    SUBCASE("single root v4 (partial backward traversal)") {
        // Rooting at v4. e1 cannot be traversed backwards because it also requires v3.
        // e2 CAN be traversed because v4 is its only head.
        root_vertices = {4u};

        expected_previsit_order = {4u, 1u};
        expected_postvisit_order = expected_previsit_order;

        expected_pred_map.resize(order, hgl::invalid_id);
        expected_pred_map[4uz] = 4u; // Root
        expected_pred_map[1uz] = 4u; // v1 reached backward from v4 via e2

        expected_in_hyperedges.resize(order, hgl::invalid_id);
        expected_in_hyperedges[1uz] = e2;
    }

    SUBCASE("roots v3 and v4 (complete backward traversal)") {
        // Rooting at v3 and v4 unlocks e1, which unlocks v2, which unlocks e0, which unlocks v0 and v1.
        root_vertices = {3u, 4u};

        // Queue trace:
        // Init: [v3, v4]
        // Pop v3 -> sees e1 (needs v4, so wait)
        // Pop v4 -> sees e1 (unlocked -> enqueues v2), sees e2 (unlocked -> enqueues v1)
        // Queue: [v2, v1]
        // Pop v2 -> sees e0 (unlocked -> enqueues v0, v1) - v1 is already visited
        // Queue: [v1, v0]
        // Pop v1 -> no incoming edges
        // Pop v0 -> no incoming edges
        expected_previsit_order = {3u, 4u, 2u, 1u, 0u};
        expected_postvisit_order = expected_previsit_order;

        expected_pred_map = {
            2u, // v0 reached backward from v2 via e0
            4u, // v1 reached backward from v4 via e2
            4u, // v2 reached backward from v4 via e1
            3u, // v3 is a root
            4u // v4 is a root
        };

        expected_in_hyperedges = {
            e0, // v0 reached via e0
            e2, // v1 reached via e2
            e1, // v2 reached via e1
            hgl::invalid_id, // v3 root
            hgl::invalid_id // v4 root
        };
    }

    CAPTURE(hypergraph);
    CAPTURE(root_vertices);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);
    CAPTURE(expected_pred_map);
    CAPTURE(expected_in_hyperedges);

    // --- noret search ---

    std::vector<id_type> previsit_order;
    std::vector<id_type> postvisit_order;
    std::vector<id_type> noret_pred_map(hypergraph.order(), hgl::invalid_id);
    std::vector<id_type> noret_in_hyperedges(hypergraph.order(), hgl::invalid_id);

    hgl::algorithm::forward_search<gl::algorithm::noret>(
        hypergraph,
        root_vertices,
        [&](const auto& node) {
            previsit_order.push_back(node.vertex_id);
            noret_pred_map[node.vertex_id] = node.pred_id;
            noret_in_hyperedges[node.vertex_id] = node.hyperedge_id;
        },
        [&](const auto& node) { postvisit_order.push_back(node.vertex_id); }
    );

    CHECK_EQ(previsit_order, expected_previsit_order);
    CHECK_EQ(postvisit_order, expected_postvisit_order);
    CHECK_EQ(noret_pred_map, expected_pred_map);
    CHECK_EQ(noret_in_hyperedges, expected_in_hyperedges);

    // --- ret search ---

    const auto search_tree =
        hgl::algorithm::forward_search<gl::algorithm::ret>(hypergraph, root_vertices);

    const auto ret_pred_map =
        search_tree | std::views::transform(&node_type::pred_id) | std::ranges::to<std::vector>();
    const auto ret_in_hyperedges =
        search_tree | std::views::transform(&node_type::hyperedge_id)
        | std::ranges::to<std::vector>();

    CHECK_EQ(ret_pred_map, expected_pred_map);
    CHECK_EQ(ret_in_hyperedges, expected_in_hyperedges);
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    forward_search_directed_hypergraph_template,
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

TEST_SUITE_END(); // test_alg_bfs

} // namespace hgl_testing
