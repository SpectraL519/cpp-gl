#include "doctest.h"
#include "testing/common/functional.hpp"
#include "testing/common/io.hpp"
#include "testing/gl/constants.hpp"
#include "testing/gl/io.hpp"

#include <gl/graph.hpp>
#include <gl/io/graph_fio.hpp>
#include <gl/topology.hpp>

#include <print>

namespace gl_testing {

TEST_SUITE_BEGIN("test_graph_file_io");

template <typename SutType>
struct test_graph_file_io_fixture {
    using sut_type = SutType;
    using directional_tag = typename sut_type::directional_tag;

    test_graph_file_io_fixture() {
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

    ~test_graph_file_io_fixture() {
        fs::remove(path);
    }

    const gl::size_type n_vertices = 5ull;
    sut_type sut_out;

    fs::path path{"test_graph_file_io.gsf"};
};

TEST_CASE_TEMPLATE_DEFINE("graph file io tests", SutType, graph_file_io_template) {
    using fixture_type = test_graph_file_io_fixture<SutType>;
    using sut_type = typename fixture_type::sut_type;

    fixture_type fixture;

    SUBCASE("save should throw in the write mode if a file already exists") {
        std::ofstream file(fixture.path);
        if (not file.is_open())
            FAIL("Could not initialize an empty file");

        GL_REQUIRE_THROWS_FS_ERROR(
            gl::io::save(fixture.sut_out, fixture.path), std::errc::file_exists
        );
    }

    SUBCASE("load should throw if a file does not exist") {
        GL_REQUIRE_THROWS_FS_ERROR(
            discard(gl::io::load<SutType>(fixture.path)), std::errc::no_such_file_or_directory
        );
    }

    SUBCASE("file io should properly save and load a graph in a gsf format") {
        gl::io::save(fixture.sut_out, fixture.path);
        const auto sut_in = gl::io::load<sut_type>(fixture.path);

        verify_graph_structure(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a graph in a gsf format with vertex properties"
    ) {
        gl::io::save(fixture.sut_out, fixture.path, {gl::io::with_vertex_properties});
        const auto sut_in = gl::io::load<sut_type>(fixture.path);

        verify_graph_structure(sut_in, fixture.sut_out);
        verify_vertex_properties(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a graph in a gsf format with edge properties") {
        gl::io::save(fixture.sut_out, fixture.path, {gl::io::with_edge_properties});
        const auto sut_in = gl::io::load<sut_type>(fixture.path);

        verify_graph_structure(sut_in, fixture.sut_out);
        verify_edge_properties(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a graph in a gsf format with vertex and edge "
            "properties") {
        gl::io::save(fixture.sut_out, fixture.path, {gl::io::with_properties});
        const auto sut_in = gl::io::load<sut_type>(fixture.path);

        verify_graph_structure(sut_in, fixture.sut_out);
        verify_vertex_properties(sut_in, fixture.sut_out);
        verify_edge_properties(sut_in, fixture.sut_out);
    }
}

// clang-format off

TEST_CASE_TEMPLATE_INSTANTIATE(
    graph_file_io_template,
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

TEST_SUITE_END(); // test_graph_file_io

} // namespace gl_testing
