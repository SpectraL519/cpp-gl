#include "doctest.h"

#include <hgl/algorithm/properties.hpp>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_alg_properties");

TEST_CASE_TEMPLATE_DEFINE(
    "hypergraph size utility tests", HypergraphTraits, hypergraph_traits_alg_props_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using directional_tag = typename sut_type::directional_tag;

    SUBCASE("utilities on empty hypergraph should return zero or true") {
        sut_type sut;

        // --- Degree Bounds ---
        CHECK_EQ(hgl::algorithm::min_degree(sut), 0uz);
        CHECK_EQ(hgl::algorithm::max_degree(sut), 0uz);

        // --- Size Bounds ---
        CHECK_EQ(hgl::algorithm::rank(sut), 0uz);
        CHECK_EQ(hgl::algorithm::corank(sut), 0uz);

        // --- Regularity/Uniformity ---
        CHECK(hgl::algorithm::is_regular(sut));
        CHECK(hgl::algorithm::is_regular(sut, 0uz));
        CHECK(hgl::algorithm::is_uniform(sut));
        CHECK(hgl::algorithm::is_uniform(sut, 0uz));

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            // --- Degree Bounds ---
            CHECK_EQ(hgl::algorithm::min_out_degree(sut), 0uz);
            CHECK_EQ(hgl::algorithm::max_out_degree(sut), 0uz);
            CHECK_EQ(hgl::algorithm::min_in_degree(sut), 0uz);
            CHECK_EQ(hgl::algorithm::max_in_degree(sut), 0uz);

            // --- Size Bounds ---
            CHECK_EQ(hgl::algorithm::min_tail_size(sut), 0uz);
            CHECK_EQ(hgl::algorithm::max_tail_size(sut), 0uz);
            CHECK_EQ(hgl::algorithm::min_head_size(sut), 0uz);
            CHECK_EQ(hgl::algorithm::max_head_size(sut), 0uz);

            // --- Regularity/Uniformity ---
            CHECK(hgl::algorithm::is_out_regular(sut));
            CHECK(hgl::algorithm::is_in_regular(sut));
            CHECK(hgl::algorithm::is_tail_uniform(sut));
            CHECK(hgl::algorithm::is_head_uniform(sut));
        }
    }

    SUBCASE("utilities on a symmetric topology (Cycle C3) should report constant properties") {
        // Setup: 3 Vertices, 3 Hyperedges forming a cycle.
        // Undirected: Edges are {0,1}, {1,2}, {2,0}.
        // Directed: Edges are 0->1, 1->2, 2->0.
        constexpr auto n_elements = 3uz;
        sut_type sut{n_elements, n_elements};

        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            sut.bind(0uz, 0uz);
            sut.bind(1uz, 0uz); // e0: {0,1}
            sut.bind(1uz, 1uz);
            sut.bind(2uz, 1uz); // e1: {1,2}
            sut.bind(2uz, 2uz);
            sut.bind(0uz, 2uz); // e2: {2,0}

            // Expected: 2-regular, 2-uniform
            CHECK_EQ(hgl::algorithm::max_degree(sut), 2uz);
            CHECK_EQ(hgl::algorithm::min_degree(sut), 2uz);
            CHECK(hgl::algorithm::is_regular(sut));
            CHECK(hgl::algorithm::is_regular(sut, 2uz));
            CHECK_FALSE(hgl::algorithm::is_regular(sut, 1uz));

            CHECK_EQ(hgl::algorithm::rank(sut), 2uz);
            CHECK_EQ(hgl::algorithm::corank(sut), 2uz);
            CHECK(hgl::algorithm::is_uniform(sut));
            CHECK(hgl::algorithm::is_uniform(sut, 2uz));
            CHECK_FALSE(hgl::algorithm::is_uniform(sut, 1uz));
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            sut.bind_tail(0, 0);
            sut.bind_head(1, 0); // e0: 0 -> 1
            sut.bind_tail(1, 1);
            sut.bind_head(2, 1); // e1: 1 -> 2
            sut.bind_tail(2, 2);
            sut.bind_head(0, 2); // e2: 2 -> 0

            // Expected General: Degree 2 (1 in + 1 out), Size 2 (1 tail + 1 head)
            CHECK_EQ(hgl::algorithm::min_degree(sut), 2uz);
            CHECK_EQ(hgl::algorithm::max_degree(sut), 2uz);
            CHECK(hgl::algorithm::is_regular(sut, 2uz));
            CHECK_FALSE(hgl::algorithm::is_regular(sut, 1uz));
            CHECK(hgl::algorithm::is_uniform(sut, 2uz));
            CHECK_FALSE(hgl::algorithm::is_uniform(sut, 1uz));

            // Expected Directed: 1-out-regular, 1-in-regular
            CHECK_EQ(hgl::algorithm::min_out_degree(sut), 1uz);
            CHECK_EQ(hgl::algorithm::max_out_degree(sut), 1uz);
            CHECK(hgl::algorithm::is_out_regular(sut, 1uz));
            CHECK_FALSE(hgl::algorithm::is_out_regular(sut, 2uz));

            CHECK_EQ(hgl::algorithm::min_in_degree(sut), 1uz);
            CHECK_EQ(hgl::algorithm::max_in_degree(sut), 1uz);
            CHECK(hgl::algorithm::is_in_regular(sut, 1uz));
            CHECK_FALSE(hgl::algorithm::is_in_regular(sut, 2uz));

            // Expected Directed Sizes: 1-tail, 1-head
            CHECK_EQ(hgl::algorithm::min_tail_size(sut), 1uz);
            CHECK_EQ(hgl::algorithm::max_tail_size(sut), 1uz);
            CHECK(hgl::algorithm::is_tail_uniform(sut, 1uz));
            CHECK_FALSE(hgl::algorithm::is_tail_uniform(sut, 2uz));

            CHECK_EQ(hgl::algorithm::min_head_size(sut), 1uz);
            CHECK_EQ(hgl::algorithm::max_head_size(sut), 1uz);
            CHECK(hgl::algorithm::is_head_uniform(sut, 1uz));
            CHECK_FALSE(hgl::algorithm::is_head_uniform(sut, 2uz));
        }
    }

    SUBCASE("utilities on asymmetric topology should report divergent bounds and false checks") {
        // Setup: 3 Vertices, 2 Hyperedges.
        // Undirected: e0={0,1,2} (size 3), e1={0} (size 1)
        // Directed:   e0: 0 -> {1,2} (1 tail, 2 heads), e1: {0,1} -> 2 (2 tails, 1 head)
        sut_type sut{3, 2};

        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            sut.bind(0, 0);
            sut.bind(1, 0);
            sut.bind(2, 0); // e0 size 3
            sut.bind(0, 1); // e1 size 1

            // Sizes: 3, 1 -> Non-uniform
            CHECK_EQ(hgl::algorithm::rank(sut), 3uz);
            CHECK_EQ(hgl::algorithm::corank(sut), 1uz);
            CHECK_FALSE(hgl::algorithm::is_uniform(sut));

            // Degrees: v0=2, v1=1, v2=1 -> Irregular
            CHECK_EQ(hgl::algorithm::max_degree(sut), 2uz);
            CHECK_EQ(hgl::algorithm::min_degree(sut), 1uz);
            CHECK_FALSE(hgl::algorithm::is_regular(sut));
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            // e0: 0 -> {1, 2}
            sut.bind_tail(0, 0);
            sut.bind_head(1, 0);
            sut.bind_head(2, 0);

            // e1: {0, 1} -> 2
            sut.bind_tail(0, 1);
            sut.bind_tail(1, 1);
            sut.bind_head(2, 1);

            // --- Size Checks ---
            // Tail sizes: e0=1, e1=2
            CHECK_EQ(hgl::algorithm::max_tail_size(sut), 2uz);
            CHECK_EQ(hgl::algorithm::min_tail_size(sut), 1uz);
            CHECK_FALSE(hgl::algorithm::is_tail_uniform(sut));

            // Head sizes: e0=2, e1=1
            CHECK_EQ(hgl::algorithm::max_head_size(sut), 2uz);
            CHECK_EQ(hgl::algorithm::min_head_size(sut), 1uz);
            CHECK_FALSE(hgl::algorithm::is_head_uniform(sut));

            // --- Degree Checks ---
            // Out degrees: v0(2), v1(1), v2(0)
            CHECK_EQ(hgl::algorithm::max_out_degree(sut), 2uz);
            CHECK_EQ(hgl::algorithm::min_out_degree(sut), 0uz);
            CHECK_FALSE(hgl::algorithm::is_out_regular(sut));

            // In degrees: v0(0), v1(1), v2(2)
            CHECK_EQ(hgl::algorithm::max_in_degree(sut), 2uz);
            CHECK_EQ(hgl::algorithm::min_in_degree(sut), 0uz);
            CHECK_FALSE(hgl::algorithm::is_in_regular(sut));
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_traits_alg_props_template,
    hgl::list_hypergraph_traits<
        hgl::repr::bidirectional_t,
        hgl::undirected_t>, // undirected bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::repr::bidirectional_t,
        hgl::undirected_t>, // undirected bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major flat incidence matrix
    hgl::list_hypergraph_traits<
        hgl::repr::bidirectional_t,
        hgl::bf_directed_t>, // bf-directed bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::repr::bidirectional_t,
        hgl::bf_directed_t>, // bf-directed bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::repr::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::repr::vertex_major_t,
        hgl::bf_directed_t> // bf-directed vertex-major flat incidence matrix
);

TEST_SUITE_END; // test_alg_properties

} // namespace hgl_testing
