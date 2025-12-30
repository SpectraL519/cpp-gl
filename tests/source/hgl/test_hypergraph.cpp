#include "hgl/directional_tags.hpp"
#include "testing/hgl/constants.hpp"
#include "testing/hgl/types.hpp"

#include <doctest.h>
#include <hgl/hypergraph.hpp>

#include <algorithm>
#include <concepts>

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
    using directional_tag = typename sut_type::directional_tag;
    using vertex_type = typename sut_type::vertex_type;
    using hyperedge_type = typename sut_type::hyperedge_type;

    // --- general tests ---

    SUBCASE("a hypergraph should be initialized with no vertices and no hyperedges by default") {
        sut_type sut{};
        CHECK_EQ(sut.order(), 0uz);
        CHECK_EQ(sut.size(), 0uz);
    }

    SUBCASE("a hypergraph constructed with n_vertices parameter should contain n_vertices vertices "
            "and no hyperedges") {
        sut_type sut{constants::n_vertices};

        REQUIRE_EQ(sut.order(), constants::n_vertices);
        REQUIRE_EQ(sut.size(), 0uz);

        REQUIRE(rng::equal(sut.vertices() | vw::transform(get_id), constants::vertex_ids_view));
        REQUIRE(rng::equal(sut.vertex_ids(), constants::vertex_ids_view));

        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::out_of_rng_vid)), std::out_of_range
        );
    }

    SUBCASE("a hypergraph constructed with n_vertices and n_hyperedges parameters should contain "
            "n_vertices vertices and n_hyperedges hyperedges") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};

        REQUIRE_EQ(sut.order(), constants::n_vertices);
        REQUIRE_EQ(sut.size(), constants::n_hyperedges);

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
            CHECK_EQ(sut.order(), v_id + 1uz);
            CHECK_EQ(sut.degree(v_id), 0uz);
        }

        CHECK_EQ(sut.order(), constants::n_vertices);
    }

    SUBCASE("add_vertex_with should initialize a new vertex with the input properties structure") {
        using properties_traits_type =
            add_vertex_property<HypergraphTraits, types::boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const auto vertex = sut.add_vertex_with(constants::p_true);
        REQUIRE_EQ(sut.order(), 1uz);

        CHECK_EQ(vertex.id(), constants::id1);
        CHECK_EQ(vertex.properties(), constants::p_true);
        CHECK_EQ(sut.degree(vertex), 0uz);
    }

    SUBCASE("add_vertices(n) should add n new vertices to the hypergraph") {
        sut_type sut{};
        sut.add_vertices(constants::n_vertices);

        CHECK_EQ(sut.order(), constants::n_vertices);
        CHECK_EQ(sut.size(), 0uz);
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

        REQUIRE_EQ(sut.order(), expected_n_vertices);
        CHECK_EQ(sut.size(), 0uz);

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

    SUBCASE("remove_vertex(vertex) should do nothing if the given vertex is invalid") {
        sut_type sut{constants::n_vertices};
        REQUIRE_EQ(sut.order(), constants::n_vertices);

        CHECK_NOTHROW(sut.remove_vertex(vertex_type{constants::out_of_rng_vid}));
        CHECK_EQ(sut.order(), constants::n_vertices);
    }

    SUBCASE("remove_vertex(vertex) should remove the given vertex and align ids of remaining "
            "vertices") {
        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        REQUIRE_EQ(sut.order(), constants::n_vertices - 1uz);
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex(constants::n_vertices - 1uz)), std::out_of_range
        );
    }

    SUBCASE("remove_vertex(id) should do nothing if the given id is invalid") {
        sut_type sut{constants::n_vertices};
        REQUIRE_EQ(sut.order(), constants::n_vertices);

        CHECK_NOTHROW(sut.remove_vertex(constants::out_of_rng_vid));
        CHECK_EQ(sut.order(), constants::n_vertices);
    }

    SUBCASE("remove_vertex(id) should remove the given vertex and align ids of remaining vertices"
    ) {
        sut_type sut{constants::n_vertices};
        sut.remove_vertex(constants::id1);

        REQUIRE_EQ(sut.order(), constants::n_vertices - 1uz);
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
        REQUIRE_EQ(sut.order(), expected_n_vertices);
    }

    SUBCASE("remove_vertices_from(vertices) should properly remove elements at given indices "
            "(ignoring duplicate vertices)") {
        constexpr auto n_vertices = constants::n_vertices + 1uz;

        sut_type sut{n_vertices};
        const auto v1 = sut.get_vertex(constants::id1);
        const auto v3 = sut.get_vertex(constants::id3);
        sut.remove_vertices_from(std::vector<vertex_type>{v1, v3, v1});

        constexpr auto expected_n_vertices = n_vertices - 2uz;
        REQUIRE_EQ(sut.order(), expected_n_vertices);
    }

    // --- hyperedge method tests ---

    SUBCASE("add_hyperedge should return a hyperedge_descriptor with an incremented id") {
        sut_type sut;

        for (gl::types::id_type e_id = 0uz; e_id < constants::n_hyperedges; e_id++) {
            const auto hyperedge = sut.add_hyperedge();
            CHECK_EQ(hyperedge.id(), e_id);
            CHECK_EQ(sut.size(), e_id + 1uz);
        }

        CHECK_EQ(sut.size(), constants::n_hyperedges);
    }

    SUBCASE("add_hyperedge_with should initialize a new hyperedge with the input properties "
            "structure") {
        using properties_traits_type =
            add_hyperedge_property<HypergraphTraits, types::boolean_property>;
        hgl::hypergraph<properties_traits_type> sut;

        const auto hyperedge = sut.add_hyperedge_with(constants::p_true);
        REQUIRE_EQ(sut.size(), 1uz);

        CHECK_EQ(hyperedge.id(), constants::id1);
        CHECK_EQ(hyperedge.properties(), constants::p_true);
    }

    SUBCASE("add_hyperedges(n) should add n new hyperedges to the hypergraph") {
        sut_type sut{};
        sut.add_hyperedges(constants::n_hyperedges);

        CHECK_EQ(sut.order(), 0uz);
        CHECK_EQ(sut.size(), constants::n_hyperedges);
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

        REQUIRE_EQ(sut.order(), 0uz);
        CHECK_EQ(sut.size(), expected_n_hyperedges);

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

    SUBCASE("remove_hyperedge(hyperedge) should do nothing if the given hyperedge is invalid") {
        sut_type sut{0uz, constants::n_hyperedges};
        REQUIRE_EQ(sut.size(), constants::n_hyperedges);

        CHECK_NOTHROW(sut.remove_hyperedge(hyperedge_type{constants::out_of_rng_eid}));
        CHECK_EQ(sut.size(), constants::n_hyperedges);
    }

    SUBCASE("remove_hyperedge(hyperedge) should remove the given hyperedge and align ids of "
            "remaining hyperedges") {
        sut_type sut{0uz, constants::n_hyperedges};
        sut.remove_hyperedge(constants::id1);

        REQUIRE_EQ(sut.size(), constants::n_hyperedges - 1uz);
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_hyperedge(constants::n_hyperedges - 1uz)), std::out_of_range
        );
    }

    SUBCASE("remove_hyperedge(id) should do nothing if the given id is invalid") {
        sut_type sut{0uz, constants::n_hyperedges};
        REQUIRE_EQ(sut.size(), constants::n_hyperedges);

        CHECK_NOTHROW(sut.remove_hyperedge(constants::out_of_rng_eid));
        CHECK_EQ(sut.size(), constants::n_hyperedges);
    }

    SUBCASE("remove_hyperedge(id) should remove the given hyperedge and align ids of remaining "
            "hyperedges") {
        sut_type sut{0uz, constants::n_hyperedges};
        sut.remove_hyperedge(constants::id1);

        REQUIRE_EQ(sut.size(), constants::n_hyperedges - 1uz);
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
        REQUIRE_EQ(sut.size(), expected_n_hyperedges);
    }

    SUBCASE("remove_hyperedges_from(hyperedges) should properly remove elements at given indices "
            "(ignoring duplicate hyperedges)") {
        constexpr auto n_hyperedges = constants::n_hyperedges + 1uz;

        sut_type sut{0uz, n_hyperedges};
        const auto he1 = sut.get_hyperedge(constants::id1);
        const auto he3 = sut.get_hyperedge(constants::id3);
        sut.remove_hyperedges_from(std::vector<hyperedge_type>{he1, he3, he1});

        constexpr auto expected_n_hyperedges = n_hyperedges - 2uz;
        REQUIRE_EQ(sut.size(), expected_n_hyperedges);
    }

    // --- incidence method tests ---

    SUBCASE("bind, unbind and are_incident should throw if either of the fiven elements is invalid"
    ) {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};

        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            CHECK_THROWS_AS(
                sut.bind(constants::out_of_rng_vid, constants::out_of_rng_eid), std::out_of_range
            );
            CHECK_THROWS_AS(sut.bind(constants::id1, constants::out_of_rng_eid), std::out_of_range);
            CHECK_THROWS_AS(sut.bind(constants::out_of_rng_vid, constants::id1), std::out_of_range);
        }

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            CHECK_THROWS_AS(
                sut.bind_tail(constants::out_of_rng_vid, constants::out_of_rng_eid),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.bind_tail(constants::id1, constants::out_of_rng_eid), std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.bind_tail(constants::out_of_rng_vid, constants::id1), std::out_of_range
            );

            CHECK_THROWS_AS(
                sut.bind_head(constants::out_of_rng_vid, constants::out_of_rng_eid),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.bind_head(constants::id1, constants::out_of_rng_eid), std::out_of_range
            );
            CHECK_THROWS_AS(
                sut.bind_head(constants::out_of_rng_vid, constants::id1), std::out_of_range
            );
        }

        CHECK_THROWS_AS(
            sut.unbind(constants::out_of_rng_vid, constants::out_of_rng_eid), std::out_of_range
        );
        CHECK_THROWS_AS(sut.unbind(constants::id1, constants::out_of_rng_eid), std::out_of_range);
        CHECK_THROWS_AS(sut.unbind(constants::out_of_rng_vid, constants::id1), std::out_of_range);

        CHECK_THROWS_AS(
            static_cast<void>(sut.are_incident(constants::out_of_rng_vid, constants::out_of_rng_eid)
            ),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            static_cast<void>(sut.are_incident(constants::id1, constants::out_of_rng_eid)),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            static_cast<void>(sut.are_incident(constants::out_of_rng_vid, constants::id1)),
            std::out_of_range
        );

        if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            CHECK_THROWS_AS(
                static_cast<void>(sut.is_tail(constants::out_of_rng_vid, constants::out_of_rng_eid)
                ),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                static_cast<void>(sut.is_tail(constants::id1, constants::out_of_rng_eid)),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                static_cast<void>(sut.is_tail(constants::out_of_rng_vid, constants::id1)),
                std::out_of_range
            );

            CHECK_THROWS_AS(
                static_cast<void>(sut.is_head(constants::out_of_rng_vid, constants::out_of_rng_eid)
                ),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                static_cast<void>(sut.is_head(constants::id1, constants::out_of_rng_eid)),
                std::out_of_range
            );
            CHECK_THROWS_AS(
                static_cast<void>(sut.is_head(constants::out_of_rng_vid, constants::id1)),
                std::out_of_range
            );
        }
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

    SUBCASE("incident_hyperedges and degree should throw if the given vertex (id) is invalid") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK_THROWS_AS(
            static_cast<void>(sut.incident_hyperedges(vertex_type{constants::out_of_rng_vid})),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            static_cast<void>(sut.degree(vertex_type{constants::out_of_rng_vid})), std::out_of_range
        );
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
                std::vector<hgl::types::id_type> expected_hyperedges{};
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
                std::vector<hgl::types::id_type> expected_hyperedges;
                std::vector<hgl::types::id_type> expected_in_hyperedges, expected_out_hyperedges;
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
                        sut.incoming_hyperedges(vertex_id),
                        expected_in_hyperedges,
                        std::equal_to{},
                        get_id
                    ));
                    CHECK_EQ(sut.in_degree(vertex_id), expected_in_hyperedges.size());

                    CHECK(std::ranges::equal(
                        sut.outgoing_hyperedges(vertex_id),
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
                    std::vector<hgl::types::id_type>{constants::id2, constants::id4},
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
                    std::vector<hgl::types::id_type>{constants::id2, constants::id4},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.degree(vertex_id), 2uz);

                CHECK(std::ranges::equal(
                    sut.incoming_hyperedges(vertex_id),
                    std::vector<hgl::types::id_type>{constants::id2},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.in_degree(vertex_id), 1uz);

                CHECK(std::ranges::equal(
                    sut.outgoing_hyperedges(vertex_id),
                    std::vector<hgl::types::id_type>{constants::id4},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.out_degree(vertex_id), 1uz);
            }
        }
    }

    SUBCASE("incident_vertices and hyperedge_size should throw if the given hyperedge (id) is "
            "invalid") {
        sut_type sut{constants::n_vertices, constants::n_hyperedges};
        CHECK_THROWS_AS(
            static_cast<void>(sut.incident_vertices(hyperedge_type{constants::out_of_rng_eid})),
            std::out_of_range
        );
        CHECK_THROWS_AS(
            static_cast<void>(sut.hyperedge_size(hyperedge_type{constants::out_of_rng_eid})),
            std::out_of_range
        );
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
                std::vector<hgl::types::id_type> expected_vertices{};
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
                std::vector<hgl::types::id_type> expected_vertices;
                std::vector<hgl::types::id_type> expected_head_vertices, expected_tail_vertices;
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
                        sut.head_vertices(hyperedge_id),
                        expected_head_vertices,
                        std::equal_to{},
                        get_id
                    ));
                    CHECK_EQ(sut.head_size(hyperedge_id), expected_head_vertices.size());

                    CHECK(std::ranges::equal(
                        sut.tail_vertices(hyperedge_id),
                        expected_tail_vertices,
                        std::equal_to{},
                        get_id
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
                    std::vector<hgl::types::id_type>{constants::id1, constants::id3},
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
                    std::vector<hgl::types::id_type>{constants::id1, constants::id3},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.hyperedge_size(hyperedge_id), 2uz);

                CHECK(std::ranges::equal(
                    sut.head_vertices(hyperedge_id),
                    std::vector<hgl::types::id_type>{constants::id1},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.head_size(hyperedge_id), 1uz);

                CHECK(std::ranges::equal(
                    sut.tail_vertices(hyperedge_id),
                    std::vector<hgl::types::id_type>{constants::id3},
                    std::equal_to{},
                    get_id
                ));
                CHECK_EQ(sut.tail_size(hyperedge_id), 1uz);
            }
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t>, // undirected hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t>, // undirected vertex-major incidence matrix
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t>, // bf-directed vertex-major incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t>, // bf-directed hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t> // bf-directed vertex-major incidence matrix
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
            CHECK_EQ(sut.get_vertex_properties(id), std::format("vertex_{}", id));
        }
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex_properties(constants::out_of_rng_vid)),
            std::out_of_range
        );
    }

    SUBCASE("hyperedge property getters should return proper property objects/view") {
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
                expected_properties.push_back(&sut.get_hyperedge_properties(hyperedge_id));

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
                    &sut.get_hyperedge_properties(constants::id2),
                    &sut.get_hyperedge_properties(constants::id4)
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
                expected_properties.push_back(&sut.get_vertex_properties(vertex_id));

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
                    &sut.get_vertex_properties(constants::id1),
                    &sut.get_vertex_properties(constants::id3)
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
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected vertex-major incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property>, // undirected hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::undirected_t,
        hgl::types::name_property,
        hgl::types::name_property> // undirected vertex-major incidence matrix
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
            CHECK_EQ(sut.get_vertex_properties(id), std::format("vertex_{}", id));
        }
        CHECK_THROWS_AS(
            static_cast<void>(sut.get_vertex_properties(constants::out_of_rng_vid)),
            std::out_of_range
        );
    }

    SUBCASE("hyperedge property getters should return proper property objects/view") {
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
                    expected_in_properties.push_back(&sut.get_hyperedge_properties(eid));
                }
                else {
                    sut.bind_tail(vertex_id, eid);
                    expected_out_properties.push_back(&sut.get_hyperedge_properties(eid));
                }
                expected_properties.push_back(&sut.get_hyperedge_properties(eid));

                CHECK(std::ranges::is_permutation(
                    sut.incident_hyperedges(vertex_id),
                    expected_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.incoming_hyperedges(vertex_id),
                    expected_in_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.outgoing_hyperedges(vertex_id),
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
                    &sut.get_hyperedge_properties(constants::id2),
                    &sut.get_hyperedge_properties(constants::id4)
                },
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.incoming_hyperedges(vertex_id),
                std::vector<hyperedge_properties_type*>{&sut.get_hyperedge_properties(constants::id2
                )},
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.outgoing_hyperedges(vertex_id),
                std::vector<hyperedge_properties_type*>{&sut.get_hyperedge_properties(constants::id4
                )},
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
                    expected_head_properties.push_back(&sut.get_vertex_properties(vid));
                }
                else {
                    sut.bind_tail(vid, hyperedge_id);
                    expected_tail_properties.push_back(&sut.get_vertex_properties(vid));
                }
                expected_properties.push_back(&sut.get_vertex_properties(vid));

                CHECK(std::ranges::is_permutation(
                    sut.incident_vertices(hyperedge_id),
                    expected_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.head_vertices(hyperedge_id),
                    expected_head_properties,
                    std::equal_to{},
                    get_property_addr
                ));

                CHECK(std::ranges::equal(
                    sut.tail_vertices(hyperedge_id),
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
                    &sut.get_vertex_properties(constants::id1),
                    &sut.get_vertex_properties(constants::id3)
                },
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.head_vertices(hyperedge_id),
                std::vector<vertex_properties_type*>{&sut.get_vertex_properties(constants::id1)},
                std::equal_to{},
                get_property_addr
            ));

            CHECK(std::ranges::equal(
                sut.tail_vertices(hyperedge_id),
                std::vector<vertex_properties_type*>{&sut.get_vertex_properties(constants::id3)},
                std::equal_to{},
                get_property_addr
            ));
        }
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    bf_directed_property_hypergraph_traits_template,
    hgl::list_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed hyperedge-major incidence list
    hgl::list_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed vertex-major incidence list
    hgl::matrix_hypergraph_traits<
        hgl::impl::hyperedge_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property>, // bf-directed hyperedge-major incidence matrix
    hgl::matrix_hypergraph_traits<
        hgl::impl::vertex_major_t,
        hgl::bf_directed_t,
        hgl::types::name_property,
        hgl::types::name_property> // bf-directed vertex-major incidence matrix
);

TEST_SUITE_END(); // test_hypergraph

} // namespace hgl_testing
