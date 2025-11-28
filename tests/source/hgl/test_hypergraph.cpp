#include "hgl/hypergraph.hpp"

#include <doctest.h>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph");

static_assert(std::same_as<hgl::vertex_descriptor<>, gl::vertex_descriptor<>>);

template <typename HypergraphTraits>
struct test_hypergraph {
    using traits_type = HypergraphTraits;
    using sut_type = hgl::hypergraph<traits_type>;

    static constexpr std::size_t n_vertices = 5uz;
    static constexpr auto vertex_ids_view = std::views::iota(hgl::constants::initial_id, n_vertices);
};

TEST_CASE_TEMPLATE_DEFINE(
    "hypergraph structure tests", HypergraphTraits, hypergraph_traits_template
) {
    using fixture_type = test_hypergraph<HypergraphTraits>;
    using sut_type = typename fixture_type::sut_type;
    using vertex_type = typename sut_type::vertex_type;

    constexpr std::size_t n_vertices = fixture_type::n_vertices;
    constexpr auto vertex_ids_view = fixture_type::vertex_ids_view;

    SUBCASE("a hypergraph should be initialized with no vertices and no edges by default") {
        sut_type sut{};
        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_EQ(sut.n_hyperedges(), 0uz);
    }

    SUBCASE("a hypergraph constructed with n_vertices parameter should contain n_vertices vertices and no edges") {
        sut_type sut{constants::n_elements};

        REQUIRE(std::ranges::equal(
            sut.vertices() | std::views::transform(get_id), vertex_id_view
        ));

        REQUIRE(std::ranges::equal(sut.vertex_ids(), vertex_id_view));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_range_element_idx)),
            std::out_of_range
        );

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
