#include "doctest.h"
#include "testing/gl/io.hpp"

#include <gl/graph.hpp>
#include <gl/io.hpp>
#include <gl/io/options.hpp>
#include <gl/io/options_manip.hpp>
#include <gl/topology.hpp>

#include <sstream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_io");

template <typename SutType>
struct test_graph_io_fixture {
    using sut_type = SutType;
    using directional_tag = typename sut_type::directional_tag;

    test_graph_io_fixture() {
        ss << gl::io::spec_fmt;

        sut_out = gl::topology::clique<sut_type>(n_vertices);

        // prepare vertex and edge properties
        std::size_t v_idx = 0, e_idx = 0;
        for (const auto& vertex : sut_out.vertices())
            vertex.properties() = std::format("vertex_{}", v_idx++);

        for (const auto& vertex : sut_out.vertices()) {
            for (const auto& edge : sut_out.out_edges(vertex)) {
                if constexpr (std::same_as<directional_tag, gl::undirected_t>) {
                    if (edge.source() == vertex.id())
                        edge.properties() = std::format("edge_{}", e_idx++);
                }
                else {
                    edge.properties() = std::format("edge_{}", e_idx++);
                }
            }
        }
    }

    const gl::size_type n_vertices = 5ull;
    sut_type sut_out;
    sut_type sut_in;

    std::stringstream ss;
};

template <typename Dir>
struct wrong_dir_tag {};

template <>
struct wrong_dir_tag<gl::undirected_t> {
    using type = gl::directed_t;
};

template <>
struct wrong_dir_tag<gl::directed_t> {
    using type = gl::undirected_t;
};

TEST_CASE_TEMPLATE_DEFINE("graph io tests", SutType, graph_io_template) {
    using fixture_type = test_graph_io_fixture<SutType>;
    using sut_type = typename fixture_type::sut_type;
    using dir_tag = typename sut_type::directional_tag;

    fixture_type fixture;

    SUBCASE("io read should throw if the directional tag doesn't match") {
        using wrong_tag = typename wrong_dir_tag<dir_tag>::type;
        using wrong_traits = gl::graph_traits<wrong_tag>;
        gl::graph<wrong_traits> invalid_sut_out{fixture.n_vertices};

        std::stringstream err_ss;
        err_ss << gl::io::spec_fmt << invalid_sut_out;

        CHECK_THROWS_AS(err_ss >> fixture.sut_in, std::ios_base::failure);
    }

    SUBCASE("io read should throw for a stream with vertex properties if vertex_properties_type is "
            "not readable") {
        fixture.ss << gl::io::with_vertex_properties << fixture.sut_out;

        using not_readable_vp_traits = gl::graph_traits<dir_tag, std::monostate, gl::name_property>;
        gl::graph<not_readable_vp_traits> invalid_sut_in;

        CHECK_THROWS_AS(fixture.ss >> invalid_sut_in, std::ios_base::failure);
    }

    SUBCASE("io read should throw for a stream with edge properties if edge_properties_type is not "
            "readable") {
        fixture.ss << gl::io::with_edge_properties << fixture.sut_out;

        using not_readable_hp_traits = gl::graph_traits<dir_tag, gl::name_property, std::monostate>;
        gl::graph<not_readable_hp_traits> invalid_sut_in;

        CHECK_THROWS_AS(fixture.ss >> invalid_sut_in, std::ios_base::failure);
    }

    SUBCASE("io operators should properly write and read the graph from a stream with no "
            "additional options enabled") {
        fixture.ss << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_graph_structure(fixture.sut_in, fixture.sut_out);
    }

    SUBCASE("io operators should properly write and read the graph from a stream with vertex "
            "properties") {
        fixture.ss << gl::io::with_vertex_properties << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_graph_structure(fixture.sut_in, fixture.sut_out);
        verify_vertex_properties(fixture.sut_in, fixture.sut_out);
    }

    SUBCASE("io operators should properly write and read the graph from a stream with edge "
            "properties") {
        fixture.ss << gl::io::with_edge_properties << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_graph_structure(fixture.sut_in, fixture.sut_out);
        verify_edge_properties(fixture.sut_in, fixture.sut_out);
    }

    SUBCASE("io operators should properly write and read the graph from a stream with both vertex "
            "and edge properties") {
        fixture.ss << gl::io::with_properties << fixture.sut_out;
        fixture.ss >> fixture.sut_in;

        verify_graph_structure(fixture.sut_in, fixture.sut_out);
        verify_vertex_properties(fixture.sut_in, fixture.sut_out);
        verify_edge_properties(fixture.sut_in, fixture.sut_out);
    }
}

// clang-format off

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_io_template,
    gl::graph<gl::list_graph_traits<gl::directed_t, gl::name_property, gl::name_property>>,
    gl::graph<gl::list_graph_traits<gl::undirected_t, gl::name_property, gl::name_property>>,
    gl::graph<gl::flat_list_graph_traits<gl::directed_t, gl::name_property, gl::name_property>>,
    gl::graph<gl::flat_list_graph_traits<gl::undirected_t, gl::name_property, gl::name_property>>,
    gl::graph<gl::matrix_graph_traits<gl::directed_t, gl::name_property, gl::name_property>>,
    gl::graph<gl::matrix_graph_traits<gl::undirected_t, gl::name_property, gl::name_property>>,
    gl::graph<gl::flat_matrix_graph_traits<gl::directed_t, gl::name_property, gl::name_property>>,
    gl::graph<gl::flat_matrix_graph_traits<gl::undirected_t, gl::name_property, gl::name_property>>
);

