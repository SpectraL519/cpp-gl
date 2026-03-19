#include "gl/conversion.hpp"
#include "gl/edge_tags.hpp"
#include "gl/graph.hpp"
#include "testing/gl/types.hpp"

#include <doctest.h>

#include <algorithm>
#include <concepts>
#include <tuple>
#include <vector>

namespace rng = std::ranges;
namespace vw = std::views;

namespace hgl_testing {

TEST_SUITE_BEGIN("test_conversion");

constexpr auto get_id = [](auto&& element) -> gl::types::id_type { return element.id(); };

struct test_conversion {
    using property_type = gl::types::name_property;

    template <gl::type_traits::c_graph GraphType>
    [[nodiscard]] GraphType create_test_graph() {
        GraphType graph{this->test_order};
        for (const auto& [source, target] : this->test_edges)
            graph.add_edge(source, target);

        this->set_properties(graph);

        return graph;
    }

    void set_properties(gl::type_traits::c_graph auto& graph) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : graph.vertex_ids())
                graph.get_vertex_properties(vid) = property_type("vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::edge_properties_type,
                          property_type>)
            for (const auto& eid : graph.edge_ids())
                graph.get_edge_properties(eid) = property_type("edge_" + std::to_string(eid));
    }

    void validate_graph(const gl::type_traits::c_graph auto& graph) {
        REQUIRE_EQ(graph.order(), this->test_order);
        REQUIRE_EQ(graph.size(), this->test_edges.size());
        for (const auto& [source, target] : this->test_edges)
            CHECK(graph.has_edge(source, target));

        this->validate_properties(graph);
    }

    void validate_properties(const gl::type_traits::c_graph auto& graph) {
        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::vertex_properties_type,
                          property_type>)
            for (const auto& vid : graph.vertex_ids())
                CHECK_EQ(graph.get_vertex_properties(vid), "vertex_" + std::to_string(vid));

        if constexpr (std::same_as<
                          typename std::decay_t<decltype(graph)>::edge_properties_type,
                          property_type>)
            for (const auto& eid : graph.edge_ids())
                CHECK_EQ(graph.get_edge_properties(eid), "edge_" + std::to_string(eid));
    }

    gl::types::size_type test_order{5};
    std::vector<gl::types::homogeneous_pair<gl::types::id_type>> test_edges{
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

    test_conversion fixture;

    SUBCASE("source graph model: list") {
        auto source_graph = fixture.create_test_graph<list_graph>();

        SUBCASE("identity conversion") {
            const auto converted_graph = gl::to<gl::impl::list_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }

        SUBCASE("flat-list conversion") {
            const auto converted_graph = gl::to<gl::impl::flat_list_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }

        SUBCASE("matrix conversion") {
            const auto converted_graph = gl::to<gl::impl::matrix_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }
    }

    SUBCASE("source graph model: flat list") {
        auto source_graph = fixture.create_test_graph<flat_list_graph>();

        SUBCASE("identity conversion") {
            const auto converted_graph = gl::to<gl::impl::flat_list_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }

        SUBCASE("list conversion") {
            const auto converted_graph = gl::to<gl::impl::list_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }

        SUBCASE("matrix conversion") {
            const auto converted_graph = gl::to<gl::impl::matrix_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }
    }

    SUBCASE("source graph model: matrix") {
        auto source_graph = fixture.create_test_graph<matrix_graph>();

        SUBCASE("identity conversion") {
            const auto converted_graph = gl::to<gl::impl::matrix_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }

        SUBCASE("list conversion") {
            const auto converted_graph = gl::to<gl::impl::list_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }

        SUBCASE("flat-list conversion") {
            const auto converted_graph = gl::to<gl::impl::flat_list_t>(std::move(source_graph));
            fixture.validate_graph(converted_graph);
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(graph_params_template, std::tuple<gl::directed_t, gl::types::empty_properties, gl::types::empty_properties>, std::tuple<gl::undirected_t, gl::types::empty_properties, gl::types::empty_properties>, std::tuple<gl::directed_t, gl::types::name_property, gl::types::empty_properties>, std::tuple<gl::undirected_t, gl::types::name_property, gl::types::empty_properties>, std::tuple<gl::directed_t, gl::types::empty_properties, gl::types::name_property>, std::tuple<gl::undirected_t, gl::types::empty_properties, gl::types::name_property>, std::tuple<gl::directed_t, gl::types::name_property, gl::types::name_property>, std::tuple<gl::undirected_t, gl::types::name_property, gl::types::name_property>);

TEST_SUITE_END(); // test_conversion

} // namespace hgl_testing
