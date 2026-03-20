#include "hgl/converters.hpp"
#include "hgl/hypergraph.hpp"
#include "hgl/impl/layout_tags.hpp"

#include <doctest.h>

#include <algorithm>
#include <concepts>
#include <vector>

namespace rng = std::ranges;
namespace vw = std::views;

namespace hgl_testing {

TEST_SUITE_BEGIN("test_converters");

template <
    gl::type_traits::c_instantiation_of<hgl::hypergraph_traits> HypergraphTraits,
    gl::type_traits::c_properties VertexProperties>
using add_vertex_property = hgl::hypergraph_traits<
    typename HypergraphTraits::directional_tag,
    VertexProperties,
    typename HypergraphTraits::hyperedge_properties_type,
    typename HypergraphTraits::implementation_tag>;

template <
    gl::type_traits::c_instantiation_of<hgl::hypergraph_traits> HypergraphTraits,
    gl::type_traits::c_properties HyperedgeProperties>
using add_hyperedge_property = hgl::hypergraph_traits<
    typename HypergraphTraits::directional_tag,
    typename HypergraphTraits::vertex_properties_type,
    HyperedgeProperties,
    typename HypergraphTraits::implementation_tag>;

inline constexpr auto get_id = [](auto&& element) -> gl::types::id_type { return element.id(); };

TEST_CASE_TEMPLATE_DEFINE(
    "Undirected hypergraph converters tests",
    HypergraphTraits,
    undirected_hypergraph_traits_converters_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using graph_type = gl::graph<gl::undirected_graph_traits<>>;

    SUBCASE("projection should produce a clique for each hyperedge") {
        sut_type sut{4ull, 4ull};

        // e0 = {0,1,2}
        sut.bind(0ull, 0ull);
        sut.bind(1ull, 0ull);
        sut.bind(2ull, 0ull);

        // e1 = {1,2,3}
        sut.bind(1ull, 1ull);
        sut.bind(2ull, 1ull);
        sut.bind(3ull, 1ull);

        // e2 = {0,3}
        sut.bind(0ull, 2ull);
        sut.bind(3ull, 2ull);

        // e3 = {0} (should not add any edge)
        sut.bind(0ull, 3ull);

        const auto clique = hgl::projection<graph_type>(sut);

        CHECK_EQ(clique.order(), sut.order());

        const std::vector<std::pair<gl::types::id_type, gl::types::id_type>> expected_edges{
            // e0: (0,1), (0,2), (1,2)
            {0ull, 1ull},
            {0ull, 2ull},
            {1ull, 2ull},
            {1ull, 3ull},
            // e1: (1,2) - exists, (1,3), (2,3)
            {2ull, 3ull},
            {0ull, 3ull},
            // e3: none
        };

        CHECK_EQ(clique.size(), expected_edges.size());
        for (const auto& [u, v] : expected_edges)
            CHECK(clique.has_edge(u, v));
    }

    SUBCASE("incidence_graph should produce a bipartite graph connecting vertices to hyperedges") {
        sut_type sut{4ull, 4ull};

        // e0 = {0,1,2}
        sut.bind(0ull, 0ull);
        sut.bind(1ull, 0ull);
        sut.bind(2ull, 0ull);

        // e1 = {1,2,3}
        sut.bind(1ull, 1ull);
        sut.bind(2ull, 1ull);
        sut.bind(3ull, 1ull);

        // e2 = {0,3}
        sut.bind(0ull, 2ull);
        sut.bind(3ull, 2ull);

        // e3 = {0}
        sut.bind(0ull, 3ull);

        const auto incidence = hgl::incidence_graph<graph_type>(sut);

        CHECK_EQ(incidence.order(), sut.order() + sut.size());

        // Expected edges: vertices 0-3, hyperedges 4-7
        const std::vector<std::pair<gl::types::id_type, gl::types::id_type>> expected_edges{
            // e0 (4): {0,1,2}
            {0ull, 4ull},
            {1ull, 4ull},
            {2ull, 4ull},
            // e1 (5): {1,2,3}
            {1ull, 5ull},
            {2ull, 5ull},
            {3ull, 5ull},
            // e2 (6): {0,3}
            {0ull, 6ull},
            {3ull, 6ull},
            // e3 (7): {0}
            {0ull, 7ull}
        };

        CHECK_EQ(incidence.size(), expected_edges.size());
        for (const auto& [u, v] : expected_edges)
            CHECK(incidence.has_edge(u, v));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    undirected_hypergraph_traits_converters_template,
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::undirected_t>, // undirected bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::undirected_t>, // undirected bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t> // vertex-major incidence matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "BF-directed hypergraph converters tests",
    HypergraphTraits,
    bf_directed_hypergraph_traits_converters_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using graph_type = gl::graph<gl::directed_graph_traits<>>;

    SUBCASE("projection should produce directed edges from tails to heads for each hyperedge") {
        sut_type sut{4ull, 2ull};

        // e0: T={0,1} -> H={2,3}
        sut.bind_tail(0ull, 0ull);
        sut.bind_tail(1ull, 0ull);
        sut.bind_head(2ull, 0ull);
        sut.bind_head(3ull, 0ull);

        // e1: T={2} -> H={0,1}
        sut.bind_tail(2ull, 1ull);
        sut.bind_head(0ull, 1ull);
        sut.bind_head(1ull, 1ull);

        const auto proj = hgl::projection<graph_type>(sut);

        CHECK_EQ(proj.order(), sut.order());

        // Expected directed edges
        const std::vector<std::pair<gl::types::id_type, gl::types::id_type>> expected_edges{
            // e0: 0->2, 0->3, 1->2, 1->3
            {0ull, 2ull},
            {0ull, 3ull},
            {1ull, 2ull},
            {1ull, 3ull},
            // e1: 2->0, 2->1
            {2ull, 0ull},
            {2ull, 1ull}
        };

        CHECK_EQ(proj.size(), expected_edges.size());
        for (const auto& [u, v] : expected_edges)
            CHECK(proj.has_edge(u, v));
    }

    SUBCASE("incidence_graph should produce a directed bipartite graph connecting tails to "
            "hyperedges and hyperedges to heads") {
        sut_type sut{4ull, 2ull};

        // e0: T={0,1} -> H={2,3}
        sut.bind_tail(0ull, 0ull);
        sut.bind_tail(1ull, 0ull);
        sut.bind_head(2ull, 0ull);
        sut.bind_head(3ull, 0ull);

        // e1: T={2} -> H={0,1}
        sut.bind_tail(2ull, 1ull);
        sut.bind_head(0ull, 1ull);
        sut.bind_head(1ull, 1ull);

        const auto incidence = hgl::incidence_graph<graph_type>(sut);

        CHECK_EQ(incidence.order(), sut.order() + sut.size());

        // Expected directed edges: vertices 0-3, hyperedges 4-5
        const std::vector<std::pair<gl::types::id_type, gl::types::id_type>> expected_edges{
            // Tails to hyperedges: 0->4, 1->4, 2->5
            {0ull, 4ull},
            {1ull, 4ull},
            {2ull, 5ull},
            // Hyperedges to heads: 4->2, 4->3, 5->0, 5->1
            {4ull, 2ull},
            {4ull, 3ull},
            {5ull, 0ull},
            {5ull, 1ull}
        };

        CHECK_EQ(incidence.size(), expected_edges.size());
        for (const auto& [u, v] : expected_edges)
            CHECK(incidence.has_edge(u, v));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bf_directed_hypergraph_traits_converters_template,
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
        hgl::bf_directed_t> // vertex-major incidence matrix
);

TEST_SUITE_END(); // test_converters

} // namespace hgl_testing
