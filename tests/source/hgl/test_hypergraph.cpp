#include "doctest.h"
#include "hgl/hypergraph_elements.hpp"
#include "testing/common/functional.hpp"
#include "testing/hgl/constants.hpp"
#include "testing/hgl/types.hpp"

#include <gl/attributes/diagnostics.hpp>

#include <hgl/directional_tags.hpp>
#include <hgl/hypergraph.hpp>
#include <hgl/hypergraph_traits.hpp>
#include <hgl/impl/layout_tags.hpp>

#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <vector>

namespace rng = std::ranges;
namespace vw = std::views;

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph");

template <
    hgl::traits::c_instantiation_of<hgl::hypergraph_traits> HypergraphTraits,
    hgl::traits::c_properties VertexProperties>
using add_vertex_property = hgl::hypergraph_traits<
    typename HypergraphTraits::directional_tag,
    VertexProperties,
    typename HypergraphTraits::hyperedge_properties_type,
    typename HypergraphTraits::implementation_tag>;

template <
    hgl::traits::c_instantiation_of<hgl::hypergraph_traits> HypergraphTraits,
    hgl::traits::c_properties HyperedgeProperties>
using add_hyperedge_property = hgl::hypergraph_traits<
    typename HypergraphTraits::directional_tag,
    typename HypergraphTraits::vertex_properties_type,
    HyperedgeProperties,
    typename HypergraphTraits::implementation_tag>;

template <
    hgl::traits::c_instantiation_of<hgl::hypergraph_traits> HypergraphTraits,
    hgl::traits::c_properties Properties>
using add_properties = hgl::hypergraph_traits<
    typename HypergraphTraits::directional_tag,
    Properties,
    Properties,
    typename HypergraphTraits::implementation_tag>;

inline constexpr auto get_id = [](auto&& element) -> hgl::default_id_type { return element.id(); };

