#include "testing/gl/constants.hpp"
#include "testing/gl/io_common.hpp"

#include <gl/graph.hpp>
#include <gl/io.hpp>
#include <gl/topology.hpp>

#include <doctest.h>

#include <sstream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_io");

// Tests covering only the io functionality with the graph specification format enabled

struct test_directed_graph_io {
    using traits_type = gl::graph_traits<gl::directed_t, gl::name_property, gl::name_property>;
    using sut_type = gl::graph<traits_type>;

    test_directed_graph_io() {
        ss << gl::io::enable_gsf;

        sut_out = gl::topology::clique<sut_type>(n_vertices);

        // prepare vertex and edge properties
        std::size_t v_idx = 0, e_idx = 0;
        for (const auto& vertex : sut_out.vertices()) {
            vertex.properties() = std::format("vertex_{}", v_idx++);
            for (const auto& edge : sut_out.incident_edges(vertex))
                edge.properties() = std::format("edge_{}", e_idx++);
        }
    }

    const gl::size_type n_vertices = 5ull;
    sut_type sut_out;
    sut_type sut_in;

    std::stringstream ss;
};

TEST_CASE_FIXTURE(
    test_directed_graph_io, "io read should throw if the directional tag doesn't match"
) {
    using undir_traits = gl::graph_traits<gl::undirected_t>;

    gl::graph<undir_traits> invalid_sut_out{n_vertices};
    ss << invalid_sut_out;

    CHECK_THROWS_AS(ss >> sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io read should throw for a stream with vertex properties if vertex_properties_type is not "
    "readable"
) {
    ss << gl::io::with_vertex_properties << sut_out;

    using not_readable_vp_traits =
        gl::graph_traits<gl::directed_t, std::monostate, gl::name_property>;
    gl::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io read should throw for a stream with edge properties if edge_properties_type is not readable"
) {
    ss << gl::io::with_edge_properties << sut_out;

    using not_readable_vp_traits =
        gl::graph_traits<gl::directed_t, gl::name_property, std::monostate>;
    gl::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with no additional "
    "options enabled"
) {
    ss << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with vertex properties"
) {
    ss << gl::io::with_vertex_properties << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
    verify_vertex_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with edge properties"
) {
    ss << gl::io::with_edge_properties << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
    verify_edge_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with both vertex and edge "
    "properties"
) {
    ss << gl::io::with_vertex_properties << gl::io::with_edge_properties << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
    verify_vertex_properties(sut_in, sut_out);
    verify_edge_properties(sut_in, sut_out);
}

struct test_undirected_graph_io {
    using traits_type = gl::graph_traits<gl::undirected_t, gl::name_property, gl::name_property>;
    using sut_type = gl::graph<traits_type>;

    test_undirected_graph_io() {
        ss << gl::io::enable_gsf;

        sut_out = gl::topology::clique<sut_type>(n_vertices);

        // prepare vertex and edge properties
        std::size_t v_idx = 0, e_idx = 0;
        for (const auto& vertex : sut_out.vertices()) {
            vertex.properties() = std::format("vertex_{}", v_idx++);
            for (const auto& edge : sut_out.incident_edges(vertex))
                if (edge.source() == vertex.id())
                    edge.properties() = std::format("edge_{}", e_idx++);
        }
    }

    const gl::size_type n_vertices = 5ull;
    sut_type sut_out;
    sut_type sut_in;

    std::stringstream ss;
};

TEST_CASE_FIXTURE(
    test_undirected_graph_io, "io read should throw if the directional tag doesn't match"
) {
    using dir_traits = gl::graph_traits<gl::directed_t>;

    gl::graph<dir_traits> invalid_sut_out{n_vertices};
    ss << invalid_sut_out;

    CHECK_THROWS_AS(ss >> sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io read should throw for a stream with vertex properties if vertex_properties_type is not "
    "readable"
) {
    ss << gl::io::with_vertex_properties << sut_out;

    using not_readable_vp_traits =
        gl::graph_traits<gl::undirected_t, std::monostate, gl::name_property>;
    gl::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io read should throw for a stream with edge properties if edge_properties_type is not readable"
) {
    ss << gl::io::with_edge_properties << sut_out;

    using not_readable_vp_traits =
        gl::graph_traits<gl::undirected_t, gl::name_property, std::monostate>;
    gl::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with no additional "
    "options enabled"
) {
    ss << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with vertex properties"
) {
    ss << gl::io::with_vertex_properties << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
    verify_vertex_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with edge properties"
) {
    ss << gl::io::with_edge_properties << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
    verify_edge_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with both vertex and edge "
    "properties"
) {
    ss << gl::io::with_vertex_properties << gl::io::with_edge_properties << sut_out;
    ss >> sut_in;

    verify_graph_structure(sut_in, sut_out);
    verify_vertex_properties(sut_in, sut_out);
    verify_edge_properties(sut_in, sut_out);
}

TEST_SUITE_END(); // test_graph_io

} // namespace gl_testing
