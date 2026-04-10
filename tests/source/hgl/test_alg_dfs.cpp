#include "doctest.h"
#include "testing/common/io.hpp"
#include "testing/hgl/constants.hpp"

#include <hgl/algorithm/traversal/depth_first_search.hpp>
#include <hgl/constants.hpp>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_alg_dfs");

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search should properly traverse the hypergraph and yield correct search trees",
    HypergraphTraitsType,
    dfs_undirected_hypergraph_traits_template
) {
    using hypergraph_type = hgl::hypergraph<HypergraphTraitsType>;
    using id_type = typename hypergraph_type::id_type;
    using node_type = hgl::algorithm::search_node<hypergraph_type>;

    hypergraph_type hypergraph;
    id_type root_vertex_id;

    std::vector<id_type> expected_previsit_order;
    std::vector<id_type> expected_postvisit_order;
    std::vector<id_type> expected_pred_map;
    std::vector<id_type> expected_in_hyperedges;
    // TODO: unreachable vertices

    SUBCASE("hub (single hyperedge)") {
        hypergraph.add_vertices(4uz);
        const auto e = hypergraph.add_hyperedge().id();
        for (const auto v : hypergraph.vertex_ids())
            hypergraph.bind(v, e);

        root_vertex_id = hgl::initial_id;

        expected_previsit_order = {0u, 3u, 2u, 1u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 0u, 0u};
        expected_in_hyperedges = {hgl::invalid_id, e, e, e};
    }

    SUBCASE("chain") {
        // E(H) = {{0, 1}, {1, 2, 3}, {3, 4, 5}}
        hypergraph.add_vertices(6uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        for (const id_type v : {0u, 1u})
            hypergraph.bind(v, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        for (const id_type v : {1u, 2u, 3u})
            hypergraph.bind(v, e1);

        const auto e2 = hypergraph.add_hyperedge().id();
        for (const id_type v : {3u, 4u, 5u})
            hypergraph.bind(v, e2);

        root_vertex_id = hgl::initial_id;

        // 0->1; 1->2,3; 3->4,5; 5; 4; 2
        expected_previsit_order = {0u, 1u, 3u, 5u, 4u, 2u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 1u, 1u, 3u, 3u};
        expected_in_hyperedges = {hgl::invalid_id, e0, e1, e1, e2, e2};
    }

    SUBCASE("overlapping hyperedges (shortest path preference)") {
        // E(H) = {{0, 1}, {1, 2, 3}, {0, 3}, {3, 4}}
        hypergraph.add_vertices(5uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        for (const id_type v : {0u, 1u})
            hypergraph.bind(v, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        for (const id_type v : {1u, 2u, 3u})
            hypergraph.bind(v, e1);

        const auto e2 = hypergraph.add_hyperedge().id();
        for (const id_type v : {0u, 3u})
            hypergraph.bind(v, e2);

        const auto e3 = hypergraph.add_hyperedge().id();
        for (const id_type v : {3u, 4u})
            hypergraph.bind(v, e3);

        root_vertex_id = hgl::initial_id;

        // 0->1,3; 3->1,2,4; 4; 2; 1
        expected_previsit_order = {0u, 3u, 4u, 2u, 1u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 3u, 3u, 0u, 3u};
        expected_in_hyperedges = {hgl::invalid_id, e1, e1, e2, e3};
    }

    SUBCASE("disconnected components (targeted root)") {
        // E(H) = {{0, 1, 2}, {3, 4}}
        hypergraph.add_vertices(5uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        for (const id_type v : {0u, 1u, 2u})
            hypergraph.bind(v, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        for (const id_type v : {3u, 4u})
            hypergraph.bind(v, e1);

        root_vertex_id = hgl::initial_id;

        // 0->1,2; 2; 1
        expected_previsit_order = {0u, 2u, 1u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 0u, hgl::invalid_id, hgl::invalid_id};
        expected_in_hyperedges = {hgl::invalid_id, e0, e0, hgl::invalid_id, hgl::invalid_id};
    }

    SUBCASE("full graph traversal (no_root)") {
        // E(H) = {{0, 1, 2}, {3, 4}}
        hypergraph.add_vertices(5uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        for (const id_type v : {0u, 1u, 2u})
            hypergraph.bind(v, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        for (const id_type v : {3u, 4u})
            hypergraph.bind(v, e1);

        root_vertex_id = gl::algorithm::no_root;

        // 0->1,2; 2; 1; 3->4; 4
        expected_previsit_order = {0u, 2u, 1u, 3u, 4u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 0u, 3u, 3u};
        expected_in_hyperedges = {hgl::invalid_id, e0, e0, hgl::invalid_id, e1};
    }

    CAPTURE(hypergraph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);
    CAPTURE(expected_pred_map);
    CAPTURE(expected_in_hyperedges);

    // --- noret bfs ---

    std::vector<id_type> previsit_order;
    std::vector<id_type> postvisit_order;
    std::vector<id_type> noret_pred_map(hypergraph.order(), hgl::invalid_id);
    std::vector<id_type> noret_in_hyperedges(hypergraph.order(), hgl::invalid_id);

    hgl::algorithm::depth_first_search<gl::algorithm::noret>(
        hypergraph,
        root_vertex_id,
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

    // --- ret bfs ---

    const auto search_tree =
        hgl::algorithm::depth_first_search<gl::algorithm::ret>(hypergraph, root_vertex_id);

    const auto ret_pred_map =
        search_tree | std::views::transform(&node_type::pred_id) | std::ranges::to<std::vector>();
    const auto ret_in_hyperedges =
        search_tree | std::views::transform(&node_type::hyperedge_id)
        | std::ranges::to<std::vector>();

    CHECK_EQ(ret_pred_map, expected_pred_map);
    CHECK_EQ(ret_in_hyperedges, expected_in_hyperedges);
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_undirected_hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::undirected_t>, // bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::undirected_t>, // bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t> // vertex-major flat incidence matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "depth_first_search should properly traverse the directed hypergraph and yield correct search "
    "trees",
    HypergraphTraitsType,
    dfs_bf_directed_hypergraph_traits_template
) {
    using hypergraph_type = hgl::hypergraph<HypergraphTraitsType>;
    using id_type = typename hypergraph_type::id_type;
    using node_type = hgl::algorithm::search_node<hypergraph_type>;

    hypergraph_type hypergraph;
    id_type root_vertex_id;

    std::vector<id_type> expected_previsit_order;
    std::vector<id_type> expected_postvisit_order;
    std::vector<id_type> expected_pred_map;
    std::vector<id_type> expected_in_hyperedges;

    SUBCASE("forward star (single hyperedge)") {
        // E(H) = {0->{1, 2, 3}}
        hypergraph.add_vertices(4uz);
        const auto e = hypergraph.add_hyperedge().id();

        hypergraph.bind_tail(0uz, e);
        for (const id_type v : {1u, 2u, 3u})
            hypergraph.bind_head(v, e);

        root_vertex_id = hgl::initial_id;

        expected_previsit_order = {0u, 3u, 2u, 1u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 0u, 0u};
        expected_in_hyperedges = {hgl::invalid_id, e, e, e};
    }

    SUBCASE("chain") {
        // E(H) = {0->1, 1->{2, 3}, 3->{4, 5}}
        hypergraph.add_vertices(6uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(0uz, e0);
        hypergraph.bind_head(1uz, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(1uz, e1);
        for (const id_type v : {2u, 3u})
            hypergraph.bind_head(v, e1);

        const auto e2 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(3uz, e2);
        for (const id_type v : {4u, 5u})
            hypergraph.bind_head(v, e2);

        root_vertex_id = hgl::initial_id;

        // 0->1; 1->2,3; 3->4,5; 5; 4; 2
        expected_previsit_order = {0u, 1u, 3u, 5u, 4u, 2u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 1u, 1u, 3u, 3u};
        expected_in_hyperedges = {hgl::invalid_id, e0, e1, e1, e2, e2};
    }

    SUBCASE("overlapping hyperedges (shortest path preference)") {
        // E(H) = {0->1, 1->{2, 3}, 0->3 (shortcut!), 3->4}
        hypergraph.add_vertices(5uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(0uz, e0);
        hypergraph.bind_head(1uz, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(1uz, e1);
        for (const id_type v : {2u, 3u})
            hypergraph.bind_head(v, e1);

        const auto e2 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(0uz, e2);
        hypergraph.bind_head(3uz, e2);

        const auto e3 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(3uz, e3);
        hypergraph.bind_head(4uz, e3);

        root_vertex_id = hgl::initial_id;

        // 0->1,3; 3->4; 4; 1->2; 2
        expected_previsit_order = {0u, 3u, 4u, 1u, 2u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 1u, 0u, 3u};
        expected_in_hyperedges = {hgl::invalid_id, e0, e1, e2, e3};
    }

    SUBCASE("disconnected components (targeted root)") {
        // E(H) = {0->{1, 2}, 3->4}
        hypergraph.add_vertices(5uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(0uz, e0);
        for (const id_type v : {1u, 2u})
            hypergraph.bind_head(v, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(3uz, e1);
        hypergraph.bind_head(4uz, e1);

        root_vertex_id = hgl::initial_id;

        // 0->1,2; 2; 1
        expected_previsit_order = {0u, 2u, 1u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 0u, hgl::invalid_id, hgl::invalid_id};
        expected_in_hyperedges = {hgl::invalid_id, e0, e0, hgl::invalid_id, hgl::invalid_id};
    }

    SUBCASE("full graph traversal (no_root)") {
        // E(H) = {0->{1, 2}, 3->4}
        hypergraph.add_vertices(5uz);

        const auto e0 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(0uz, e0);
        for (const id_type v : {1u, 2u})
            hypergraph.bind_head(v, e0);

        const auto e1 = hypergraph.add_hyperedge().id();
        hypergraph.bind_tail(3uz, e1);
        hypergraph.bind_head(4uz, e1);

        root_vertex_id = gl::algorithm::no_root;

        // 0->1,2; 2; 1; 3->4; 4
        expected_previsit_order = {0u, 2u, 1u, 3u, 4u};
        expected_postvisit_order = expected_previsit_order;
        expected_pred_map = {0u, 0u, 0u, 3u, 3u};
        expected_in_hyperedges = {hgl::invalid_id, e0, e0, hgl::invalid_id, e1};
    }

    CAPTURE(hypergraph);
    CAPTURE(root_vertex_id);
    CAPTURE(expected_previsit_order);
    CAPTURE(expected_postvisit_order);
    CAPTURE(expected_pred_map);
    CAPTURE(expected_in_hyperedges);

    // --- noret bfs ---

    std::vector<id_type> previsit_order;
    std::vector<id_type> postvisit_order;
    std::vector<id_type> noret_pred_map(hypergraph.order(), hgl::invalid_id);
    std::vector<id_type> noret_in_hyperedges(hypergraph.order(), hgl::invalid_id);

    hgl::algorithm::depth_first_search<gl::algorithm::noret>(
        hypergraph,
        root_vertex_id,
        [&](const auto& node) {
            previsit_order.push_back(node.vertex_id);
            noret_pred_map[node.vertex_id] = node.pred_id;
            noret_in_hyperedges[node.vertex_id] = node.hyperedge_id;
        },
        [&](const auto& node) { postvisit_order.push_back(node.vertex_id); }
    );

    CHECK(std::ranges::equal(previsit_order, expected_previsit_order));
    CHECK(std::ranges::equal(postvisit_order, expected_postvisit_order));
    CHECK(std::ranges::equal(noret_pred_map, expected_pred_map));
    CHECK(std::ranges::equal(noret_in_hyperedges, expected_in_hyperedges));

    // --- ret bfs ---

    const auto search_tree =
        hgl::algorithm::depth_first_search<gl::algorithm::ret>(hypergraph, root_vertex_id);

    const auto ret_pred_map =
        search_tree | std::views::transform(&node_type::pred_id) | std::ranges::to<std::vector>();
    const auto ret_in_hyperedges =
        search_tree | std::views::transform(&node_type::hyperedge_id)
        | std::ranges::to<std::vector>();

    CHECK(std::ranges::equal(ret_pred_map, expected_pred_map));
    CHECK(std::ranges::equal(ret_in_hyperedges, expected_in_hyperedges));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    dfs_bf_directed_hypergraph_traits_template,
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

TEST_SUITE_END(); // test_alg_dfs

} // namespace hgl_testing
