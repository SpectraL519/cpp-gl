#include "doctest.h"
#include "testing/hgl/io.hpp"

#include <hgl/hypergraph.hpp>
#include <hgl/io.hpp>

#include <sstream>
#include <type_traits>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph_io");

template <typename SutType>
struct test_hypergraph_io_fixture {
    using sut_type = SutType;
    using directional_tag = typename sut_type::directional_tag;

    test_hypergraph_io_fixture() {
        ss << hgl::io::spec_fmt;

        sut_out.add_vertices(n_vertices);

        std::size_t v_idx = 0;
        for (const auto& vertex : sut_out.vertices())
            vertex.properties() = std::format("vertex_{}", v_idx++);

        auto add_edge = [&](const std::string& name) {
            return sut_out.add_hyperedge_with(hgl::name_property{name}).id();
        };

        // Construct graph based on directional tag
        if constexpr (std::same_as<directional_tag, hgl::undirected_t>) {
            auto he1 = add_edge("edge_0");
            sut_out.bind(0, he1);
            sut_out.bind(1, he1);
            sut_out.bind(2, he1);

            auto he2 = add_edge("edge_1");
            sut_out.bind(1, he2);
            sut_out.bind(2, he2);
            sut_out.bind(3, he2);

            auto he3 = add_edge("edge_2");
            sut_out.bind(0, he3);
            sut_out.bind(3, he3);
        }
        else if constexpr (std::same_as<directional_tag, hgl::bf_directed_t>) {
            auto he1 = add_edge("edge_0");
            sut_out.bind_tail(0, he1);
            sut_out.bind_tail(1, he1);
            sut_out.bind_head(2, he1);

            auto he2 = add_edge("edge_1");
            sut_out.bind_tail(2, he2);
            sut_out.bind_head(3, he2);
            sut_out.bind_head(4, he2);

            auto he3 = add_edge("edge_2");
            sut_out.bind_tail(1, he3);
            sut_out.bind_head(4, he3);
        }
    }

    const hgl::size_type n_vertices = 5uz;
    sut_type sut_out;
    sut_type sut_in;

    std::stringstream ss;
};

template <hgl::traits::c_hypergraph_directional_tag Dir>
struct wrong_dir_tag {};

template <>
struct wrong_dir_tag<hgl::undirected_t> {
    using type = hgl::bf_directed_t;
};

template <>
struct wrong_dir_tag<hgl::bf_directed_t> {
    using type = hgl::undirected_t;
};

TEST_CASE_TEMPLATE_DEFINE("hypergraph io tests", SutType, hypergraph_io_template) {
    using fixture_type = test_hypergraph_io_fixture<SutType>;
    using sut_type = typename fixture_type::sut_type;
    using dir_tag = typename sut_type::directional_tag;

    fixture_type fixture;

    SUBCASE("io read should throw if the directional tag doesn't match") {
        // Resolve the opposite directional tag
        using wrong_tag = typename wrong_dir_tag<dir_tag>::type;
        using wrong_traits = hgl::hypergraph_traits<wrong_tag>;
        hgl::hypergraph<wrong_traits> invalid_sut_out{fixture.n_vertices};

        std::stringstream err_ss;
        err_ss << hgl::io::spec_fmt << invalid_sut_out;

        CHECK_THROWS_AS(err_ss >> fixture.sut_in, std::ios_base::failure);
    }

    SUBCASE("io read should throw for a stream with vertex properties if vertex_properties_type is "
            "not readable") {
        fixture.ss << hgl::io::with_vertex_properties << fixture.sut_out;

        using not_readable_vp_traits =
            hgl::hypergraph_traits<dir_tag, std::monostate, hgl::name_property>;
        hgl::hypergraph<not_readable_vp_traits> invalid_sut_in;

        CHECK_THROWS_AS(fixture.ss >> invalid_sut_in, std::ios_base::failure);
    }

    SUBCASE("io read should throw for a stream with hyperedge properties if "
            "hyperedge_properties_type is not readable") {
        fixture.ss << hgl::io::with_hyperedge_properties << fixture.sut_out;

        using not_readable_hp_traits =
            hgl::hypergraph_traits<dir_tag, hgl::name_property, std::monostate>;
        hgl::hypergraph<not_readable_hp_traits> invalid_sut_in;

        CHECK_THROWS_AS(fixture.ss >> invalid_sut_in, std::ios_base::failure);
    }

    SUBCASE("io operators should properly write and read the hypergraph from a stream with no "
            "additional options enabled") {
        fixture.ss << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_hypergraph_structure(fixture.sut_in, fixture.sut_out);
    }

    SUBCASE("io operators should properly write and read the hypergraph from a stream with vertex "
            "properties") {
        fixture.ss << hgl::io::with_vertex_properties << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_hypergraph_structure(fixture.sut_in, fixture.sut_out);
        verify_vertex_properties(fixture.sut_in, fixture.sut_out);
    }

    SUBCASE("io operators should properly write and read the hypergraph from a stream with "
            "hyperedge properties") {
        fixture.ss << hgl::io::with_hyperedge_properties << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_hypergraph_structure(fixture.sut_in, fixture.sut_out);
        verify_hyperedge_properties(fixture.sut_in, fixture.sut_out);
    }

    SUBCASE("io operators should properly write and read the hypergraph from a stream with both "
            "vertex and hyperedge properties") {
        fixture.ss << hgl::io::with_properties << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_hypergraph_structure(fixture.sut_in, fixture.sut_out);
        verify_vertex_properties(fixture.sut_in, fixture.sut_out);
        verify_hyperedge_properties(fixture.sut_in, fixture.sut_out);
    }
}

// clang-format off

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_io_template,
    hgl::hypergraph<hgl::list_hypergraph_traits<hgl::repr::bidirectional_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::list_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::list_hypergraph_traits<hgl::repr::vertex_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_list_hypergraph_traits<hgl::repr::bidirectional_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_list_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_list_hypergraph_traits<hgl::repr::vertex_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::matrix_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::matrix_hypergraph_traits<hgl::repr::vertex_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_matrix_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_matrix_hypergraph_traits<hgl::repr::vertex_major_t, hgl::undirected_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::list_hypergraph_traits<hgl::repr::bidirectional_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::list_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::list_hypergraph_traits<hgl::repr::vertex_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_list_hypergraph_traits<hgl::repr::bidirectional_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_list_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_list_hypergraph_traits<hgl::repr::vertex_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::matrix_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::matrix_hypergraph_traits<hgl::repr::vertex_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_matrix_hypergraph_traits<hgl::repr::hyperedge_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>,
    hgl::hypergraph<hgl::flat_matrix_hypergraph_traits<hgl::repr::vertex_major_t, hgl::bf_directed_t, hgl::name_property, hgl::name_property>>
);

// clang-format on

TEST_SUITE_END(); // test_hypergraph_io

} // namespace hgl_testing
