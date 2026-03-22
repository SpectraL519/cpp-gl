#include "hgl/conversion.hpp"
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

struct test_hypergraph_conversion {
    using property_type = hgl::types::name_property;

    template <hgl::type_traits::c_undirected_hypergraph HypergraphType>
    [[nodiscard]] HypergraphType create_test_hypergraph() {
        HypergraphType h(4uz, 3uz);

        h.bind(0uz, 0uz);
        h.bind(1uz, 0uz);
        h.bind(2uz, 0uz);
        h.bind(1uz, 1uz);
        h.bind(2uz, 1uz);
        h.bind(3uz, 1uz);
        h.bind(0uz, 2uz);
        h.bind(3uz, 2uz);

        this->set_properties(h);
        return h;
    }

    template <hgl::type_traits::c_bf_directed_hypergraph HypergraphType>
    [[nodiscard]] HypergraphType create_test_hypergraph() {
        HypergraphType h(4uz, 2uz);

        h.bind_tail(0uz, 0uz);
        h.bind_tail(1uz, 0uz);
        h.bind_head(2uz, 0uz);
        h.bind_head(3uz, 0uz);

        h.bind_tail(2uz, 1uz);
        h.bind_head(0uz, 1uz);
        h.bind_head(1uz, 1uz);

        this->set_properties(h);
        return h;
    }

    void set_properties(hgl::type_traits::c_hypergraph auto& h) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : h.vertex_ids())
                h.get_vertex_properties(vid) = property_type("vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::hyperedge_properties_type,
                          property_type>)
            for (const auto& eid : h.hyperedge_ids())
                h.get_hyperedge_properties(eid) = property_type("hyperedge_" + std::to_string(eid));
    }

    void validate_hypergraph(const hgl::type_traits::c_undirected_hypergraph auto& h) {
        REQUIRE_EQ(h.order(), 4uz);
        REQUIRE_EQ(h.size(), 3uz);
        CHECK(h.are_incident(0uz, 0uz));
        CHECK(h.are_incident(1uz, 0uz));
        CHECK(h.are_incident(2uz, 0uz));
        CHECK(h.are_incident(1uz, 1uz));
        CHECK(h.are_incident(2uz, 1uz));
        CHECK(h.are_incident(3uz, 1uz));
        CHECK(h.are_incident(0uz, 2uz));
        CHECK(h.are_incident(3uz, 2uz));

        CHECK_FALSE(h.are_incident(3uz, 0uz));
        CHECK_FALSE(h.are_incident(0uz, 1uz));
        CHECK_FALSE(h.are_incident(1uz, 2uz));

        this->validate_properties(h);
    }

    void validate_hypergraph(const hgl::type_traits::c_bf_directed_hypergraph auto& h) {
        REQUIRE_EQ(h.order(), 4uz);
        REQUIRE_EQ(h.size(), 2uz);

        CHECK(h.is_tail(0uz, 0uz));
        CHECK(h.is_tail(1uz, 0uz));
        CHECK(h.is_head(2uz, 0uz));
        CHECK(h.is_head(3uz, 0uz));

        CHECK(h.is_tail(2uz, 1uz));
        CHECK(h.is_head(0uz, 1uz));
        CHECK(h.is_head(1uz, 1uz));

        CHECK_FALSE(h.is_head(0uz, 0uz));
        CHECK_FALSE(h.is_tail(3uz, 0uz));
        CHECK_FALSE(h.is_tail(0uz, 1uz));
        CHECK_FALSE(h.is_head(2uz, 1uz));

        this->validate_properties(h);
    }

    void validate_properties(const hgl::type_traits::c_hypergraph auto& h) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : h.vertex_ids())
                CHECK_EQ(h.get_vertex_properties(vid), "vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::hyperedge_properties_type,
                          property_type>)
            for (const auto& eid : h.hyperedge_ids())
                CHECK_EQ(h.get_hyperedge_properties(eid), "hyperedge_" + std::to_string(eid));
    }
};

