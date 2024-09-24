#include "constants.hpp"
#include "functional.hpp"
#include "io_common.hpp"

#include <gl/graph.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

namespace fs = std::filesystem;

namespace gl_testing {

#define CUSTOM_REQUIRE_THROWS_FS_ERROR(expr, errc)                                             \
    try {                                                                                      \
        expr;                                                                                  \
        FAIL("Expected `std::filesystem::filesystem_error` but no exception was thrown");      \
    }                                                                                          \
    catch (const fs::filesystem_error& e) {                                                    \
        const auto expected_code = std::make_error_code(errc);                                 \
        if (e.code() != expected_code) {                                                       \
            FAIL(std::format(                                                                  \
                "Expected error code {}, but got {}.", expected_code.value(), e.code().value() \
            ));                                                                                \
        }                                                                                      \
    }                                                                                          \
    catch (...) {                                                                              \
        FAIL("Expected `std::filesystem::filesystem_error` but caught a different exception"); \
    }

TEST_SUITE_BEGIN("test_graph_file_io");

// Tests covering only the io functionality with the graph specification format enabled

struct test_graph_file_io {
    using traits_type =
        lib::graph_traits<lib::directed_t, lib_t::name_property, lib_t::name_property>;
    using sut_type = lib::graph<traits_type>;

    test_graph_file_io() {
        sut_out = lib::topology::clique<sut_type>(n_vertices);

        // prepare vertex and edge properties
        std::size_t v_idx = 0, e_idx = 0;
        for (const auto& vertex : sut_out.vertices()) {
            vertex.properties = {std::format("vertex_{}", v_idx++)};
            for (const auto& edge : sut_out.adjacent_edges(vertex))
                edge.properties = {std::format("edge_{}", e_idx++)};
        }
    }

    ~test_graph_file_io() {
        fs::remove(path);
    }

    const lib_t::size_type n_vertices = 5ull;
    sut_type sut_out;

    fs::path path{"test_directed_graph_file_io.gsf"};
};

TEST_CASE_TEMPLATE_DEFINE("graph file io tests", SutType, directional_tag_sut_template) {
    using fixture_type = test_graph_file_io;
    using sut_type = typename fixture_type::sut_type;

    fixture_type fixture;

    SUBCASE("save should throw in the write mode if a file already exists") {
        std::ofstream file(fixture.path);
        if (not file.is_open())
            FAIL("Could not initialize an empty file");

        CUSTOM_REQUIRE_THROWS_FS_ERROR(
            lib::io::save(fixture.sut_out, fixture.path), std::errc::file_exists
        );
    }

    SUBCASE("load shoul throw if a file does not exist") {
        CUSTOM_REQUIRE_THROWS_FS_ERROR(
            func::discard_result(lib::io::load<SutType>(fixture.path)),
            std::errc::no_such_file_or_directory
        );
    }

    // TODO: add
    // * invalid file type tests
    // * could not open file tests

    SUBCASE("file io should properly save and load a graph in a gsf format") {
        lib::io::save(fixture.sut_out, fixture.path);
        const auto sut_in = lib::io::load<sut_type>(fixture.path);

        io_common::verify_graph_structure(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a graph in a gsf format with vertex properties"
    ) {
        lib::io::save(fixture.sut_out, fixture.path, {lib::io::with_vertex_properties});
        const auto sut_in = lib::io::load<sut_type>(fixture.path);

        io_common::verify_graph_structure(sut_in, fixture.sut_out);
        io_common::verify_vertex_properties(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a graph in a gsf format with edge properties") {
        lib::io::save(fixture.sut_out, fixture.path, {lib::io::with_edge_properties});
        const auto sut_in = lib::io::load<sut_type>(fixture.path);

        io_common::verify_graph_structure(sut_in, fixture.sut_out);
        io_common::verify_edge_properties(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a graph in a gsf format with vertex and edge "
            "properties") {
        lib::io::save(fixture.sut_out, fixture.path, {lib::io::with_properties});
        const auto sut_in = lib::io::load<sut_type>(fixture.path);

        io_common::verify_graph_structure(sut_in, fixture.sut_out);
        io_common::verify_vertex_properties(sut_in, fixture.sut_out);
        io_common::verify_edge_properties(sut_in, fixture.sut_out);
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    directional_tag_sut_template,
    lib::graph<lib::list_graph_traits<lib::directed_t>>, // directed adj list
    lib::graph<lib::list_graph_traits<lib::undirected_t>> // undirected adj list
);

TEST_SUITE_END(); // test_graph_file_io

} // namespace gl_testing
