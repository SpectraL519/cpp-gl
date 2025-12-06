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
    gl::type_traits::c_instantiation_of<hgl::hypergraph_traits> HypergraphTraits,
    gl::type_traits::c_properties HyperedgeProperties>
using add_hyperedge_property = hgl::hypergraph_traits<
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
    using hyperedge_type = typename sut_type::hyperedge_type;

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
    }

    SUBCASE("a hypergraph constructed with n_vertices and n_hyperedges parameters should contain "
            "n_vertices vertices and n_hyperedges hyperedges") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices);
        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges);

        REQUIRE(rng::equal(sut.vertices() | vw::transform(get_id), constants::vertex_ids_view));
        REQUIRE(rng::equal(sut.vertex_ids(), constants::vertex_ids_view));
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_rng_vid)), std::out_of_range
        );

        REQUIRE(rng::equal(sut.hyperedges() | vw::transform(get_id), constants::hyperedge_ids_view)
        );
        REQUIRE(rng::equal(sut.hyperedge_ids(), constants::hyperedge_ids_view));
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_hyperedge(constants::out_of_rng_eid)), std::out_of_range
        );
    }

    // --- vertex method tests ---

    SUBCASE("add_vertex should return a vertex_descriptor with an incremented id") {
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

    SUBCASE("remove_vertex(vertex) should throw if the given vertex is invalid") {
        sut_type sut{constants::n_vertices};
        CHECK_THROWS_AS(
            sut.remove_vertex(vertex_type{constants::out_of_rng_vid}), std::out_of_range
        );
    }

    SUBCASE("remove_vertex(vertex) should remove the given vertex and align ids of remaining "
            "vertices") {
        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices - 1uz);
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
        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices - 1uz);
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::n_vertices - 1uz)), std::out_of_range
        );
    }

    SUBCASE("remove_vertices_from(ids) should properly remove elements at given indices (ignoring "
            "duplicate indices)") {
        constexpr auto n_vertices = constants::n_vertices + 1uz;

        sut_type sut{n_vertices};
        sut.remove_vertices_from(
            std::vector<hgl::types::id_type>{constants::id1, constants::id3, constants::id1}
        );

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
    }

    SUBCASE("remove_vertices_from(vertices) should properly remove elements at given indices "
            "(ignoring duplicate vertices)") {
        constexpr auto n_vertices = constants::n_vertices + 1uz;

        sut_type sut{n_vertices};
        const auto v1 = sut.get_vertex(constants::id1);
        const auto v3 = sut.get_vertex(constants::id3);
        sut.remove_vertices_from(std::vector<vertex_type>{v1, v3, v1});

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
    }

    // --- hyperedge method tests ---

    SUBCASE("add_hyperedge should return a hyperedge_descriptor with an incremented id") {
        sut_type sut;

        for (gl::types::id_type e_id = 0uz; e_id < constants::n_hyperedges; e_id++) {
            const auto hyperedge = sut.add_hyperedge();
            CHECK_EQ(hyperedge.id(), e_id);
            CHECK_EQ(sut.n_hyperedges(), e_id + 1uz);
        }

        CHECK_EQ(sut.n_hyperedges(), constants::n_hyperedges);
    }

    SUBCASE("add_hyperedge_with should initialize a new hyperedge with the input properties "
            "structure") {
        using properties_traits_type =
            add_hyperedge_property<HypergraphTraits, types::boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const auto hyperedge = sut.add_hyperedge_with(constants::p_true);
        REQUIRE_EQ(sut.n_hyperedges(), 1uz);

        CHECK_EQ(hyperedge.id(), constants::id1);
        CHECK_EQ(hyperedge.properties(), constants::p_true);
    }

    SUBCASE("add_hyperedges(n) should add n new hyperedges to the hypergraph") {
        sut_type sut{};
        sut.add_hyperedges(constants::n_hyperedges);

        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_EQ(sut.n_hyperedges(), constants::n_hyperedges);
    }

    SUBCASE("add_hyperedges_with should add new hyperedges to the hypergraph with the given "
            "properties") {
        using properties_traits_type =
            add_hyperedge_property<HypergraphTraits, types::boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const std::vector<types::boolean_property> properties_list{
            constants::p_true, constants::p_false, constants::p_true, constants::p_false
        };
        const auto expected_n_hyperedges = properties_list.size();

        sut.add_hyperedges_with(properties_list);

        REQUIRE_EQ(sut.n_vertices(), 0uz);
        CHECK_EQ(sut.n_hyperedges(), expected_n_hyperedges);

        CHECK(rng::equal(
            sut.hyperedges(),
            properties_list,
            rng::equal_to{},
            [](const auto hyperedge) { return hyperedge.properties(); }
        ));
    }

    SUBCASE("has_hyperedge(id) should return true when a hyperedge with the given id is present in "
            "the graph") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};

        CHECK(rng::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
            return sut.has_hyperedge(hyperedge_id);
        }));
        CHECK_FALSE(sut.has_hyperedge(constants::out_of_rng_eid));
    }

    SUBCASE("get_hyperedge should throw if the given id is invalid") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_hyperedge(constants::out_of_rng_eid)), std::out_of_range
        );
    }

    SUBCASE("get_hyperedge should return a hyperedge with the given id") {
        sut_type sut;
        const auto added_hyperedge = sut.add_hyperedge();
        CHECK_EQ(sut.get_hyperedge(added_hyperedge.id()), added_hyperedge);
    }

    SUBCASE("remove_hyperedge(hyperedge) should throw if the given hyperedge is invalid") {
        sut_type sut{0uz, constants::n_hyperedges};
        CHECK_THROWS_AS(
            sut.remove_hyperedge(hyperedge_type{constants::out_of_rng_eid}), std::out_of_range
        );
    }

    SUBCASE("remove_hyperedge(hyperedge) should remove the given hyperedge and align ids of "
            "remaining hyperedges") {
        sut_type sut{0uz, constants::n_hyperedges};
        sut.remove_hyperedge(constants::id1);

        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges - 1uz);
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_hyperedge(constants::n_hyperedges - 1uz)), std::out_of_range
        );
    }

    SUBCASE("remove_hyperedge(id) should throw if the given id is invalid") {
        sut_type sut{0uz, constants::n_hyperedges};
        CHECK_THROWS_AS(sut.remove_hyperedge(constants::out_of_rng_eid), std::out_of_range);
    }

    SUBCASE("remove_hyperedge(id) should remove the given hyperedge and align ids of remaining "
            "hyperedges") {
        sut_type sut{0uz, constants::n_hyperedges};
        sut.remove_hyperedge(constants::id1);

        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges - 1uz);
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_hyperedge(constants::n_hyperedges - 1uz)), std::out_of_range
        );
    }

    SUBCASE("remove_hyperedges_from(ids) should properly remove elements at given indices "
            "(ignoring duplicate indices)") {
        constexpr auto n_hyperedges = constants::n_hyperedges + 1uz;

        sut_type sut{0uz, n_hyperedges};
        sut.remove_hyperedges_from(
            std::vector<hgl::types::id_type>{constants::id1, constants::id3, constants::id1}
        );

        constexpr auto expected_n_hyperedges = n_hyperedges - 2uz;
        REQUIRE_EQ(sut.n_hyperedges(), expected_n_hyperedges);
    }

    SUBCASE("remove_hyperedges_from(hyperedges) should properly remove elements at given indices "
            "(ignoring duplicate hyperedges)") {
        constexpr auto n_hyperedges = constants::n_hyperedges + 1uz;

        sut_type sut{0uz, n_hyperedges};
        const auto he1 = sut.get_hyperedge(constants::id1);
        const auto he3 = sut.get_hyperedge(constants::id3);
        sut.remove_hyperedges_from(std::vector<hyperedge_type>{he1, he3, he1});

        constexpr auto expected_n_hyperedges = n_hyperedges - 2uz;
        REQUIRE_EQ(sut.n_hyperedges(), expected_n_hyperedges);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major incicence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incicence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incicence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major incicence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major incicence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incicence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incicence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t> // bf-directed vertex-major incicence matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "properties getter tests", HypergraphTraits, property_hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;

    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    for (auto vertex : sut.vertices())
        vertex.properties() = std::format("vertex_{}", vertex.id());
    for (auto hyperedge : sut.hyperedges())
        hyperedge.properties() = std::format("hyperedge_{}", hyperedge.id());

    auto vmap = sut.vertex_properties_map();
    CHECK(vmap.size() == constants::n_vertices);
    for (auto [id, property] : vw::zip(sut.vertex_ids(), vmap)) {
        CHECK_EQ(property, std::format("vertex_{}", id));
        CHECK_EQ(vmap[id], std::format("vertex_{}", id));
        CHECK_EQ(sut.get_vertex_properties(id), std::format("vertex_{}", id));
    }
    CHECK_THROWS_AS(
        static_cast<void>(sut.get_vertex_properties(constants::out_of_rng_vid)), std::out_of_range
    );

    auto emap = sut.hyperedge_properties_map();
    CHECK(emap.size() == constants::n_hyperedges);
    for (auto [id, property] : vw::zip(sut.hyperedge_ids(), emap)) {
        CHECK_EQ(property, std::format("hyperedge_{}", id));
        CHECK_EQ(emap[id], std::format("hyperedge_{}", id));
        CHECK_EQ(sut.get_hyperedge_properties(id), std::format("hyperedge_{}", id));
    }
    CHECK_THROWS_AS(
        static_cast<void>(sut.get_hyperedge_properties(constants::out_of_rng_eid)),
        std::out_of_range
    );
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    property_hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected hyperedge-major incicence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed hyperedge-major incicence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected vertex-major incicence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed vertex-major incicence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected hyperedge-major incicence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed hyperedge-major incicence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected vertex-major incicence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property> // bf-directed vertex-major incicence matrix
);

TEST_SUITE_END(); // test_hypergraph

} // namespace hgl_testing
