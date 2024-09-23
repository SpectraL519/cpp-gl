#include "constants.hpp"

#include <gl/graph.hpp>
#include <gl/io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <sstream>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_io");

// Tests covering only the io functionality with the graph specification format enabled

namespace common {

template <lib_tt::c_graph GraphType>
void verify_graph_structure(const GraphType& g1, const GraphType& g2) {
    REQUIRE_EQ(g1.n_vertices(), g2.n_vertices());
    REQUIRE_EQ(g1.n_unique_edges(), g2.n_unique_edges());

    // verify that the edges of the in graph are equivalent to the edges of the out graph
    CHECK(std::ranges::all_of(g1.vertices(), [&](const auto& v1) {
        return std::ranges::all_of(g1.adjacent_edges(v1), [&](const auto& edge) {
            return g2.has_edge(edge.first_id(), edge.second_id());
        });
    }));
}

template <lib_tt::c_graph GraphType>
void verify_vertex_properties(const GraphType& g1, const GraphType& g2) {
    const auto properties_proj = [](const auto& vertex) { return vertex.properties; };

    CHECK(std::ranges::equal(
        g1.vertices(), g2.vertices(), std::ranges::equal_to{}, properties_proj, properties_proj
    ));
}

template <lib_tt::c_graph GraphType>
void verify_edge_properties(const GraphType& g1, const GraphType& g2) {
    const auto properties_proj = [](const auto& vertex) { return vertex.properties; };

    CHECK(std::ranges::all_of(g1.vertices(), [&](const auto& v1) {
        return std::ranges::all_of(g1.adjacent_edges(v1), [&](const auto& edge) {
            // get edge returns optional<ref_wrap>
            return edge.properties
                == g2.get_edge(edge.first_id(), edge.second_id()).value().get().properties;
        });
    }));
}

} // namespace common

struct test_directed_graph_io {
    using traits_type =
        lib::graph_traits<lib::directed_t, lib_t::name_property, lib_t::name_property>;
    using sut_type = lib::graph<traits_type>;

    test_directed_graph_io() {
        ss << lib::io::enable_gsf;

        sut_out = lib::topology::clique<sut_type>(n_vertices);

        // prepare vertex and edge properties
        std::size_t v_idx = 0, e_idx = 0;
        for (const auto& vertex : sut_out.vertices()) {
            vertex.properties = {std::format("vertex_{}", v_idx++)};
            for (const auto& edge : sut_out.adjacent_edges(vertex))
                edge.properties = {std::format("edge_{}", e_idx++)};
        }
    }

    const lib_t::size_type n_vertices = 5ull;
    sut_type sut_out;
    sut_type sut_in;

    std::stringstream ss;
};

TEST_CASE_FIXTURE(
    test_directed_graph_io, "io read should throw if the directional tag doesn't match"
) {
    using undir_traits = lib::graph_traits<lib::undirected_t>;

    lib::graph<undir_traits> invalid_sut_out{n_vertices};
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
        lib::graph_traits<lib::directed_t, std::monostate, lib_t::name_property>;
    lib::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io read should throw for a stream with edge properties if edge_properties_type is not readable"
) {
    ss << gl::io::with_edge_properties << sut_out;

    using not_readable_vp_traits =
        lib::graph_traits<lib::directed_t, lib_t::name_property, std::monostate>;
    lib::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with no additional "
    "options enabled"
) {
    ss << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with vertex properties"
) {
    ss << lib::io::with_vertex_properties << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
    common::verify_vertex_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with edge properties"
) {
    ss << lib::io::with_edge_properties << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
    common::verify_edge_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_directed_graph_io,
    "io operators should properly write and read the graph from a stream with both vertex and edge "
    "properties"
) {
    ss << lib::io::with_vertex_properties << lib::io::with_edge_properties << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
    common::verify_vertex_properties(sut_in, sut_out);
    common::verify_edge_properties(sut_in, sut_out);
}

struct test_undirected_graph_io {
    using traits_type =
        lib::graph_traits<lib::undirected_t, lib_t::name_property, lib_t::name_property>;
    using sut_type = lib::graph<traits_type>;

    test_undirected_graph_io() {
        ss << lib::io::enable_gsf;

        sut_out = lib::topology::clique<sut_type>(n_vertices);

        // prepare vertex and edge properties
        std::size_t v_idx = 0, e_idx = 0;
        for (const auto& vertex : sut_out.vertices()) {
            vertex.properties = {std::format("vertex_{}", v_idx++)};
            for (const auto& edge : sut_out.adjacent_edges(vertex))
                if (edge.first_id() == vertex.id())
                    edge.properties = {std::format("edge_{}", e_idx++)};
        }
    }

    const lib_t::size_type n_vertices = 5ull;
    sut_type sut_out;
    sut_type sut_in;

    std::stringstream ss;
};

TEST_CASE_FIXTURE(
    test_undirected_graph_io, "io read should throw if the directional tag doesn't match"
) {
    using dir_traits = lib::graph_traits<lib::directed_t>;

    lib::graph<dir_traits> invalid_sut_out{n_vertices};
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
        lib::graph_traits<lib::undirected_t, std::monostate, lib_t::name_property>;
    lib::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io read should throw for a stream with edge properties if edge_properties_type is not readable"
) {
    ss << gl::io::with_edge_properties << sut_out;

    using not_readable_vp_traits =
        lib::graph_traits<lib::undirected_t, lib_t::name_property, std::monostate>;
    lib::graph<not_readable_vp_traits> invalid_sut_in;

    CHECK_THROWS_AS(ss >> invalid_sut_in, std::ios_base::failure);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with no additional "
    "options enabled"
) {
    ss << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with vertex properties"
) {
    ss << lib::io::with_vertex_properties << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
    common::verify_vertex_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with edge properties"
) {
    ss << lib::io::with_edge_properties << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
    common::verify_edge_properties(sut_in, sut_out);
}

TEST_CASE_FIXTURE(
    test_undirected_graph_io,
    "io operators should properly write and read the graph from a stream with both vertex and edge "
    "properties"
) {
    ss << lib::io::with_vertex_properties << lib::io::with_edge_properties << sut_out;
    ss >> sut_in;

    common::verify_graph_structure(sut_in, sut_out);
    common::verify_vertex_properties(sut_in, sut_out);
    common::verify_edge_properties(sut_in, sut_out);
}

TEST_SUITE_END(); // test_graph_io

} // namespace gl_testing
