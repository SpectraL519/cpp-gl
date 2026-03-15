#include "hgl/converters.hpp"
#include "hgl/hypergraph.hpp"

#include <doctest.h>

#include <algorithm>
#include <concepts>
#include <vector>

namespace rng = std::ranges;
namespace vw = std::views;

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph");

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
    "Undirected hypergraph converters tests", HypergraphTraits, undirected_hypergraph_traits_converters_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using graph_type = gl::graph<gl::undirected_graph_traits<>>;

    SUBCASE("make_clique_graph should produce a clique for each hyperedge") {
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

        const auto clique = hgl::make_clique_graph<graph_type>(sut);

        CHECK_EQ(clique.order(), sut.order());

        const std::vector<std::pair<gl::types::id_type, gl::types::id_type>> expected_edges{
            // e0: (0,1), (0,2), (1,2)
            {0ull, 1ull}, {0ull, 2ull}, {1ull, 2ull}, {1ull, 3ull},
            // e1: (1,2) - exists, (1,3), (2,3)
            {2ull, 3ull}, {0ull, 3ull},
            // e3: none
        };

        CHECK_EQ(clique.size(), expected_edges.size());
        for (const auto& [u, v] : expected_edges)
            CHECK(clique.has_edge(u, v));
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    undirected_hypergraph_traits_converters_template,
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // vertex-major incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t> // vertex-major incidence matrix
);

} // namespace hgl_testing