// clang-format on

struct test_graph_io_options {
    using opt = gl::io::detail::option_bit;

    [[nodiscard]] bool is_set(opt bit) {
        return gl::io::is_option_set(ss, bit);
    }

    std::stringstream ss;
};

TEST_CASE_FIXTURE(test_graph_io_options, "layout options should be mutually exclusive") {
    REQUIRE_FALSE(is_set(opt::verbose));
    REQUIRE_FALSE(is_set(opt::spec_fmt));

    // verbose - spec_fmt toggling
    ss << gl::io::verbose;
    CHECK(is_set(opt::verbose));
    CHECK_FALSE(is_set(opt::spec_fmt));

    ss << gl::io::spec_fmt;
    CHECK_FALSE(is_set(opt::verbose));
    CHECK(is_set(opt::spec_fmt));

    ss << gl::io::verbose;
    CHECK(is_set(opt::verbose));
    CHECK_FALSE(is_set(opt::spec_fmt));

    // concise toggling
    ss << gl::io::concise;
    CHECK_FALSE(is_set(opt::verbose));
    CHECK_FALSE(is_set(opt::spec_fmt));

    ss << gl::io::spec_fmt;
    CHECK(is_set(opt::spec_fmt));
    ss << gl::io::concise;
    CHECK_FALSE(is_set(opt::verbose));
    CHECK_FALSE(is_set(opt::spec_fmt));
}

TEST_CASE_FIXTURE(
    test_graph_io_options, "with_vertex_properties should toggle only the vertex properties option"
) {
    REQUIRE_FALSE(is_set(opt::with_vertex_properties));
    REQUIRE_FALSE(is_set(opt::with_connection_properties));

    ss << gl::io::with_vertex_properties;
    CHECK(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));
}

TEST_CASE_FIXTURE(
    test_graph_io_options,
    "without_vertex_properties should disable only the vertex properties option"
) {
    ss << gl::io::with_properties;
    REQUIRE(is_set(opt::with_vertex_properties));
    REQUIRE(is_set(opt::with_connection_properties));

    ss << gl::io::without_vertex_properties;
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK(is_set(opt::with_connection_properties));
}

TEST_CASE_FIXTURE(
    test_graph_io_options,
    "with_edge_properties should toggle only the connection properties option"
) {
    REQUIRE_FALSE(is_set(opt::with_vertex_properties));
    REQUIRE_FALSE(is_set(opt::with_connection_properties));

    ss << gl::io::with_edge_properties;
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK(is_set(opt::with_connection_properties));
}

TEST_CASE_FIXTURE(
    test_graph_io_options,
    "without_edge_properties should disable only the connection properties option"
) {
    ss << gl::io::with_properties;
    REQUIRE(is_set(opt::with_vertex_properties));
    REQUIRE(is_set(opt::with_connection_properties));

    ss << gl::io::without_edge_properties;
    CHECK(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));
}

TEST_CASE_FIXTURE(
    test_graph_io_options,
    "with(out)_properties should toggle both vertex and connection properties options"
) {
    REQUIRE_FALSE(is_set(opt::with_vertex_properties));
    REQUIRE_FALSE(is_set(opt::with_connection_properties));

    ss << gl::io::with_properties;
    CHECK(is_set(opt::with_vertex_properties));
    CHECK(is_set(opt::with_connection_properties));

    // both enabled
    ss << gl::io::without_properties;
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));

    // one enabled
    ss << gl::io::with_vertex_properties;
    CHECK(is_set(opt::with_vertex_properties));
    ss << gl::io::without_properties;
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));

    ss << gl::io::with_edge_properties;
    CHECK(is_set(opt::with_connection_properties));
    ss << gl::io::without_properties;
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));
}

TEST_CASE_FIXTURE(test_graph_io_options, "default_options should disable all option flags") {
    // concise layout
    ss << gl::io::with_properties;
    REQUIRE_FALSE(is_set(opt::verbose));
    REQUIRE_FALSE(is_set(opt::spec_fmt));
    REQUIRE(is_set(opt::with_vertex_properties));
    REQUIRE(is_set(opt::with_connection_properties));

    ss << gl::io::default_options;
    CHECK_FALSE(is_set(opt::verbose));
    CHECK_FALSE(is_set(opt::spec_fmt));
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));

    // verbose layout
    ss << gl::io::verbose << gl::io::with_properties;
    REQUIRE(is_set(opt::verbose));
    REQUIRE_FALSE(is_set(opt::spec_fmt));
    REQUIRE(is_set(opt::with_vertex_properties));
    REQUIRE(is_set(opt::with_connection_properties));

    ss << gl::io::default_options;
    CHECK_FALSE(is_set(opt::verbose));
    CHECK_FALSE(is_set(opt::spec_fmt));
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));

    // specification layout
    ss << gl::io::spec_fmt << gl::io::with_properties;
    REQUIRE_FALSE(is_set(opt::verbose));
    REQUIRE(is_set(opt::spec_fmt));
    REQUIRE(is_set(opt::with_vertex_properties));
    REQUIRE(is_set(opt::with_connection_properties));

    ss << gl::io::default_options;
    CHECK_FALSE(is_set(opt::verbose));
    CHECK_FALSE(is_set(opt::spec_fmt));
    CHECK_FALSE(is_set(opt::with_vertex_properties));
    CHECK_FALSE(is_set(opt::with_connection_properties));
}

TEST_SUITE_END(); // test_graph_io

} // namespace gl_testing