TEST_CASE_TEMPLATE_DEFINE(
    "hypergraph structure tests", HypergraphTraits, hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using directional_tag = typename sut_type::directional_tag;
    using vertex_type = typename sut_type::vertex_type;
    using hyperedge_type = typename sut_type::hyperedge_type;

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

        CHECK_THROWS_AS(discard(sut.vertex(constants::out_of_rng_vid)), std::invalid_argument);
    }

    SUBCASE("a hypergraph constructed with n_vertices and n_hyperedges parameters should contain "
            "n_vertices vertices and n_hyperedges hyperedges") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices);
        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges);

        REQUIRE(rng::equal(sut.vertices() | vw::transform(get_id), constants::vertex_ids_view));
        REQUIRE(rng::equal(sut.vertex_ids(), constants::vertex_ids_view));
        CHECK_THROWS_AS(discard(sut.vertex(constants::out_of_rng_vid)), std::invalid_argument);

        REQUIRE(rng::equal(sut.hyperedges() | vw::transform(get_id), constants::hyperedge_ids_view)
        );
        REQUIRE(rng::equal(sut.hyperedge_ids(), constants::hyperedge_ids_view));
        CHECK_THROWS_AS(discard(sut.hyperedge(constants::out_of_rng_eid)), std::invalid_argument);
    }

    // --- vertex modifiers ---

    SUBCASE("add_vertex should return a vertex_descriptor with an incremented id") {
        sut_type sut;

        for (auto v_id = 0u; v_id < constants::n_vertices; v_id++) {
            const auto vertex = sut.add_vertex();
            CHECK_EQ(vertex.id(), v_id);
            CHECK_EQ(sut.n_vertices(), v_id + 1uz);
            CHECK_EQ(sut.degree(v_id), 0uz);
        }

        CHECK_EQ(sut.n_vertices(), constants::n_vertices);
    }

    SUBCASE("add_vertex_with should initialize a new vertex with the input properties structure") {
        using properties_traits_type = add_vertex_property<HypergraphTraits, boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const auto vertex = sut.add_vertex_with(constants::p_true);
        REQUIRE_EQ(sut.n_vertices(), 1uz);

        CHECK_EQ(vertex.id(), constants::id1);
        CHECK_EQ(vertex.properties(), constants::p_true);
        CHECK_EQ(sut.degree(vertex), 0uz);
    }

    SUBCASE("add_vertices(n) should add n new vertices to the hypergraph") {
        sut_type sut{};
        sut.add_vertices(constants::n_vertices);

        CHECK_EQ(sut.n_vertices(), constants::n_vertices);
        CHECK_EQ(sut.n_hyperedges(), 0uz);
    }

    SUBCASE("add_vertices_with should add new vertices to the hypergraph with the given properties"
    ) {
        using properties_traits_type = add_vertex_property<HypergraphTraits, boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const std::vector<boolean_property> properties_list{
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

    SUBCASE("remove_vertex(vertex) should do nothing if the given vertex is invalid") {
        sut_type sut{constants::n_vertices};
        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices);

        CHECK_NOTHROW(sut.remove_vertex(vertex_type{constants::out_of_rng_vid}));
        CHECK_EQ(sut.n_vertices(), constants::n_vertices);
    }

    SUBCASE("remove_vertex(vertex) should remove the given vertex and align ids of remaining "
            "vertices") {
        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices - 1uz);
        CHECK_THROWS_AS(discard(sut.vertex(constants::n_vertices - 1uz)), std::invalid_argument);
    }

    SUBCASE("remove_vertex(id) should do nothing if the given id is invalid") {
        sut_type sut{constants::n_vertices};
        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices);

        CHECK_NOTHROW(sut.remove_vertex(constants::out_of_rng_vid));
        CHECK_EQ(sut.n_vertices(), constants::n_vertices);
    }

    SUBCASE("remove_vertex(id) should remove the given vertex and align ids of remaining vertices"
    ) {
        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        REQUIRE_EQ(sut.n_vertices(), constants::n_vertices - 1uz);
        CHECK_THROWS_AS(discard(sut.vertex(constants::n_vertices - 1uz)), std::invalid_argument);
    }

    SUBCASE("remove_vertices(ids) should properly remove elements at given indices (ignoring "
            "duplicate indices)") {
        constexpr auto n_vertices = constants::n_vertices + 1uz;

        sut_type sut{n_vertices};
        sut.remove_vertices(
            std::vector<hgl::default_id_type>{constants::id1, constants::id3, constants::id1}
        );

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
    }

    SUBCASE("remove_vertices(vertices) should properly remove elements at given indices "
            "(ignoring duplicate vertices)") {
        constexpr auto n_vertices = constants::n_vertices + 1uz;

        sut_type sut{n_vertices};
        const auto v1 = sut.vertex(constants::id1);
        const auto v3 = sut.vertex(constants::id3);
        sut.remove_vertices(std::vector<vertex_type>{v1, v3, v1});

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.n_vertices(), expected_n_vertices);
    }

    // --- vertex getters ---

    SUBCASE("has_vertex(id) should return true when a vertex with the given id is present in "
            "the graph") {
        sut_type sut{constants::n_vertices};

        CHECK(rng::all_of(constants::vertex_ids_view, [&sut](const auto vertex_id) {
            return sut.has_vertex(vertex_id);
        }));
        CHECK_FALSE(sut.has_vertex(constants::out_of_rng_vid));
    }

    SUBCASE("vertex/at should throw if the given id is invalid") {
        sut_type sut{constants::n_vertices};
        CHECK_THROWS_AS(discard(sut.vertex(constants::out_of_rng_vid)), std::invalid_argument);
        CHECK_THROWS_AS(
            discard(sut.at(hgl::vertex, constants::out_of_rng_vid)), std::invalid_argument
        );
    }

    SUBCASE("vertex/at should return a vertex with the given id") {
        sut_type sut;
        const auto added_vertex = sut.add_vertex();
        CHECK_EQ(sut.vertex(added_vertex.id()), added_vertex);
        CHECK_EQ(sut.at(hgl::vertex, added_vertex.id()), added_vertex);
    }

    SUBCASE("vertex_uncheckekd/operator[] should not throw if the given id is invalid (UB)") {
        sut_type sut{constants::n_vertices};
        CHECK_NOTHROW(discard(sut.vertex_unchecked(constants::out_of_rng_vid)));
        CHECK_NOTHROW(discard(sut[hgl::vertex, constants::out_of_rng_vid]));
    }

    SUBCASE("vertex_uncheckekd/operator[] should return a vertex with the given id") {
        sut_type sut;
        const auto added_vertex = sut.add_vertex();
        CHECK_EQ(sut.vertex_unchecked(added_vertex.id()), added_vertex);
        CHECK_EQ(sut[hgl::vertex, added_vertex.id()], added_vertex);
    }

    // --- hyperedge modifers ---

    SUBCASE("add_hyperedge should return a hyperedge_descriptor with an incremented id") {
        sut_type sut;

        for (auto e_id = 0u; e_id < constants::n_hyperedges; e_id++) {
            const auto hyperedge = sut.add_hyperedge();
            CHECK_EQ(hyperedge.id(), e_id);
            CHECK_EQ(sut.n_hyperedges(), e_id + 1uz);
        }

        CHECK_EQ(sut.n_hyperedges(), constants::n_hyperedges);
    }

    SUBCASE("add_hyperedge(<vertices>) should create a hyperedge and bind the given vertices") {
        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            sut_type sut{3uz};
            const std::vector<hgl::default_id_type> v_ids{0u, 1u, 2u};

            auto he1 = sut.add_hyperedge(v_ids);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he1), v_ids));

            auto vertices = v_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            auto he2 = sut.add_hyperedge(vertices);
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he2), v_ids));
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            sut_type sut{4uz};
            const std::vector<hgl::default_id_type> t_ids{0u, 3u};
            const std::vector<hgl::default_id_type> h_ids{1u, 2u};

            auto he1 = sut.add_hyperedge(t_ids, h_ids);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.tail_ids(he1), t_ids));
            CHECK(rng::equal(sut.head_ids(he1), h_ids));

            auto h_vertices = h_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            auto t_vertices = t_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            auto he2 = sut.add_hyperedge(t_vertices, h_vertices);
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.tail_ids(he2), t_ids));
            CHECK(rng::equal(sut.head_ids(he2), h_ids));
        }
    }

    SUBCASE("add_hyperedge(initializer_list) variants should create a hyperedge and bind the given "
            "vertices") {
        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            sut_type sut{3uz};
            const std::vector<hgl::default_id_type> v_ids{0u, 1u, 2u};

            auto he1 = sut.add_hyperedge(v_ids);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he1), v_ids));

            auto he2 = sut.add_hyperedge({sut.vertex(0u), sut.vertex(1u), sut.vertex(2u)});
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he2), v_ids));
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            sut_type sut{4uz};

            std::initializer_list<hgl::default_id_type> t_ids = {0u, 1u};
            std::initializer_list<hgl::default_id_type> h_ids = {2u, 3u};

            auto he1 = sut.add_hyperedge(t_ids, h_ids);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.tail_ids(he1), t_ids));
            CHECK(rng::equal(sut.head_ids(he1), h_ids));

            auto he2 = sut.add_hyperedge(
                {sut.vertex(0u), sut.vertex(1u)}, {sut.vertex(2u), sut.vertex(3u)}
            );
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.tail_ids(he2), t_ids));
            CHECK(rng::equal(sut.head_ids(he2), h_ids));
        }
    }

    SUBCASE("add_hyperedge_with should initialize a new hyperedge with the input properties "
            "structure") {
        using properties_traits_type = add_hyperedge_property<HypergraphTraits, boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const auto hyperedge = sut.add_hyperedge_with(constants::p_true);
        REQUIRE_EQ(sut.n_hyperedges(), 1uz);

        CHECK_EQ(hyperedge.id(), constants::id1);
        CHECK_EQ(hyperedge.properties(), constants::p_true);
    }

    SUBCASE("add_hyperedge_with(<vertices>, properties) variants should create a hyperedge, bind "
            "vertices, and set properties") {
        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            using properties_traits_type =
                add_hyperedge_property<HypergraphTraits, boolean_property>;

            hgl::hypergraph<properties_traits_type> sut{4uz};
            const std::vector<hgl::default_id_type> v_ids{0u, 1u, 3u};

            auto he1 = sut.add_hyperedge_with(v_ids, constants::p_true);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he1), v_ids));
            CHECK_EQ(he1.properties(), constants::p_true);

            auto vertices = v_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            auto he2 = sut.add_hyperedge_with(vertices, constants::p_false);
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he2), v_ids));
            CHECK_EQ(he2.properties(), constants::p_false);
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            using properties_traits_type =
                add_hyperedge_property<HypergraphTraits, boolean_property>;

            hgl::hypergraph<properties_traits_type> sut{4uz};
            const std::vector<hgl::default_id_type> t_ids{0u, 1u};
            const std::vector<hgl::default_id_type> h_ids{2u, 3u};

            auto he1 = sut.add_hyperedge_with(t_ids, h_ids, constants::p_true);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.tail_ids(he1), t_ids));
            CHECK(rng::equal(sut.head_ids(he1), h_ids));
            CHECK_EQ(he1.properties(), constants::p_true);

            auto t_vertices = t_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            auto h_vertices = h_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            auto he2 = sut.add_hyperedge_with(t_vertices, h_vertices, constants::p_false);
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.tail_ids(he2), t_ids));
            CHECK(rng::equal(sut.head_ids(he2), h_ids));
            CHECK_EQ(he2.properties(), constants::p_false);
        }
    }

    SUBCASE("add_hyperedge_with(initializer_list, properties) variants should create a hyperedge, "
            "bind vertices, and set properties") {
        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            using properties_traits_type =
                add_hyperedge_property<HypergraphTraits, boolean_property>;

            hgl::hypergraph<properties_traits_type> sut{4uz};
            std::initializer_list<hgl::default_id_type> v_ids{0u, 1u, 3u};

            auto he1 = sut.add_hyperedge_with(v_ids, constants::p_true);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he1), v_ids));
            CHECK_EQ(he1.properties(), constants::p_true);

            auto he2 = sut.add_hyperedge_with(
                {sut.vertex(0u), sut.vertex(1u), sut.vertex(3u)}, constants::p_false
            );
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.incident_vertex_ids(he2), v_ids));
            CHECK_EQ(he2.properties(), constants::p_false);
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            using properties_traits_type =
                add_hyperedge_property<HypergraphTraits, boolean_property>;

            hgl::hypergraph<properties_traits_type> sut{4uz};
            std::initializer_list<hgl::default_id_type> t_ids{0u, 1u};
            std::initializer_list<hgl::default_id_type> h_ids{2u, 3u};

            auto he1 = sut.add_hyperedge_with(t_ids, h_ids, constants::p_true);
            CHECK_EQ(sut.n_hyperedges(), 1uz);
            CHECK(rng::equal(sut.tail_ids(he1), t_ids));
            CHECK(rng::equal(sut.head_ids(he1), h_ids));
            CHECK_EQ(he1.properties(), constants::p_true);

            auto he2 = sut.add_hyperedge_with(
                {sut.vertex(0u), sut.vertex(1u)},
                {sut.vertex(2u), sut.vertex(3u)},
                constants::p_false
            );
            CHECK_EQ(sut.n_hyperedges(), 2uz);
            CHECK(rng::equal(sut.tail_ids(he2), t_ids));
            CHECK(rng::equal(sut.head_ids(he2), h_ids));
            CHECK_EQ(he2.properties(), constants::p_false);
        }
    }

    SUBCASE("add_hyperedges(n) should add n new hyperedges to the hypergraph") {
        sut_type sut{};
        sut.add_hyperedges(constants::n_hyperedges);

        CHECK_EQ(sut.n_vertices(), 0uz);
        CHECK_EQ(sut.n_hyperedges(), constants::n_hyperedges);
    }

    SUBCASE("add_hyperedges_with should add new hyperedges to the hypergraph with the given "
            "properties") {
        using properties_traits_type = add_hyperedge_property<HypergraphTraits, boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const std::vector<boolean_property> properties_list{
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

    SUBCASE("remove_hyperedge(hyperedge) should do nothing if the given hyperedge is invalid") {
        sut_type sut{0uz, constants::n_hyperedges};
        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges);

        CHECK_NOTHROW(sut.remove_hyperedge(hyperedge_type{constants::out_of_rng_eid}));
        CHECK_EQ(sut.n_hyperedges(), constants::n_hyperedges);
    }

    SUBCASE("remove_hyperedge(hyperedge) should remove the given hyperedge and align ids of "
            "remaining hyperedges") {
        sut_type sut{0uz, constants::n_hyperedges};
        sut.remove_hyperedge(constants::id1);

        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges - 1uz);
        CHECK_THROWS_AS(
            discard(sut.hyperedge(constants::n_hyperedges - 1uz)), std::invalid_argument
        );
    }

    SUBCASE("remove_hyperedge(id) should do nothing if the given id is invalid") {
        sut_type sut{0uz, constants::n_hyperedges};
        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges);

        CHECK_NOTHROW(sut.remove_hyperedge(constants::out_of_rng_eid));
        CHECK_EQ(sut.n_hyperedges(), constants::n_hyperedges);
    }

    SUBCASE("remove_hyperedge(id) should remove the given hyperedge and align ids of remaining "
            "hyperedges") {
        sut_type sut{0uz, constants::n_hyperedges};
        sut.remove_hyperedge(constants::id1);

        REQUIRE_EQ(sut.n_hyperedges(), constants::n_hyperedges - 1uz);
        CHECK_THROWS_AS(
            discard(sut.hyperedge(constants::n_hyperedges - 1uz)), std::invalid_argument
        );
    }

    SUBCASE("remove_hyperedges_from(ids) should properly remove elements at given indices "
            "(ignoring duplicate indices)") {
        constexpr auto n_hyperedges = constants::n_hyperedges + 1uz;

        sut_type sut{0uz, n_hyperedges};
        sut.remove_hyperedges_from(
            std::vector<hgl::default_id_type>{constants::id1, constants::id3, constants::id1}
        );

        constexpr auto expected_n_hyperedges = n_hyperedges - 2uz;
        REQUIRE_EQ(sut.n_hyperedges(), expected_n_hyperedges);
    }

    SUBCASE("remove_hyperedges_from(hyperedges) should properly remove elements at given indices "
            "(ignoring duplicate hyperedges)") {
        constexpr auto n_hyperedges = constants::n_hyperedges + 1uz;

        sut_type sut{0uz, n_hyperedges};
        const auto he1 = sut.hyperedge(constants::id1);
        const auto he3 = sut.hyperedge(constants::id3);
        sut.remove_hyperedges_from(std::vector<hyperedge_type>{he1, he3, he1});

        constexpr auto expected_n_hyperedges = n_hyperedges - 2uz;
        REQUIRE_EQ(sut.n_hyperedges(), expected_n_hyperedges);
    }

    // --- hyperedge getters ---

    SUBCASE("has_hyperedge(id) should return true when a hyperedge with the given id is present in "
            "the graph") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};

        CHECK(rng::all_of(constants::hyperedge_ids_view, [&sut](const auto hyperedge_id) {
            return sut.has_hyperedge(hyperedge_id);
        }));
        CHECK_FALSE(sut.has_hyperedge(constants::out_of_rng_eid));
    }

    SUBCASE("hyperedge/at should throw if the given id is invalid") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK_THROWS_AS(discard(sut.hyperedge(constants::out_of_rng_eid)), std::invalid_argument);
        CHECK_THROWS_AS(
            discard(sut.at(hgl::hyperedge, constants::out_of_rng_eid)), std::invalid_argument
        );
    }

    SUBCASE("hyperedge/at should return a hyperedge with the given id") {
        sut_type sut;
        const auto added_hyperedge = sut.add_hyperedge();
        CHECK_EQ(sut.hyperedge(added_hyperedge.id()), added_hyperedge);
        CHECK_EQ(sut.at(hgl::hyperedge, added_hyperedge.id()), added_hyperedge);
    }

    SUBCASE("hyperedge_unchecked/operator[] should not throw if the given id is invalid (UB)") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK_NOTHROW(discard(sut.hyperedge_unchecked(constants::out_of_rng_eid)));
        CHECK_NOTHROW(discard(sut[hgl::hyperedge, constants::out_of_rng_eid]));
    }

    SUBCASE("hyperedge_unchecked/operator[] should return a hyperedge with the given id") {
        sut_type sut;
        const auto added_hyperedge = sut.add_hyperedge();
        CHECK_EQ(sut.hyperedge_unchecked(added_hyperedge.id()), added_hyperedge);
        CHECK_EQ(sut[hgl::hyperedge, added_hyperedge.id()], added_hyperedge);
    }

    // --- incidence method tests ---

    SUBCASE("bind, unbind and are_incident should throw if either of the fiven elements is invalid"
    ) {
        GL_SUPPRESS_WARNING_BEGIN("-Warray-bounds");

        sut_type sut{constants::n_vertices, constants::n_hyperedges};

        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            CHECK_THROWS_AS(
                sut.bind(constants::out_of_rng_vid, constants::out_of_rng_eid),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                sut.bind(constants::id1, constants::out_of_rng_eid), std::invalid_argument
            );
            CHECK_THROWS_AS(
                sut.bind(constants::out_of_rng_vid, constants::id1), std::invalid_argument
            );
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            CHECK_THROWS_AS(
                sut.bind_tail(constants::out_of_rng_vid, constants::out_of_rng_eid),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                sut.bind_tail(constants::id1, constants::out_of_rng_eid), std::invalid_argument
            );
            CHECK_THROWS_AS(
                sut.bind_tail(constants::out_of_rng_vid, constants::id1), std::invalid_argument
            );

            CHECK_THROWS_AS(
                sut.bind_head(constants::out_of_rng_vid, constants::out_of_rng_eid),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                sut.bind_head(constants::id1, constants::out_of_rng_eid), std::invalid_argument
            );
            CHECK_THROWS_AS(
                sut.bind_head(constants::out_of_rng_vid, constants::id1), std::invalid_argument
            );
        }

        CHECK_THROWS_AS(
            sut.unbind(constants::out_of_rng_vid, constants::out_of_rng_eid), std::invalid_argument
        );
        CHECK_THROWS_AS(
            sut.unbind(constants::id1, constants::out_of_rng_eid), std::invalid_argument
        );
        CHECK_THROWS_AS(
            sut.unbind(constants::out_of_rng_vid, constants::id1), std::invalid_argument
        );

        CHECK_THROWS_AS(
            discard(sut.are_incident(constants::out_of_rng_vid, constants::out_of_rng_eid)),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            discard(sut.are_incident(constants::id1, constants::out_of_rng_eid)),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            discard(sut.are_incident(constants::out_of_rng_vid, constants::id1)),
            std::invalid_argument
        );

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            CHECK_THROWS_AS(
                discard(sut.is_tail(constants::out_of_rng_vid, constants::out_of_rng_eid)),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                discard(sut.is_tail(constants::id1, constants::out_of_rng_eid)),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                discard(sut.is_tail(constants::out_of_rng_vid, constants::id1)),
                std::invalid_argument
            );

            CHECK_THROWS_AS(
                discard(sut.is_head(constants::out_of_rng_vid, constants::out_of_rng_eid)),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                discard(sut.is_head(constants::id1, constants::out_of_rng_eid)),
                std::invalid_argument
            );
            CHECK_THROWS_AS(
                discard(sut.is_head(constants::out_of_rng_vid, constants::id1)),
                std::invalid_argument
            );
        }

        GL_SUPPRESS_WARNING_END;
    }

    SUBCASE("are_incident should return false by default") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        for (const auto vertex : sut.vertices())
            for (const auto hyperedge : sut.hyperedges())
                CHECK_FALSE(sut.are_incident(vertex, hyperedge));
    }

    SUBCASE("bind should properly mark the given vertex and hyperedge as incident and unbind "
            "should mark them as not incident") {
        constexpr auto vertex_id = constants::id1;
        constexpr auto hyperedge_id = constants::id2;
        constexpr auto unbound_id = constants::id3;

        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        REQUIRE_FALSE(sut.are_incident(vertex_id, hyperedge_id));
        REQUIRE_FALSE(sut.are_incident(vertex_id, unbound_id));
        REQUIRE_FALSE(sut.are_incident(unbound_id, hyperedge_id));

        const auto validate_incidence = [&](const bool incident) {
            CHECK_EQ(sut.are_incident(vertex_id, hyperedge_id), incident);
            CHECK_FALSE(sut.are_incident(vertex_id, unbound_id));
            CHECK_FALSE(sut.are_incident(unbound_id, hyperedge_id));
        };

        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            sut.bind(vertex_id, hyperedge_id);
            validate_incidence(true);
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            SUBCASE("bind head") {
                sut.bind_head(vertex_id, hyperedge_id);
                validate_incidence(true);
                CHECK(sut.is_head(vertex_id, hyperedge_id));
                CHECK_FALSE(sut.is_tail(vertex_id, hyperedge_id));
            }
            SUBCASE("bind tail") {
                sut.bind_tail(vertex_id, hyperedge_id);
                validate_incidence(true);
                CHECK(sut.is_tail(vertex_id, hyperedge_id));
                CHECK_FALSE(sut.is_head(vertex_id, hyperedge_id));
            }
        }

        sut.unbind(vertex_id, hyperedge_id);
        validate_incidence(false);
    }

    SUBCASE("bulk bind (undirected) should properly mark given sets as incident") {
        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            sut_type sut{4uz, 4uz};

            std::vector<hgl::default_id_type> he1_v_ids{0u, 1u};
            sut.bind(he1_v_ids, 0u);
            CHECK(rng::equal(sut.incident_vertex_ids(0u), he1_v_ids));

            auto he2_vertices =
                he1_v_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            sut.bind(he2_vertices, sut.hyperedge(1u));
            CHECK(rng::equal(sut.incident_vertex_ids(1u), he1_v_ids));

            std::vector<hgl::default_id_type> v3_he_ids{2u, 3u};
            sut.bind(2u, v3_he_ids);
            CHECK(rng::equal(sut.incident_hyperedge_ids(2u), v3_he_ids));

            auto v4_hyperedges =
                v3_he_ids | vw::transform([&](const auto id) { return sut.hyperedge(id); });
            sut.bind(sut.vertex(3u), v4_hyperedges);
            CHECK(rng::equal(sut.incident_hyperedge_ids(3u), v3_he_ids));
        }
    }

    SUBCASE("bulk bind (initializer list, undirected) should properly mark given sets as incident"
    ) {
        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            sut_type sut{4uz, 4uz};

            std::initializer_list<hgl::default_id_type> he1_v_ids{0u, 1u};
            sut.bind(he1_v_ids, 0u);
            CHECK(rng::equal(sut.incident_vertex_ids(0u), he1_v_ids));

            sut.bind({sut.vertex(0u), sut.vertex(1u)}, sut.hyperedge(1u));
            CHECK(rng::equal(sut.incident_vertex_ids(1u), he1_v_ids));

            std::initializer_list<hgl::default_id_type> v3_he_ids{2u, 3u};
            sut.bind(2u, v3_he_ids);
            CHECK(rng::equal(sut.incident_hyperedge_ids(2u), v3_he_ids));

            sut.bind(sut.vertex(3u), {sut.hyperedge(2u), sut.hyperedge(3u)});
            CHECK(rng::equal(sut.incident_hyperedge_ids(3u), v3_he_ids));
        }
    }

    SUBCASE("bulk bind_tail (bf-directed) should properly mark given sets as incident") {
        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            sut_type sut{4uz, 4uz};

            std::vector<hgl::default_id_type> he1_v_ids{0u, 1u};
            sut.bind_tail(he1_v_ids, 0u);
            CHECK(rng::equal(sut.tail_ids(0u), he1_v_ids));

            auto he2_vertices =
                he1_v_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            sut.bind_tail(he2_vertices, sut.hyperedge(1u));
            CHECK(rng::equal(sut.tail_ids(1u), he1_v_ids));

            std::vector<hgl::default_id_type> v3_he_ids{2u, 3u};
            sut.bind_tail(2u, v3_he_ids);
            CHECK(rng::equal(sut.out_hyperedge_ids(2u), v3_he_ids));

            auto v4_hyperedges =
                v3_he_ids | vw::transform([&](const auto id) { return sut.hyperedge(id); });
            sut.bind_tail(sut.vertex(3u), v4_hyperedges);
            CHECK(rng::equal(sut.out_hyperedge_ids(3u), v3_he_ids));
        }
    }

    SUBCASE("bulk bind_tail (initializer list, bf-directed) should properly mark given sets as "
            "incident") {
        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            sut_type sut{4uz, 4uz};

            std::initializer_list<hgl::default_id_type> he1_v_ids{0u, 1u};
            sut.bind_tail(he1_v_ids, 0u);
            CHECK(rng::equal(sut.tail_ids(0u), he1_v_ids));

            sut.bind_tail({sut.vertex(0u), sut.vertex(1u)}, sut.hyperedge(1u));
            CHECK(rng::equal(sut.tail_ids(1u), he1_v_ids));

            std::initializer_list<hgl::default_id_type> v3_he_ids{2u, 3u};
            sut.bind_tail(2u, v3_he_ids);
            CHECK(rng::equal(sut.out_hyperedge_ids(2u), v3_he_ids));

            sut.bind_tail(sut.vertex(3u), {sut.hyperedge(2u), sut.hyperedge(3u)});
            CHECK(rng::equal(sut.out_hyperedge_ids(3u), v3_he_ids));
        }
    }

    SUBCASE("bulk bind_head (bf-directed) should properly mark given sets as incident") {
        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            sut_type sut{4uz, 4uz};

            std::vector<hgl::default_id_type> he1_v_ids{0u, 1u};
            sut.bind_head(he1_v_ids, 0u);
            CHECK(rng::equal(sut.head_ids(0u), he1_v_ids));

            auto he2_vertices =
                he1_v_ids | vw::transform([&](const auto id) { return sut.vertex(id); });
            sut.bind_head(he2_vertices, sut.hyperedge(1u));
            CHECK(rng::equal(sut.head_ids(1u), he1_v_ids));

            std::vector<hgl::default_id_type> v3_he_ids{2u, 3u};
            sut.bind_head(2u, v3_he_ids);
            CHECK(rng::equal(sut.in_hyperedge_ids(2u), v3_he_ids));

            auto v4_hyperedges =
                v3_he_ids | vw::transform([&](const auto id) { return sut.hyperedge(id); });
            sut.bind_head(sut.vertex(3u), v4_hyperedges);
            CHECK(rng::equal(sut.in_hyperedge_ids(3u), v3_he_ids));
        }
    }

    SUBCASE("bulk bind_head (initializer list, bf-directed) should properly mark given sets as "
            "incident") {
        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            sut_type sut{4uz, 4uz};

            std::initializer_list<hgl::default_id_type> he1_v_ids{0u, 1u};
            sut.bind_head(he1_v_ids, 0u);
            CHECK(rng::equal(sut.head_ids(0u), he1_v_ids));

            sut.bind_head({sut.vertex(0u), sut.vertex(1u)}, sut.hyperedge(1u));
            CHECK(rng::equal(sut.head_ids(1u), he1_v_ids));

            std::initializer_list<hgl::default_id_type> v3_he_ids{2u, 3u};
            sut.bind_head(2u, v3_he_ids);
            CHECK(rng::equal(sut.in_hyperedge_ids(2u), v3_he_ids));

            sut.bind_head(sut.vertex(3u), {sut.hyperedge(2u), sut.hyperedge(3u)});
            CHECK(rng::equal(sut.in_hyperedge_ids(3u), v3_he_ids));
        }
    }

    SUBCASE("incident_hyperedges and degree should throw if the given vertex (id) is invalid") {
        GL_SUPPRESS_WARNING_BEGIN("-Warray-bounds");

        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK_THROWS_AS(
            discard(sut.incident_hyperedges(vertex_type{constants::out_of_rng_vid})),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            discard(sut.degree(vertex_type{constants::out_of_rng_vid})), std::invalid_argument
        );

        GL_SUPPRESS_WARNING_END;
    }

    SUBCASE("incident_hyperedges should return an empty view by default and degree should return 0 "
            "by default") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK(std::ranges::all_of(sut.vertices(), [&sut](const auto& vertex) {
            return std::ranges::empty(sut.incident_hyperedges(vertex));
        }));
        CHECK(std::ranges::all_of(sut.vertices(), [&sut](const auto& vertex) {
            return sut.degree(vertex) == 0uz;
        }));
    }

    SUBCASE("incident_hyperedges should return a view containing ids of hyperedges incident with "
            "the given vertex, and degree should return their count") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        constexpr auto vertex_id = constants::id1;

        SUBCASE("sequential hyperedges") {
            if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
                std::vector<hgl::default_id_type> expected_hyperedges{};
                for (const auto eid : sut.hyperedge_ids()) {
                    sut.bind(vertex_id, eid);
                    expected_hyperedges.push_back(eid);

                    CHECK(std::ranges::equal(
                        sut.incident_hyperedges(vertex_id),
                        expected_hyperedges,
                        std::equal_to{},
                        get_id
                    ));
                    CHECK_EQ(sut.degree(vertex_id), expected_hyperedges.size());
                }
            }

            if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
                std::vector<hgl::default_id_type> expected_hyperedges;
                std::vector<hgl::default_id_type> expected_in_hyperedges, expected_out_hyperedges;
                for (const auto eid : sut.hyperedge_ids()) {
                    if (eid % 2 == 0) {
                        sut.bind_head(vertex_id, eid);
                        expected_in_hyperedges.push_back(eid);
                    }
                    else {
                        sut.bind_tail(vertex_id, eid);
                        expected_out_hyperedges.push_back(eid);
                    }
                    expected_hyperedges.push_back(eid);

                    CHECK(std::ranges::is_permutation(
                        sut.incident_hyperedges(vertex_id),
                        expected_hyperedges,
                        std::equal_to{},
                        get_id
                    ));
                    CHECK_EQ(sut.degree(vertex_id), expected_hyperedges.size());

                    CHECK(std::ranges::equal(
                        sut.in_hyperedges(vertex_id), expected_in_hyperedges, std::equal_to{}, get_id
                    ));
                    CHECK_EQ(sut.in_degree(vertex_id), expected_in_hyperedges.size());

                    CHECK(std::ranges::equal(
                        sut.out_hyperedges(vertex_id),
                        expected_out_hyperedges,
                        std::equal_to{},
                        get_id
                    ));
                    CHECK_EQ(sut.out_degree(vertex_id), expected_out_hyperedges.size());
                }
            }
        }

        SUBCASE("specific hyperedges") {
            if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
                sut.bind(vertex_id, constants::id2);
                sut.bind(vertex_id, constants::id4);

                CHECK(std::ranges::equal(
                    sut.incident_hyperedges(vertex_id),
                    std::vector<hgl::default_id_type>{constants::id2, constants::id4},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.degree(vertex_id), 2uz);
            }

            if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
                sut.bind_head(vertex_id, constants::id2);
                sut.bind_tail(vertex_id, constants::id4);

                CHECK(std::ranges::is_permutation(
                    sut.incident_hyperedges(vertex_id),
                    std::vector<hgl::default_id_type>{constants::id2, constants::id4},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.degree(vertex_id), 2uz);

                CHECK(std::ranges::equal(
                    sut.in_hyperedges(vertex_id),
                    std::vector<hgl::default_id_type>{constants::id2},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.in_degree(vertex_id), 1uz);

                CHECK(std::ranges::equal(
                    sut.out_hyperedges(vertex_id),
                    std::vector<hgl::default_id_type>{constants::id4},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.out_degree(vertex_id), 1uz);
            }
        }
    }

    SUBCASE("incident_vertices and hyperedge_size should throw if the given hyperedge (id) is "
            "invalid") {
        GL_SUPPRESS_WARNING_BEGIN("-Warray-bounds");

        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK_THROWS_AS(
            discard(sut.incident_vertices(hyperedge_type{constants::out_of_rng_eid})),
            std::invalid_argument
        );
        CHECK_THROWS_AS(
            discard(sut.hyperedge_size(hyperedge_type{constants::out_of_rng_eid})),
            std::invalid_argument
        );

        GL_SUPPRESS_WARNING_END;
    }

    SUBCASE("incident_vertices should return an empty view by default and hyperedge_size should "
            "return 0 by default") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK(std::ranges::all_of(sut.hyperedges(), [&sut](const auto& hyperedge) {
            return std::ranges::empty(sut.incident_vertices(hyperedge));
        }));
        CHECK(std::ranges::all_of(sut.vertices(), [&sut](const auto& hyperedge) {
            return sut.hyperedge_size(hyperedge) == 0uz;
        }));
    }

    SUBCASE("incident_vertices should return a view containing ids of vertices incident with the "
            "given hyperedge, and hyperedge_size should return their count") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        constexpr auto hyperedge_id = constants::id1;

        SUBCASE("sequential vertices") {
            if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
                std::vector<hgl::default_id_type> expected_vertices{};
                for (const auto vid : sut.vertex_ids()) {
                    sut.bind(vid, hyperedge_id);
                    expected_vertices.push_back(vid);

                    CHECK(std::ranges::equal(
                        sut.incident_vertices(hyperedge_id),
                        expected_vertices,
                        std::equal_to{},
                        get_id
                    ));
                    CHECK_EQ(sut.hyperedge_size(hyperedge_id), expected_vertices.size());
                }
            }

            if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
                std::vector<hgl::default_id_type> expected_vertices;
                std::vector<hgl::default_id_type> expected_head_vertices, expected_tail_vertices;
                for (const auto vid : sut.vertex_ids()) {
                    if (vid % 2 == 0) {
                        sut.bind_head(vid, hyperedge_id);
                        expected_head_vertices.push_back(vid);
                    }
                    else {
                        sut.bind_tail(vid, hyperedge_id);
                        expected_tail_vertices.push_back(vid);
                    }
                    expected_vertices.push_back(vid);

                    CHECK(std::ranges::is_permutation(
                        sut.incident_vertices(hyperedge_id),
                        expected_vertices,
                        std::equal_to{},
                        get_id
                    ));
                    CHECK_EQ(sut.hyperedge_size(hyperedge_id), expected_vertices.size());

                    CHECK(std::ranges::equal(
                        sut.head(hyperedge_id), expected_head_vertices, std::equal_to{}, get_id
                    ));
                    CHECK_EQ(sut.head_size(hyperedge_id), expected_head_vertices.size());

                    CHECK(std::ranges::equal(
                        sut.tail(hyperedge_id), expected_tail_vertices, std::equal_to{}, get_id
                    ));
                    CHECK_EQ(sut.tail_size(hyperedge_id), expected_tail_vertices.size());
                }
            }
        }

        SUBCASE("specific vertices") {
            if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
                sut.bind(constants::id1, hyperedge_id);
                sut.bind(constants::id3, hyperedge_id);

                CHECK(std::ranges::equal(
                    sut.incident_vertices(hyperedge_id),
                    std::vector<hgl::default_id_type>{constants::id1, constants::id3},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.hyperedge_size(hyperedge_id), 2uz);
            }

            if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
                sut.bind_head(constants::id1, hyperedge_id);
                sut.bind_tail(constants::id3, hyperedge_id);

                CHECK(std::ranges::is_permutation(
                    sut.incident_vertices(hyperedge_id),
                    std::vector<hgl::default_id_type>{constants::id1, constants::id3},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.hyperedge_size(hyperedge_id), 2uz);

                CHECK(std::ranges::equal(
                    sut.head(hyperedge_id),
                    std::vector<hgl::default_id_type>{constants::id1},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.head_size(hyperedge_id), 1uz);

                CHECK(std::ranges::equal(
                    sut.tail(hyperedge_id),
                    std::vector<hgl::default_id_type>{constants::id3},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.tail_size(hyperedge_id), 1uz);
            }
        }
    }

    SUBCASE("element size map getters should return maps of properly calculated element sizes") {
        constexpr auto n_elements = 5ull;
        sut_type sut{n_elements, n_elements};

        constexpr auto is_zero = [](const auto& size) { return size == 0uz; };
        REQUIRE(std::ranges::all_of(sut.degree_map(), is_zero));
        REQUIRE(std::ranges::all_of(sut.hyperedge_size_map(), is_zero));

        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            for (auto i = 0u; i < n_elements; i++)
                for (auto j = 0u; j <= i; j++)
                    sut.bind(i, j);

            const auto deg_map = sut.degree_map();
            const auto esize_map = sut.hyperedge_size_map();

            for (auto i = 0uz; i < n_elements; i++) {
                CHECK_EQ(deg_map[i], i + 1uz);
                CHECK_EQ(esize_map[i], n_elements - i);
            }
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            REQUIRE(std::ranges::all_of(sut.out_degree_map(), is_zero));
            REQUIRE(std::ranges::all_of(sut.in_degree_map(), is_zero));
            REQUIRE(std::ranges::all_of(sut.tail_size_map(), is_zero));
            REQUIRE(std::ranges::all_of(sut.head_size_map(), is_zero));

            for (auto i = 0u; i < n_elements; i++) {
                for (auto j = 0u; j <= i; j++) {
                    if (i == j)
                        sut.bind_tail(i, j);
                    else
                        sut.bind_head(i, j);
                }
            }

            const auto deg_map = sut.degree_map();
            const auto out_deg_map = sut.out_degree_map();
            const auto in_deg_map = sut.in_degree_map();
            const auto esize_map = sut.hyperedge_size_map();
            const auto tsize_map = sut.tail_size_map();
            const auto hsize_map = sut.head_size_map();

            for (auto k = 0uz; k < n_elements; k++) {
                CHECK_EQ(deg_map[k], k + 1uz);
                CHECK_EQ(out_deg_map[k], 1uz);
                CHECK_EQ(in_deg_map[k], k);

                CHECK_EQ(esize_map[k], n_elements - k);
                CHECK_EQ(tsize_map[k], 1uz);
                CHECK_EQ(hsize_map[k], n_elements - k - 1uz);
            }
        }
    }

    // --- comparison and cloning ---

    const auto set_properties = [](auto& sut) {
        for (auto vertex : sut.vertices())
            vertex.properties() = std::format("vertex_{}", vertex.id());

        for (auto hyperedge : sut.hyperedges())
            hyperedge.properties() = std::format("hyperedge_{}", hyperedge.id());
    };

    using p_hypergraph_traits = add_properties<HypergraphTraits, hgl::name_property>;
    using p_sut_type = hgl::hypergraph<p_hypergraph_traits>;

    const auto create_test_p_hypergraph = [&set_properties]() {
        p_sut_type sut(2uz, 2uz);
        if constexpr (hgl::traits::c_undirected_hypergraph<p_sut_type>) {
            sut.bind(0uz, 0uz);
            sut.bind(1uz, 1uz);
        }
        else {
            sut.bind_tail(0uz, 0uz);
            sut.bind_head(1uz, 1uz);
        }
        set_properties(sut);
        return sut;
    };

    SUBCASE("equality operator should properly compare hypergraphs") {
        auto sut1 = create_test_p_hypergraph();
        auto sut2 = create_test_p_hypergraph();

        SUBCASE("identical hypergraphs are equal") {
            CHECK_EQ(sut1, sut2);
        }

        SUBCASE("hypergraphs with different orders are not equal") {
            sut2.add_vertex();
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("hypergraphs with different sizes are not equal") {
            sut2.add_hyperedge();
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("hypergraphs with different bindings are not equal") {
            if constexpr (hgl::traits::c_undirected_hypergraph<p_sut_type>)
                sut2.bind(0uz, 1uz);
            else
                sut2.bind_tail(0uz, 1uz);
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("hypergraphs with different vertex properties are not equal") {
            sut2.vertex_properties(0uz) = "dummy";
            CHECK_NE(sut1, sut2);
        }

        SUBCASE("hypergraphs with different hyperedge properties are not equal") {
            sut2.hyperedge_properties(0uz) = "dummy";
            CHECK_NE(sut1, sut2);
        }
    }

    SUBCASE("clone should return an exact copy of the source hypergraph") {
        const auto sut1 = create_test_p_hypergraph();
        const auto sut2 = hgl::clone(sut1);
        CHECK_EQ(sut1, sut2);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::undirected_t>, // undirected bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incidence list
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
        hgl::undirected_t>, // undirected hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major flat incidence matrix
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t>, // bf-directed bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t>, // bf-directed bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t> // bf-directed vertex-major flat incidence matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "properties getter tests for undirected hypergraphs",
    HypergraphTraits,
    undirected_property_hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using vertex_properties_type = typename sut_type::vertex_properties_type;
    using hyperedge_properties_type = typename sut_type::hyperedge_properties_type;

    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    for (auto vertex : sut.vertices())
        vertex.properties() = std::format("vertex_{}", vertex.id());
    for (auto hyperedge : sut.hyperedges())
        hyperedge.properties() = std::format("hyperedge_{}", hyperedge.id());

    SUBCASE("vertex property getters should return proper property objects/view") {
        auto vmap = sut.vertex_properties_map();
        CHECK(vmap.size() == constants::n_vertices);
        for (auto [id, property] : vw::zip(sut.vertex_ids(), vmap)) {
            CHECK_EQ(property, std::format("vertex_{}", id));
            CHECK_EQ(vmap[id], std::format("vertex_{}", id));
            CHECK_EQ(sut.vertex_properties(id), std::format("vertex_{}", id));
        }
        CHECK_THROWS_AS(
            discard(sut.vertex_properties(constants::out_of_rng_vid)), std::invalid_argument
        );
    }

    SUBCASE("hyperedge property getters should return proper property objects/view") {
        auto emap = sut.hyperedge_properties_map();
        CHECK(emap.size() == constants::n_hyperedges);
        for (auto [id, property] : vw::zip(sut.hyperedge_ids(), emap)) {
            CHECK_EQ(property, std::format("hyperedge_{}", id));
            CHECK_EQ(emap[id], std::format("hyperedge_{}", id));
            CHECK_EQ(sut.hyperedge_properties(id), std::format("hyperedge_{}", id));
        }
        CHECK_THROWS_AS(
            discard(sut.hyperedge_properties(constants::out_of_rng_eid)), std::invalid_argument
        );
    }

    constexpr auto get_property_addr = [](const auto& descriptor) {
        return &descriptor.properties();
    };

    SUBCASE("incident_hyperedges should return a view of hyperedge_descriptor objects containing "
            "correct property references") {
        constexpr auto vertex_id = constants::id1;

        SUBCASE("sequential hyperedges") {
            std::vector<hyperedge_properties_type*> expected_properties{};
            for (const auto hyperedge_id : sut.hyperedge_ids()) {
                sut.bind(vertex_id, hyperedge_id);
                expected_properties.push_back(&sut.hyperedge_properties(hyperedge_id));

                CHECK(std::ranges::equal(
                    sut.incident_hyperedges(vertex_id),
                    expected_properties,
                    std::equal_to{},
                    get_property_addr
                ));
            }
        }

        SUBCASE("specific hyperedges") {
            sut.bind(vertex_id, constants::id2);
            sut.bind(vertex_id, constants::id4);

            CHECK(std::ranges::equal(
                sut.incident_hyperedges(vertex_id),
                std::vector<hyperedge_properties_type*>{
                    &sut.hyperedge_properties(constants::id2),
                    &sut.hyperedge_properties(constants::id4)
                },
                std::equal_to{},
                get_property_addr
            ));
        }
    }

    SUBCASE("incident_vertices should return a view of vertex_descriptor objects containing "
            "correct property references") {
        constexpr auto hyperedge_id = constants::id1;

        SUBCASE("sequential vertices") {
            std::vector<vertex_properties_type*> expected_properties{};
            for (const auto vertex_id : sut.vertex_ids()) {
                sut.bind(vertex_id, hyperedge_id);
                expected_properties.push_back(&sut.vertex_properties(vertex_id));

                CHECK(std::ranges::equal(
                    sut.incident_vertices(hyperedge_id),
                    expected_properties,
                    std::equal_to{},
                    get_property_addr
                ));
            }
        }

        SUBCASE("specific vertices") {
            sut.bind(constants::id1, hyperedge_id);
            sut.bind(constants::id3, hyperedge_id);

            CHECK(std::ranges::equal(
                sut.incident_vertices(hyperedge_id),
                std::vector<vertex_properties_type*>{
                    &sut.vertex_properties(constants::id1), &sut.vertex_properties(constants::id3)
                },
                std::equal_to{},
                get_property_addr
            ));
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    undirected_property_hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property>, // undirected hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::name_property,
        hgl::name_property> // undirected vertex-major flat incidence matrix
);

TEST_CASE_TEMPLATE_DEFINE(
    "properties getter tests for bf-directed hypergraphs",
    HypergraphTraits,
    bf_directed_property_hypergraph_traits_template
) {
    using sut_type = hgl::hypergraph<HypergraphTraits>;
    using vertex_properties_type = typename sut_type::vertex_properties_type;
    using hyperedge_properties_type = typename sut_type::hyperedge_properties_type;

    sut_type sut{constants::n_vertices, constants::n_hyperedges};
    for (auto vertex : sut.vertices())
        vertex.properties() = std::format("vertex_{}", vertex.id());
    for (auto hyperedge : sut.hyperedges())
        hyperedge.properties() = std::format("hyperedge_{}", hyperedge.id());

    SUBCASE("vertex property getters should return proper property objects/view") {
        auto vmap = sut.vertex_properties_map();
        CHECK(vmap.size() == constants::n_vertices);
        for (auto [id, property] : vw::zip(sut.vertex_ids(), vmap)) {
            CHECK_EQ(property, std::format("vertex_{}", id));
            CHECK_EQ(vmap[id], std::format("vertex_{}", id));
            CHECK_EQ(sut.vertex_properties(id), std::format("vertex_{}", id));
        }
        CHECK_THROWS_AS(
            discard(sut.vertex_properties(constants::out_of_rng_vid)), std::invalid_argument
        );
    }

    SUBCASE("hyperedge property getters should return proper property objects/view") {
        auto emap = sut.hyperedge_properties_map();
        CHECK(emap.size() == constants::n_hyperedges);
        for (auto [id, property] : vw::zip(sut.hyperedge_ids(), emap)) {
            CHECK_EQ(property, std::format("hyperedge_{}", id));
            CHECK_EQ(emap[id], std::format("hyperedge_{}", id));
            CHECK_EQ(sut.hyperedge_properties(id), std::format("hyperedge_{}", id));
        }
        CHECK_THROWS_AS(
            discard(sut.hyperedge_properties(constants::out_of_rng_eid)), std::invalid_argument
        );
    }

    constexpr auto get_property_addr = [](const auto& descriptor) {
        return &descriptor.properties();
    };

    SUBCASE("incident_hyperedges should return a view of hyperedge_descriptor objects containing "
            "correct property references") {
        constexpr auto vertex_id = constants::id1;

        SUBCASE("sequential hyperedges") {
            std::vector<hyperedge_properties_type*> expected_properties;
            std::vector<hyperedge_properties_type*> expected_in_properties, expected_out_properties;
            for (const auto eid : sut.hyperedge_ids()) {
                if (eid % 2 == 0) {
                    sut.bind_head(vertex_id, eid);
                    expected_in_properties.push_back(&sut.hyperedge_properties(eid));
                }
                else {
                    sut.bind_tail(vertex_id, eid);
                    expected_out_properties.push_back(&sut.hyperedge_properties(eid));
                }
                expected_properties.push_back(&sut.hyperedge_properties(eid));

                CHECK(std::ranges::is_permutation(
                    sut.incident_hyperedges(vertex_id),
                    expected_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.in_hyperedges(vertex_id),
                    expected_in_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.out_hyperedges(vertex_id),
                    expected_out_properties,
                    std::equal_to{},
                    get_property_addr
                ));
            }
        }

        SUBCASE("specific hyperedges") {
            sut.bind_head(vertex_id, constants::id2);
            sut.bind_tail(vertex_id, constants::id4);

            CHECK(std::ranges::is_permutation(
                sut.incident_hyperedges(vertex_id),
                std::vector<hyperedge_properties_type*>{
                    &sut.hyperedge_properties(constants::id2),
                    &sut.hyperedge_properties(constants::id4)
                },
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.in_hyperedges(vertex_id),
                std::vector<hyperedge_properties_type*>{&sut.hyperedge_properties(constants::id2)},
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.out_hyperedges(vertex_id),
                std::vector<hyperedge_properties_type*>{&sut.hyperedge_properties(constants::id4)},
                std::equal_to{},
                get_property_addr
            ));
        }
    }

    SUBCASE("incident_vertices should return a view of vertex_descriptor objects containing "
            "correct property references") {
        constexpr auto hyperedge_id = constants::id1;

        SUBCASE("sequential vertices") {
            std::vector<vertex_properties_type*> expected_properties;
            std::vector<vertex_properties_type*> expected_head_properties, expected_tail_properties;
            for (const auto vid : sut.vertex_ids()) {
                if (vid % 2 == 0) {
                    sut.bind_head(vid, hyperedge_id);
                    expected_head_properties.push_back(&sut.vertex_properties(vid));
                }
                else {
                    sut.bind_tail(vid, hyperedge_id);
                    expected_tail_properties.push_back(&sut.vertex_properties(vid));
                }
                expected_properties.push_back(&sut.vertex_properties(vid));

                CHECK(std::ranges::is_permutation(
                    sut.incident_vertices(hyperedge_id),
                    expected_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.head(hyperedge_id),
                    expected_head_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.tail(hyperedge_id),
                    expected_tail_properties,
                    std::equal_to{},
                    get_property_addr
                ));
            }
        }

        SUBCASE("specific vertices") {
            sut.bind_head(constants::id1, hyperedge_id);
            sut.bind_tail(constants::id3, hyperedge_id);

            CHECK(std::ranges::is_permutation(
                sut.incident_vertices(hyperedge_id),
                std::vector<vertex_properties_type*>{
                    &sut.vertex_properties(constants::id1), &sut.vertex_properties(constants::id3)
                },
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.head(hyperedge_id),
                std::vector<vertex_properties_type*>{&sut.vertex_properties(constants::id1)},
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.tail(hyperedge_id),
                std::vector<vertex_properties_type*>{&sut.vertex_properties(constants::id3)},
                std::equal_to{},
                get_property_addr
            ));
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bf_directed_property_hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed bidirectional incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed vertex-major incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::bidirectional_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed bidirectional flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed hyperedge-major flat incidence list
    hgl::flat_list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed vertex-major flat incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed vertex-major incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property>, // bf-directed hyperedge-major flat incidence matrix
    hgl::flat_matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::name_property,
        hgl::name_property> // bf-directed vertex-major flat incidence matrix
);

TEST_SUITE_END(); // test_hypergraph

} // namespace hgl_testing
