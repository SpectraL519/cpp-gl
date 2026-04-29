#include "doctest.h"

#include <gl/directional_tags.hpp>
#include <gl/graph_traits.hpp>

#include <hgl/conversion.hpp>
#include <hgl/hypergraph.hpp>
#include <hgl/hypergraph_traits.hpp>
#include <hgl/impl/layout_tags.hpp>
#include <hgl/types.hpp>

#include <algorithm>
#include <concepts>
#include <vector>

namespace rng = std::ranges;
namespace vw = std::views;

namespace hgl_testing {

TEST_SUITE_BEGIN("test_converters");

struct test_hypergraph_conversion {
    using property_type = hgl::name_property;

    template <hgl::traits::c_undirected_hypergraph HypergraphType>
    [[nodiscard]] HypergraphType create_test_hypergraph() {
        HypergraphType h(4uz, 3uz);
        h.bind({0u, 1u, 2u}, 0u);
        h.bind({1u, 2u, 3u}, 1u);
        h.bind({0u, 3u}, 2u);
        this->set_properties(h);
        return h;
    }

    template <hgl::traits::c_bf_directed_hypergraph HypergraphType>
    [[nodiscard]] HypergraphType create_test_hypergraph() {
        HypergraphType h(4uz, 2uz);
        h.bind_tail({0u, 1u}, 0u);
        h.bind_head({2u, 3u}, 0u);
        h.bind_tail(2u, 1u);
        h.bind_head({0u, 1u}, 1u);
        this->set_properties(h);
        return h;
    }

    void set_properties(hgl::traits::c_hypergraph auto& h) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : h.vertex_ids())
                h.vertex_properties(vid) = property_type("vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::hyperedge_properties_type,
                          property_type>)
            for (const auto& eid : h.hyperedge_ids())
                h.hyperedge_properties(eid) = property_type("hyperedge_" + std::to_string(eid));
    }

    void validate_hypergraph(const hgl::traits::c_undirected_hypergraph auto& h) {
        REQUIRE_EQ(h.n_vertices(), 4uz);
        REQUIRE_EQ(h.n_hyperedges(), 3uz);
        CHECK(h.are_incident(0u, 0u));
        CHECK(h.are_incident(1u, 0u));
        CHECK(h.are_incident(2u, 0u));
        CHECK(h.are_incident(1u, 1u));
        CHECK(h.are_incident(2u, 1u));
        CHECK(h.are_incident(3u, 1u));
        CHECK(h.are_incident(0u, 2u));
        CHECK(h.are_incident(3u, 2u));

        CHECK_FALSE(h.are_incident(3u, 0u));
        CHECK_FALSE(h.are_incident(0u, 1u));
        CHECK_FALSE(h.are_incident(1u, 2u));

        this->validate_properties(h);
    }

    void validate_hypergraph(const hgl::traits::c_bf_directed_hypergraph auto& h) {
        REQUIRE_EQ(h.n_vertices(), 4uz);
        REQUIRE_EQ(h.n_hyperedges(), 2uz);

        CHECK(h.is_tail(0u, 0u));
        CHECK(h.is_tail(1u, 0u));
        CHECK(h.is_head(2u, 0u));
        CHECK(h.is_head(3u, 0u));

        CHECK(h.is_tail(2u, 1u));
        CHECK(h.is_head(0u, 1u));
        CHECK(h.is_head(1u, 1u));

        CHECK_FALSE(h.is_head(0u, 0u));
        CHECK_FALSE(h.is_tail(3u, 0u));
        CHECK_FALSE(h.is_tail(0u, 1u));
        CHECK_FALSE(h.is_head(2u, 1u));

        this->validate_properties(h);
    }

    void validate_properties(const hgl::traits::c_hypergraph auto& h) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : h.vertex_ids())
                CHECK_EQ(h.vertex_properties(vid), "vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(h)>::hyperedge_properties_type,
                          property_type>)
            for (const auto& eid : h.hyperedge_ids())
                CHECK_EQ(h.hyperedge_properties(eid), "hyperedge_" + std::to_string(eid));
    }
};

