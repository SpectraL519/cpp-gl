#include "testing/hgl/constants.hpp"
#include "testing/hgl/types.hpp"

#include <doctest.h>
#include <hgl/hypergraph.hpp>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph");

template <
    gl::type_traits::c_instantiation_of<hgl::hypergraph_traits> HypergraphTraits,
    gl::type_traits::c_properties VertexProperties>
using add_vertex_property = hgl::hypergraph_traits<
    typename HypergraphTraits::hyperedge_directional_tag,
    VertexProperties,
    typename HypergraphTraits::hyperedge_properties_type,
    typename HypergraphTraits::implementation_tag>;

template <
    gl::type_traits::c_instantiation_of<gl::graph_traits> HypergraphTraits,
    gl::type_traits::c_properties HyperedgeProperties>
using add_edge_property = hgl::hypergraph_traits<
    typename HypergraphTraits::edge_directional_tag,
    typename HypergraphTraits::vertex_properties_type,
    HyperedgeProperties,
    typename HypergraphTraits::implementation_tag>;

inline constexpr auto get_id = [](auto&& element) -> gl::types::id_type { return element.id(); };

TEST_CASE_TEMPLATE_DEFINE(
    "hypergraph structure tests", HypergraphTraits, hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using vertex_type = typename sut_type::vertex_type;

    SUBCASE("a hypergraph should be initialized with no vertices and no edges by default") {
        sut_type sut{};
        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_EQ(sut.n_hyperedges(), 0uz);
    }

    SUBCASE("a hypergraph constructed with n_vertices parameter should contain n_vertices vertices "
            "and no edges") {
        sut_type sut{constants::n_vertices};

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices);
        REQUIRE_EQ(sut.n_hyperedges(), 0uz);

        REQUIRE(std::ranges::equal(
            sut.vertices() | std::views::transform(get_id), constants::vertex_ids_view
        ));

        REQUIRE(std::ranges::equal(sut.vertex_ids(), constants::vertex_ids_view));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_rng_vid)), std::out_of_range
        );

        // TODO: check no edges
    }

    SUBCASE("add_vertex should return a vertex_descriptor with an incremented id and no edges") {
        sut_type sut;

        for (gl::types::id_type v_id = 0uz; v_id < constants::n_vertices; v_id++) {
            const auto vertex = sut.add_vertex();
            CHECK_EQ(vertex.id(), v_id);
            CHECK_EQ(sut.n_vertices(), v_id + 1uz);
            // TODO: check no edges
        }

        CHECK_EQ(sut.n_vertices(), constants::n_vertices);
    }

    SUBCASE("add_vertex_with should initialize a new vertex with the input properties structure") {
        using properties_traits_type =
            add_vertex_property<HypergraphTraits, types::boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const auto vertex = sut.add_vertex_with(constants::p_true);
        REQUIRE_EQ(sut.n_vertices(), 1uz);

        CHECK_EQ(vertex.id(), hgl::constants::initial_id);
        CHECK_EQ(vertex.properties(), constants::p_true);
        // TODO: check no edges
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_traits_template,
    hgl::edge_list_hg_traits<hgl::undirected_t>, // undirected edge list
    hgl::edge_list_hg_traits<hgl::bf_directed_t>, // bf-directed edge list
    hgl::adjacency_list_hg_traits<hgl::undirected_t>, // undirected adjacency list
    hgl::adjacency_list_hg_traits<hgl::bf_directed_t>, // bf-directed adjacency list
    hgl::incidence_matrix_hg_traits<hgl::undirected_t>, // undirected incidence matrix
    hgl::incidence_matrix_hg_traits<hgl::bf_directed_t> // bf-directed incidence matrix
);

TEST_SUITE_END(); // test_hypergraph

} // namespace hgl_testing
