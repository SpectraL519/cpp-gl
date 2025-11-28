#include "hgl/hypergraph.hpp"

#include <doctest.h>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph");

static_assert(std::same_as<hgl::vertex_descriptor<>, gl::vertex_descriptor<>>);

inline constexpr auto get_id = [](auto&& element) -> gl::types::id_type { return element.id(); };

TEST_CASE_TEMPLATE_DEFINE(
    "hypergraph structure tests", HypergraphTraits, hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using vertex_type = typename sut_type::vertex_type;

    constexpr hgl::types::size_type n_vertices = 5uz;
    constexpr hgl::types::size_type out_of_rng_vid = n_vertices;
    constexpr auto vertex_ids_view = std::views::iota(hgl::constants::initial_id, n_vertices);

    SUBCASE("a hypergraph should be initialized with no vertices and no edges by default") {
        sut_type sut{};
        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_EQ(sut.n_hyperedges(), 0uz);
    }

    SUBCASE("a hypergraph constructed with n_vertices parameter should contain n_vertices vertices "
            "and no edges") {
        sut_type sut{n_vertices};

        REQUIRE(std::ranges::equal(sut.vertices() | std::views::transform(get_id), vertex_ids_view)
        );

        REQUIRE(std::ranges::equal(sut.vertex_ids(), vertex_ids_view));

        CHECK_THROWS_AS(static_cast<void>(sut.get_vertex(out_of_rng_vid)), std::out_of_range);

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