TEST_CASE_TEMPLATE_DEFINE(
    "Hypergraph Representation Model Conversion Tests", HypergraphParams, hypergraph_params_template
) {
    using DT = std::tuple_element_t<0, HypergraphParams>;
    using VP = std::tuple_element_t<1, HypergraphParams>;
    using EP = std::tuple_element_t<2, HypergraphParams>;

    using vmajor_list_tag = hgl::impl::list_t<hgl::impl::vertex_major_t>;
    using emajor_list_tag = hgl::impl::list_t<hgl::impl::hyperedge_major_t>;
    using bidir_list_tag = hgl::impl::list_t<hgl::impl::bidirectional_t>;

    using vmajor_list_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, vmajor_list_tag>>;
    using emajor_list_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, emajor_list_tag>>;
    using bidir_list_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, bidir_list_tag>>;

    using vmajor_flat_list_tag = hgl::impl::flat_list_t<hgl::impl::vertex_major_t>;
    using emajor_flat_list_tag = hgl::impl::flat_list_t<hgl::impl::hyperedge_major_t>;
    using bidir_flat_list_tag = hgl::impl::flat_list_t<hgl::impl::bidirectional_t>;

    using vmajor_flat_list_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, vmajor_flat_list_tag>>;
    using emajor_flat_list_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, emajor_flat_list_tag>>;
    using bidir_flat_list_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, bidir_flat_list_tag>>;

    using vmajor_matrix_tag = hgl::impl::matrix_t<hgl::impl::vertex_major_t>;
    using emajor_matrix_tag = hgl::impl::matrix_t<hgl::impl::hyperedge_major_t>;

    using vmajor_matrix_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, vmajor_matrix_tag>>;
    using emajor_matrix_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, emajor_matrix_tag>>;

    using vmajor_flat_matrix_tag = hgl::impl::flat_matrix_t<hgl::impl::vertex_major_t>;
    using emajor_flat_matrix_tag = hgl::impl::flat_matrix_t<hgl::impl::hyperedge_major_t>;

    using vmajor_flat_matrix_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, vmajor_flat_matrix_tag>>;
    using emajor_flat_matrix_hypergraph =
        hgl::hypergraph<hgl::hypergraph_traits<DT, VP, EP, emajor_flat_matrix_tag>>;

    test_hypergraph_conversion fixture;

    auto test_conversion_for =
        [&fixture]<typename Source>(std::type_identity<Source>, const char* source_name) {
            SUBCASE(source_name) {
                SUBCASE("to vertex-major list") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<vmajor_list_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to hyperedge-major list") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<emajor_list_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to bidirectional list") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<bidir_list_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to vertex-major flat-list") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<vmajor_flat_list_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to hyperedge-major flat-list") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<emajor_flat_list_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to bidirectional flat-list") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<bidir_flat_list_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to vertex-major matrix") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<vmajor_matrix_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to hyperedge-major matrix") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<emajor_matrix_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to vertex-major flat-matrix") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<vmajor_flat_matrix_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
                SUBCASE("to hyperedge-major flat-matrix") {
                    auto src = fixture.create_test_hypergraph<Source>();
                    auto dst = hgl::to<emajor_flat_matrix_tag>(std::move(src));
                    fixture.validate_hypergraph(dst);
                }
            }
        };

    test_conversion_for(std::type_identity<vmajor_list_hypergraph>{}, "source: vertex-major list");
    test_conversion_for(
        std::type_identity<emajor_list_hypergraph>{}, "source: hyperedge-major list"
    );
    test_conversion_for(std::type_identity<bidir_list_hypergraph>{}, "source: bidirectional list");

    test_conversion_for(
        std::type_identity<vmajor_flat_list_hypergraph>{}, "source: vertex-major flat-list"
    );
    test_conversion_for(
        std::type_identity<emajor_flat_list_hypergraph>{}, "source: hyperedge-major flat-list"
    );
    test_conversion_for(
        std::type_identity<bidir_flat_list_hypergraph>{}, "source: bidirectional flat-list"
    );

    test_conversion_for(
        std::type_identity<vmajor_matrix_hypergraph>{}, "source: vertex-major matrix"
    );
    test_conversion_for(
        std::type_identity<emajor_matrix_hypergraph>{}, "source: hyperedge-major matrix"
    );

    test_conversion_for(
        std::type_identity<vmajor_flat_matrix_hypergraph>{}, "source: vertex-major flat-matrix"
    );
    test_conversion_for(
        std::type_identity<emajor_flat_matrix_hypergraph>{}, "source: hyperedge-major flat-matrix"
    );
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_params_template,
    std::tuple<
        hgl::undirected_t,
        hgl::empty_properties,
        hgl::empty_properties>, // undirected, no properties
    std::tuple<
        hgl::bf_directed_t,
        hgl::empty_properties,
        hgl::empty_properties>, // bf-directed, no properties
    std::tuple<
        hgl::undirected_t,
        hgl::name_property,
        hgl::empty_properties>, // undirected, vertex properties
    std::tuple<
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::empty_properties>, // bf-directed, vertex properties
    std::tuple<
        hgl::undirected_t,
        hgl::empty_properties,
        hgl::name_property>, // undirected, hyperedge properties
    std::tuple<
        hgl::bf_directed_t,
        hgl::empty_properties,
        hgl::name_property>, // bf-directed, hyperedge properties
    std::tuple<
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected, all properties
    std::tuple<
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property> // bf-directed, all properties
);

