#include "testing/hgl/constants.hpp"
#include "testing/hgl/types.hpp"

#include <doctest.h>
#include <hgl/hypergraph.hpp>

#include <algorithm>

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
    gl::type_traits::c_instantiation_of<gl::graph_traits> HypergraphTraits,
    gl::type_traits::c_properties HyperedgeProperties>
using add_edge_property = hgl::hypergraph_traits<
    typename HypergraphTraits::directional_tag,
    typename HypergraphTraits::vertex_properties_type,
    HyperedgeProperties,
    typename HypergraphTraits::implementation_tag>;

inline constexpr auto get_id = [](auto&& element) -> gl::types::id_type { return element.id(); };

TEST_CASE_TEMPLATE_DEFINE(
    "hypergraph structure tests", HypergraphTraits, hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using vertex_type = typename sut_type::vertex_type;

    // --- general tests ---

    SUBCASE("a hypergraph should be initialized with no vertices and no hyperedges by default") {
        sut_type sut{};
        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_EQ(sut.n_hyperedges(), 0uz);
    }

    SUBCASE("a hypergraph constructed with n_vertices parameter should contain n_vertices vertices "
            "and no hyperedges") {
        sut_type sut{constants::n_vertices};

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices);
        REQUIRE_EQ(sut.n_hyperedges(), 0uz);

        REQUIRE(rng::equal(sut.vertices() | vw::transform(get_id), constants::vertex_ids_view));
        REQUIRE(rng::equal(sut.vertex_ids(), constants::vertex_ids_view));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_rng_vid)), std::out_of_range
        );

        // TODO: check no hyperedges
    }

    SUBCASE("a hypergraph constructed with n_vertices and n_hyperedges parameters should contain "
            "n_vertices vertices and n_hyperedges hyperedges") {
        sut_type sut{constants::n_vertices};

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices);
        REQUIRE_EQ(sut.n_hyperedges(), 0uz);

        REQUIRE(rng::equal(sut.vertices() | vw::transform(get_id), constants::vertex_ids_view));
        REQUIRE(rng::equal(sut.vertex_ids(), constants::vertex_ids_view));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_rng_vid)), std::out_of_range
        );

        // TODO: check no hyperedges
    }

    // --- vertex method tests ---

    SUBCASE("add_vertex should return a vertex_descriptor with an incremented id and no hyperedges"
    ) {
        sut_type sut;

        for (gl::types::id_type v_id = 0uz; v_id < constants::n_vertices; v_id++) {
            const auto vertex = sut.add_vertex();
            CHECK_EQ(vertex.id(), v_id);
            CHECK_EQ(sut.n_vertices(), v_id + 1uz);
            // TODO: check no hyperedges
        }

        CHECK_EQ(sut.n_vertices(), constants::n_vertices);
    }

    SUBCASE("add_vertex_with should initialize a new vertex with the input properties structure") {
        using properties_traits_type =
            add_vertex_property<HypergraphTraits, types::boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const auto vertex = sut.add_vertex_with(constants::p_true);
        REQUIRE_EQ(sut.n_vertices(), 1uz);

        CHECK_EQ(vertex.id(), constants::id1);
        CHECK_EQ(vertex.properties(), constants::p_true);
        // TODO: check no hyperedges
    }

    SUBCASE("add_vertices(n) should add n new vertices to the hypergraph") {
        sut_type sut{};
        sut.add_vertices(constants::n_vertices);

        CHECK_EQ(sut.n_vertices(), constants::n_vertices);
        CHECK_EQ(sut.n_hyperedges(), 0uz);
    }

    SUBCASE("add_vertices_with should add new vertices to the hypergraph with the given properties"
    ) {
        using properties_traits_type =
            add_vertex_property<HypergraphTraits, types::boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const std::vector<types::boolean_property> properties_list{
            constants::p_true, constants::p_false, constants::p_true
        };
        const auto expected_n_vertices = properties_list.size();

        sut.add_vertices_with(properties_list);

        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
        CHECK_EQ(sut.n_hyperedges(), 0uz);

        CHECK(rng::equal(sut.vertices(), properties_list, rng::equal_to{}, [](const auto vertex) {
            return vertex.properties();
        }));
    }

    SUBCASE("has_vertex(id) should return true when a vertex with the given id is present in "
            "the graph") {
        sut_type sut{constants::n_vertices};

        CHECK(rng::all_of(constants::vertex_ids_view, [&sut](const auto vertex_id) {
            return sut.has_vertex(vertex_id);
        }));
        CHECK_FALSE(sut.has_vertex(constants::out_of_rng_vid));
    }

    SUBCASE("get_vertex should throw if the given id is invalid") {
        sut_type sut{constants::n_vertices};
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_rng_vid)), std::out_of_range
        );
    }

    SUBCASE("get_vertex should return a vertex with the given id") {
        sut_type sut;
        const auto added_vertex = sut.add_vertex();
        CHECK_EQ(sut.get_vertex(added_vertex.id()), added_vertex);
    }

    SUBCASE("vertices should return the correct vertex collection view") {
        sut_type sut{constants::n_vertices};

        CHECK(rng::equal(sut.vertices(), constants::vertex_ids_view, rng::equal_to{}, get_id));
    }

    SUBCASE("vertex_ids should return the correct vertex list iterator range") {
        sut_type sut{constants::n_vertices};
        CHECK(rng::equal(sut.vertex_ids(), constants::vertex_ids_view));
    }

    SUBCASE("remove_vertex(vertex) should throw if the given vertex is invalid") {
        sut_type sut{constants::n_vertices};
        CHECK_THROWS_AS(
            sut.remove_vertex(vertex_type{constants::out_of_rng_vid}), std::out_of_range
        );
    }

    SUBCASE("remove_vertex(vertex) should remove the given vertex and align ids of remaining "
            "vertices") {
        // TODO: verify hyperedges alignment

        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        const auto vertex_id_view = sut.vertex_ids();
        REQUIRE(std::ranges::equal(
            vertex_id_view, std::views::iota(constants::id1, constants::n_vertices - 1uz)
        ));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::n_vertices - 1uz)), std::out_of_range
        );
    }

    SUBCASE("remove_vertex(id) should throw if the given id is invalid") {
        sut_type sut{constants::n_vertices};
        CHECK_THROWS_AS(sut.remove_vertex(constants::out_of_rng_vid), std::out_of_range);
    }

    SUBCASE("remove_vertex(id) should remove the given vertex and align ids of remaining vertices"
    ) {
        // TODO: verify hyperedges alignment

        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        const auto vertex_id_view = sut.vertex_ids();
        REQUIRE(std::ranges::equal(
            vertex_id_view, std::views::iota(constants::id1, constants::n_vertices - 1uz)
        ));
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::n_vertices - 1uz)), std::out_of_range
        );
    }

    SUBCASE("remove_vetices_from(ids) should properly remove elements at given indices (ignoring "
            "duplicate indices)") {
        // TODO: verify hyperedges alignment

        constexpr auto n_vertices = constants::n_vertices + 1uz;

        sut_type sut{n_vertices};
        sut.remove_vertices_from(
            std::vector<hgl::types::id_type>{constants::id1, constants::id3, constants::id1}
        );

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
    }

    SUBCASE("remove_vetices_from(vertices) should properly remove elements at given indices "
            "(ignoring duplicate vertex references)") {
        // TODO: verify hyperedges alignment

        constexpr auto n_vertices = constants::n_vertices + 1uz;

        sut_type sut{n_vertices};
        const auto v1 = sut.get_vertex(constants::id1);
        const auto v3 = sut.get_vertex(constants::id3);
        sut.remove_vertices_from(std::vector<vertex_type>{v1, v3, v1});

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_traits_template,
    hgl::hyperedge_list_hg_traits<hgl::undirected_t>, // undirected edge list
    hgl::hyperedge_list_hg_traits<hgl::bf_directed_t>, // bf-directed edge list
    hgl::adjacency_list_hg_traits<hgl::undirected_t>, // undirected adjacency list
    hgl::adjacency_list_hg_traits<hgl::bf_directed_t>, // bf-directed adjacency list
    hgl::incidence_matrix_hg_traits<hgl::undirected_t>, // undirected incidence matrix
    hgl::incidence_matrix_hg_traits<hgl::bf_directed_t> // bf-directed incidence matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "properties getter tests", HypergraphTraits, property_hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;

    sut_type sut{constants::n_vertices};
    for (auto vertex : sut.vertices()) {
        vertex.properties() = std::format("vertex_{}", vertex.id());
        // sut.add_hyperedge().properties() = std::format("hyperedge_{}", vertex.id());
    }

    auto vmap = sut.vertex_properties_map();
    CHECK(vmap.size() == constants::n_vertices);
    for (auto [id, property] : std::views::zip(sut.vertex_ids(), vmap)) {
        CHECK_EQ(property, std::format("vertex_{}", id));
        CHECK_EQ(vmap[id], std::format("vertex_{}", id));
        CHECK_EQ(sut.get_vertex_properties(id), std::format("vertex_{}", id));
    }

    CHECK_THROWS_AS(
        static_cast<void>(sut.get_vertex_properties(constants::out_of_rng_vid)), std::out_of_range
    );

    // auto emap = sut.hyperedge_properties_map();
    // CHECK(emap.size() == constants::n_vertices);
    // for (auto [id, property] : std::views::enumerate(emap)) {
    //     CHECK_EQ(property, std::format("hyperedge_{}", id));
    //     CHECK_EQ(emap[id], std::format("hyperedge_{}", id));
    //     CHECK_EQ(sut.get_hyperedge_properties(id), std::format("hyperedge_{}", id));
    // }

    // CHECK_THROWS_AS(
    //     static_cast<void>(sut.get_hyperedge_properties(constants::out_of_rng_vid)),
    //     std::out_of_range
    // );
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    property_hypergraph_traits_template,
    hgl::hyperedge_list_hg_traits<
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected edge list
    hgl::hyperedge_list_hg_traits<
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed edge list
    hgl::adjacency_list_hg_traits<
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected adjacency list
    hgl::adjacency_list_hg_traits<
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed adjacency list
    hgl::incidence_matrix_hg_traits<
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected incidence matrix
    hgl::incidence_matrix_hg_traits<
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property> // bf-directed incidence matrix
);

TEST_SUITE_END(); // test_hypergraph

} // namespace hgl_testing