TEST_CASE_TEMPLATE_DEFINE(
    "Hypergraph Representation Model Conversion Tests", HypergraphParams, hypergraph_params_template
) {
    using DT = std::tuple_element_t<0, HypergraphParams>;
    using LT = std::tuple_element_t<1, HypergraphParams>;
    using VP = std::tuple_element_t<2, HypergraphParams>;
    using EP = std::tuple_element_t<3, HypergraphParams>;

    using list_hypergraph = hgl::hypergraph<hgl::list_hypergraph_traits<LT, DT, VP, EP>>;
    using flat_list_hypergraph = hgl::hypergraph<hgl::flat_list_hypergraph_traits<LT, DT, VP, EP>>;
    using matrix_hypergraph = hgl::hypergraph<hgl::matrix_hypergraph_traits<LT, DT, VP, EP>>;

    using target_list_tag = typename list_hypergraph::implementation_tag;
    using target_flat_list_tag = typename flat_list_hypergraph::implementation_tag;
    using target_matrix_tag = typename matrix_hypergraph::implementation_tag;

    test_hypergraph_conversion fixture;

    SUBCASE("source hypergraph model: list") {
        auto source_hypergraph = fixture.create_test_hypergraph<list_hypergraph>();

        SUBCASE("identity conversion") {
            const auto converted_hypergraph =
                hgl::to<target_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("flat-list conversion") {
            const auto converted_hypergraph =
                hgl::to<target_flat_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("matrix conversion") {
            const auto converted_hypergraph =
                hgl::to<target_matrix_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }
    }

    SUBCASE("source hypergraph model: flat list") {
        auto source_hypergraph = fixture.create_test_hypergraph<flat_list_hypergraph>();

        SUBCASE("identity conversion") {
            const auto converted_hypergraph =
                hgl::to<target_flat_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("list conversion") {
            const auto converted_hypergraph =
                hgl::to<target_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("matrix conversion") {
            const auto converted_hypergraph =
                hgl::to<target_matrix_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }
    }

    SUBCASE("source hypergraph model: matrix") {
        auto source_hypergraph = fixture.create_test_hypergraph<matrix_hypergraph>();

        SUBCASE("identity conversion") {
            const auto converted_hypergraph =
                hgl::to<target_matrix_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("list conversion") {
            const auto converted_hypergraph =
                hgl::to<target_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("flat-list conversion") {
            const auto converted_hypergraph =
                hgl::to<target_flat_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_params_template,
    std::tuple<hgl::undirected_t, hgl::impl::hyperedge_major_t, hgl::types::empty_properties, hgl::types::empty_properties>, // undirected, hyperedge-major, no properties
    std::tuple<hgl::bf_directed_t, hgl::impl::hyperedge_major_t, hgl::types::empty_properties, hgl::types::empty_properties>, // bf-directed, hyperedge-major, no properties
    std::tuple<hgl::undirected_t, hgl::impl::vertex_major_t, hgl::types::empty_properties, hgl::types::empty_properties>, // undirected, vertex-major, no properties
    std::tuple<hgl::bf_directed_t, hgl::impl::vertex_major_t, hgl::types::empty_properties, hgl::types::empty_properties>, // bf-directed, vertex-major, no properties
    std::tuple<hgl::undirected_t, hgl::impl::hyperedge_major_t, hgl::types::name_property, hgl::types::empty_properties>, // undirected, hyperedge-major, vertex properties
    std::tuple<hgl::bf_directed_t, hgl::impl::hyperedge_major_t, hgl::types::name_property, hgl::types::empty_properties>, // bf-directed, hyperedge-major, vertex properties
    std::tuple<hgl::undirected_t, hgl::impl::vertex_major_t, hgl::types::name_property, hgl::types::empty_properties>, // undirected, vertex-major, vertex properties
    std::tuple<hgl::bf_directed_t, hgl::impl::vertex_major_t, hgl::types::name_property, hgl::types::empty_properties>, // bf-directed, vertex-major, vertex properties
    std::tuple<hgl::undirected_t, hgl::impl::hyperedge_major_t, hgl::types::empty_properties, hgl::types::name_property>, // undirected, hyperedge-major, hyperedge properties
    std::tuple<hgl::bf_directed_t, hgl::impl::hyperedge_major_t, hgl::types::empty_properties, hgl::types::name_property>, // bf-directed, hyperedge-major, hyperedge properties
    std::tuple<hgl::undirected_t, hgl::impl::vertex_major_t, hgl::types::empty_properties, hgl::types::name_property>, // undirected, vertex-major, hyperedge properties
    std::tuple<hgl::bf_directed_t, hgl::impl::vertex_major_t, hgl::types::empty_properties, hgl::types::name_property>, // bf-directed, vertex-major, hyperedge properties
    std::tuple<hgl::undirected_t, hgl::impl::hyperedge_major_t, hgl::types::name_property, hgl::types::name_property>, // undirected, hyperedge-major, all properties
    std::tuple<hgl::bf_directed_t, hgl::impl::hyperedge_major_t, hgl::types::name_property, hgl::types::name_property>, // bf-directed, hyperedge-major, all properties
    std::tuple<hgl::undirected_t, hgl::impl::vertex_major_t, hgl::types::name_property, hgl::types::name_property>, // undirected, vertex-major, all properties
    std::tuple<hgl::bf_directed_t, hgl::impl::vertex_major_t, hgl::types::name_property, hgl::types::name_property> // bf-directed, vertex-major, all properties
);

TEST_CASE_TEMPLATE_DEFINE(
    "Hypergraph Representation Model Conversion Tests (Bidirectional)",
    HypergraphParams,
    bidirectional_hypergraph_params_template
) {
    using DT = std::tuple_element_t<0, HypergraphParams>;
    using VP = std::tuple_element_t<1, HypergraphParams>;
    using EP = std::tuple_element_t<2, HypergraphParams>;

    using list_hypergraph = hgl::hypergraph<hgl::list_hypergraph_traits<hgl::impl::bidirectional_t, DT, VP, EP>>;
    using flat_list_hypergraph = hgl::hypergraph<hgl::flat_list_hypergraph_traits<hgl::impl::bidirectional_t, DT, VP, EP>>;

    using target_list_tag = typename list_hypergraph::implementation_tag;
    using target_flat_list_tag = typename flat_list_hypergraph::implementation_tag;

    // Matrix does not support bidirectional_t natively, but we CAN test converting
    // a bidirectional source into an asymmetric matrix target!
    using target_matrix_tag = hgl::impl::matrix_t<hgl::impl::hyperedge_major_t>;

    test_hypergraph_conversion fixture;

    SUBCASE("source hypergraph model: bidirectional list") {
        auto source_hypergraph = fixture.create_test_hypergraph<list_hypergraph>();

        SUBCASE("identity conversion") {
            const auto converted_hypergraph =
                hgl::to<target_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("flat-list conversion") {
            const auto converted_hypergraph =
                hgl::to<target_flat_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("matrix conversion (asymmetric)") {
            const auto converted_hypergraph =
                hgl::to<target_matrix_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }
    }

    SUBCASE("source hypergraph model: bidirectional flat list") {
        auto source_hypergraph = fixture.create_test_hypergraph<flat_list_hypergraph>();

        SUBCASE("identity conversion") {
            const auto converted_hypergraph =
                hgl::to<target_flat_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("list conversion") {
            const auto converted_hypergraph =
                hgl::to<target_list_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }

        SUBCASE("matrix conversion (asymmetric)") {
            const auto converted_hypergraph =
                hgl::to<target_matrix_tag>(std::move(source_hypergraph));
            fixture.validate_hypergraph(converted_hypergraph);
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bidirectional_hypergraph_params_template,
    std::tuple<hgl::undirected_t, hgl::types::empty_properties, hgl::types::empty_properties>, // undirected, no properties
    std::tuple<hgl::bf_directed_t, hgl::types::empty_properties, hgl::types::empty_properties>, // bf-directed, no properties
    std::tuple<hgl::undirected_t, hgl::types::name_property, hgl::types::empty_properties>, // undirected, vertex properties
    std::tuple<hgl::bf_directed_t, hgl::types::name_property, hgl::types::empty_properties>, // bf-directed, vertex properties
    std::tuple<hgl::undirected_t, hgl::types::empty_properties, hgl::types::name_property>, // undirected, hyperedge properties
    std::tuple<hgl::bf_directed_t, hgl::types::empty_properties, hgl::types::name_property>, // bf-directed, hyperedge properties
    std::tuple<hgl::undirected_t, hgl::types::name_property, hgl::types::name_property>, // undirected, all properties
    std::tuple<hgl::bf_directed_t, hgl::types::name_property, hgl::types::name_property> // bf-directed, all properties
);

TEST_CASE_TEMPLATE_DEFINE(
    "Undirected Hypergraph to Graph Conversion Tests",
    HypergraphTraits,
    undirected_hypergraph_traits_to_graph_conversion_template
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

        const std::vector<std::pair<hgl::types::id_type, hgl::types::id_type>> expected_edges{
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
        const std::vector<std::pair<hgl::types::id_type, hgl::types::id_type>> expected_edges{
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
    undirected_hypergraph_traits_to_graph_conversion_template,
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
    "BF-directed Hypergraph to Graph Converters Tests",
    HypergraphTraits,
    bf_directed_hypergraph_traits_to_graph_conversion_template
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
        const std::vector<std::pair<hgl::types::id_type, hgl::types::id_type>> expected_edges{
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
        const std::vector<std::pair<hgl::types::id_type, hgl::types::id_type>> expected_edges{
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
    bf_directed_hypergraph_traits_to_graph_conversion_template,
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