TEST_CASE_TEMPLATE_DEFINE(
    "Undirected Hypergraph to Graph Conversion Tests",
    HypergraphTraits,
    undirected_hypergraph_traits_to_graph_conversion_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;

    using list_graph = gl::graph<gl::list_graph_traits<gl::undirected_t>>;
    using flat_list_graph = gl::graph<gl::flat_list_graph_traits<gl::undirected_t>>;
    using matrix_graph = gl::graph<gl::matrix_graph_traits<gl::undirected_t>>;
    using flat_matrix_graph = gl::graph<gl::flat_matrix_graph_traits<gl::undirected_t>>;

    SUBCASE("projection should produce a clique for each hyperedge") {
        sut_type sut{4uz, 4uz};
        sut.bind({0u, 1u, 2u}, 0u); // e0 = {0,1,2}
        sut.bind({1u, 2u, 3u}, 1u); // e1 = {1,2,3}
        sut.bind({0u, 3u}, 2u); // e2 = {0,3}
        sut.bind(0u, 3u); // e3 = {0} (should not add any edge)

        const std::vector<hgl::homogeneous_pair<hgl::default_id_type>> expected_edges{
            // e0: (0,1), (0,2), (1,2)
            {0uz, 1uz},
            {0uz, 2uz},
            {1uz, 2uz},
            {1uz, 3uz},
            // e1: (1,2) - exists, (1,3), (2,3)
            {2uz, 3uz},
            {0uz, 3uz},
            // e3: self-loop (0,0)
            {0uz, 0uz}
        };

        auto test_conversion_for =
            [&]<typename TargetGraph>(std::type_identity<TargetGraph>, const char* target_name) {
                SUBCASE(target_name) {
                    const auto clique = hgl::projection<TargetGraph>(sut);
                    CHECK_EQ(clique.n_vertices(), sut.n_vertices());
                    CHECK_EQ(clique.n_edges(), expected_edges.size());
                    for (const auto& [u, v] : expected_edges)
                        CHECK(clique.has_edge(u, v));
                }
            };

        test_conversion_for(std::type_identity<list_graph>{}, "target model: list");
        test_conversion_for(std::type_identity<flat_list_graph>{}, "target model: flat-list");
        test_conversion_for(std::type_identity<matrix_graph>{}, "target model: matrix");
        test_conversion_for(std::type_identity<flat_matrix_graph>{}, "target model: flat-matrix");
    }

    SUBCASE("incidence_graph should produce a bipartite graph connecting vertices to hyperedges") {
        sut_type sut{4uz, 4uz};
        sut.bind({0u, 1u, 2u}, 0u); // e0 = {0,1,2}
        sut.bind({1u, 2u, 3u}, 1u); // e1 = {1,2,3}
        sut.bind({0u, 3u}, 2u); // e2 = {0,3}
        sut.bind(0u, 3u); // e3 = {0}

        // Expected edges: vertices 0-3, hyperedges 4-7
        const std::vector<hgl::homogeneous_pair<hgl::default_id_type>> expected_edges{
            // e0 (4): {0,1,2}
            {0u, 4u},
            {1u, 4u},
            {2u, 4u},
            // e1 (5): {1,2,3}
            {1u, 5u},
            {2u, 5u},
            {3u, 5u},
            // e2 (6): {0,3}
            {0u, 6u},
            {3u, 6u},
            // e3 (7): {0}
            {0u, 7u}
        };

        auto test_conversion_for =
            [&]<typename TargetGraph>(std::type_identity<TargetGraph>, const char* target_name) {
                SUBCASE(target_name) {
                    const auto incidence = hgl::incidence_graph<TargetGraph>(sut);
                    CHECK_EQ(incidence.n_vertices(), sut.n_vertices() + sut.n_hyperedges());
                    CHECK_EQ(incidence.n_edges(), expected_edges.size());
                    for (const auto& [u, v] : expected_edges)
                        CHECK(incidence.has_edge(u, v));
                }
            };

        test_conversion_for(std::type_identity<list_graph>{}, "target model: list");
        test_conversion_for(std::type_identity<flat_list_graph>{}, "target model: flat list");
        test_conversion_for(std::type_identity<matrix_graph>{}, "target model: matrix");
        test_conversion_for(std::type_identity<flat_matrix_graph>{}, "target model: flat-matrix");
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
    "BF-directed Hypergraph to Graph Converters Tests",
    HypergraphTraits,
    bf_directed_hypergraph_traits_to_graph_conversion_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;

    // Define the three target graph models
    using list_graph = gl::graph<
        gl::directed_graph_traits<gl::empty_properties, gl::empty_properties, gl::impl::list_t>>;
    using flat_list_graph = gl::graph<
        gl::directed_graph_traits<gl::empty_properties, gl::empty_properties, gl::impl::flat_list_t>>;
    using matrix_graph = gl::graph<
        gl::directed_graph_traits<gl::empty_properties, gl::empty_properties, gl::impl::matrix_t>>;
    using flat_matrix_graph = gl::graph<gl::directed_graph_traits<
        gl::empty_properties,
        gl::empty_properties,
        gl::impl::flat_matrix_t>>;

    SUBCASE("projection should produce directed edges from tails to heads for each hyperedge") {
        sut_type sut{4uz, 2uz};

        // e0: T={0,1} -> H={2,3}
        sut.bind_tail({0u, 1u}, 0u);
        sut.bind_head({2u, 3u}, 0u);

        // e1: T={2} -> H={0,1}
        sut.bind_tail(2u, 1u);
        sut.bind_head({0u, 1u}, 1u);

        const std::vector<hgl::homogeneous_pair<hgl::default_id_type>> expected_edges{
            // e0: 0->2, 0->3, 1->2, 1->3
            {0u, 2u},
            {0u, 3u},
            {1u, 2u},
            {1u, 3u},
            // e1: 2->0, 2->1
            {2u, 0u},
            {2u, 1u}
        };

        auto test_conversion_for =
            [&]<typename TargetGraph>(std::type_identity<TargetGraph>, const char* target_name) {
                SUBCASE(target_name) {
                    const auto proj = hgl::projection<TargetGraph>(sut);
                    CHECK_EQ(proj.n_vertices(), sut.n_vertices());
                    CHECK_EQ(proj.n_edges(), expected_edges.size());
                    for (const auto& [u, v] : expected_edges)
                        CHECK(proj.has_edge(u, v));
                }
            };

        test_conversion_for(std::type_identity<list_graph>{}, "target model: list");
        test_conversion_for(std::type_identity<flat_list_graph>{}, "target model: flat list");
        test_conversion_for(std::type_identity<matrix_graph>{}, "target model: matrix");
        test_conversion_for(std::type_identity<flat_matrix_graph>{}, "target model: flat-matrix");
    }

    SUBCASE("incidence_graph should produce a directed bipartite graph connecting tails to "
            "hyperedges and hyperedges to heads") {
        sut_type sut{4uz, 2uz};

        // e0: T={0,1} -> H={2,3}
        sut.bind_tail({0u, 1u}, 0u);
        sut.bind_head({2u, 3u}, 0u);

        // e1: T={2} -> H={0,1}
        sut.bind_tail(2u, 1u);
        sut.bind_head({0u, 1u}, 1u);

        // Expected directed edges: vertices 0-3, hyperedges 4-5
        const std::vector<hgl::homogeneous_pair<hgl::default_id_type>> expected_edges{
            // Tails to hyperedges: 0->4, 1->4, 2->5
            {0u, 4u},
            {1u, 4u},
            {2u, 5u},
            // Hyperedges to heads: 4->2, 4->3, 5->0, 5->1
            {4u, 2u},
            {4u, 3u},
            {5u, 0u},
            {5u, 1u}
        };

        // Generic runner for the target models
        auto test_conversion_for =
            [&]<typename TargetGraph>(std::type_identity<TargetGraph>, const char* target_name) {
                SUBCASE(target_name) {
                    const auto incidence = hgl::incidence_graph<TargetGraph>(sut);
                    CHECK_EQ(incidence.n_vertices(), sut.n_vertices() + sut.n_hyperedges());
                    CHECK_EQ(incidence.n_edges(), expected_edges.size());
                    for (const auto& [u, v] : expected_edges)
                        CHECK(incidence.has_edge(u, v));
                }
            };

        test_conversion_for(std::type_identity<list_graph>{}, "target model: list");
        test_conversion_for(std::type_identity<flat_list_graph>{}, "target model: flat list");
        test_conversion_for(std::type_identity<matrix_graph>{}, "target model: matrix");
        test_conversion_for(std::type_identity<flat_matrix_graph>{}, "target model: flat-matrix");
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
        hgl::bf_directed_t>, // vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t> // vertex-major flat incidence matrix
);

TEST_SUITE_END(); // test_converters

} // namespace hgl_testing
