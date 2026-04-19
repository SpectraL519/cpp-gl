#include "doctest.h"
#include "testing/gl/types.hpp"

#include <gl/conversion.hpp>
#include <gl/directional_tags.hpp>
#include <gl/graph.hpp>
#include <gl/impl/impl_tags.hpp>

#include <algorithm>
#include <concepts>
#include <tuple>
#include <type_traits>
#include <vector>

namespace rng = std::ranges;
namespace vw = std::views;

namespace gl_testing {

TEST_SUITE_BEGIN("test_conversion");

struct test_conversion {
    using property_type = gl::name_property;

    template <gl::traits::c_graph GraphType>
    [[nodiscard]] GraphType create_test_graph() {
        GraphType graph{this->test_order};
        for (const auto& [source, target] : this->test_edges)
            graph.add_edge(source, target);

        this->set_properties(graph);

        return graph;
    }

    void set_properties(gl::traits::c_graph auto& graph) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : graph.vertex_ids())
                graph.vertex_properties(vid) = property_type("vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::edge_properties_type,
                          property_type>)
            for (const auto& eid : graph.edge_ids())
                graph.edge_properties(eid) = property_type("edge_" + std::to_string(eid));
    }

    void validate_graph(const gl::traits::c_graph auto& graph) {
        REQUIRE_EQ(graph.n_vertices(), this->test_order);
        REQUIRE_EQ(graph.n_edges(), this->test_edges.size());
        for (const auto& [source, target] : this->test_edges)
            CHECK(graph.has_edge(source, target));

        this->validate_properties(graph);
    }

    void validate_properties(const gl::traits::c_graph auto& graph) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : graph.vertex_ids())
                CHECK_EQ(graph.vertex_properties(vid), "vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::edge_properties_type,
                          property_type>)
            for (const auto& eid : graph.edge_ids())
                CHECK_EQ(graph.edge_properties(eid), "edge_" + std::to_string(eid));
    }

    gl::size_type test_order{5};
    std::vector<gl::homogeneous_pair<gl::default_id_type>> test_edges{
        {0, 1},
        {0, 2},
        {1, 3},
        {2, 3},
        {3, 4}
    };
};

TEST_CASE_TEMPLATE_DEFINE(
    "Graph representation model conversion tests", GraphParams, graph_params_template
) {
    using DT = std::tuple_element_t<0, GraphParams>;
    using VP = std::tuple_element_t<1, GraphParams>;
    using EP = std::tuple_element_t<2, GraphParams>;

    using list_graph = gl::graph<gl::list_graph_traits<DT, VP, EP>>;
    using flat_list_graph = gl::graph<gl::flat_list_graph_traits<DT, VP, EP>>;
    using matrix_graph = gl::graph<gl::matrix_graph_traits<DT, VP, EP>>;
    using flat_matrix_graph = gl::graph<gl::flat_matrix_graph_traits<DT, VP, EP>>;

    test_conversion fixture;

    auto test_conversion_for =
        [&fixture]<typename Source>(std::type_identity<Source>, const char* source_name) {
            SUBCASE(source_name) {
                SUBCASE("to list") {
                    auto src = fixture.create_test_graph<Source>();
                    auto dst = gl::to<gl::impl::list_t>(std::move(src));
                    fixture.validate_graph(dst);
                }
                SUBCASE("to flat-list") {
                    auto src = fixture.create_test_graph<Source>();
                    auto dst = gl::to<gl::impl::flat_list_t>(std::move(src));
                    fixture.validate_graph(dst);
                }
                SUBCASE("to matrix") {
                    auto src = fixture.create_test_graph<Source>();
                    auto dst = gl::to<gl::impl::matrix_t>(std::move(src));
                    fixture.validate_graph(dst);
                }
                SUBCASE("to flat-matrix") {
                    auto src = fixture.create_test_graph<Source>();
                    auto dst = gl::to<gl::impl::flat_matrix_t>(std::move(src));
                    fixture.validate_graph(dst);
                }
            }
        };

    test_conversion_for(std::type_identity<list_graph>{}, "source: list");
    test_conversion_for(std::type_identity<flat_list_graph>{}, "source: flat-list");
    test_conversion_for(std::type_identity<matrix_graph>{}, "source: matrix");
    test_conversion_for(std::type_identity<flat_matrix_graph>{}, "source: flat-matrix");
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_params_template,
    std::tuple<
        gl::directed_t,
        gl::empty_properties,
        gl::empty_properties>, // directed graph, no properties
    std::tuple<
        gl::undirected_t,
        gl::empty_properties,
        gl::empty_properties>, // undirected graph, no properties
    std::tuple<
        gl::directed_t,
        gl::name_property,
        gl::empty_properties>, // directed graph, vertex properties
    std::tuple<
        gl::undirected_t,
        gl::name_property,
        gl::empty_properties>, // undirected graph, vertex properties
    std::tuple<
        gl::directed_t,
        gl::empty_properties,
        gl::name_property>, // directed graph, edge properties
    std::tuple<
        gl::undirected_t,
        gl::empty_properties,
        gl::name_property>, // undirected graph, edge properties
    std::tuple<
        gl::directed_t,
        gl::name_property,
        gl::name_property>, // directed graph, all properties
    std::tuple<
        gl::undirected_t,
        gl::name_property,
        gl::name_property> // undirected graph, all properties
);

TEST_SUITE_END(); // test_conversion

} // namespace gl_testing
