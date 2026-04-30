#include "doctest.h"
#include "testing/common/functional.hpp"
#include "testing/common/io.hpp"
#include "testing/hgl/io.hpp"

#include <hgl/hypergraph.hpp>
#include <hgl/io/hypergraph_fio.hpp>

#include <print>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph_file_io");

template <typename SutType>
struct test_hypergraph_file_io_fixture {
    using sut_type = SutType;
    using directional_tag = typename sut_type::directional_tag;

    test_hypergraph_file_io_fixture() {
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
        }
    }

    ~test_hypergraph_file_io_fixture() {
        fs::remove(path);
    }

    const hgl::size_type n_vertices = 5uz;
    sut_type sut_out;

    fs::path path{"test_hypergraph_file_io.hgsf"};
};

TEST_CASE_TEMPLATE_DEFINE("hypergraph file io tests", SutType, hypergraph_file_io_template) {
    using fixture_type = test_hypergraph_file_io_fixture<SutType>;
    using sut_type = typename fixture_type::sut_type;

    fixture_type fixture;

    SUBCASE("save should throw in the write mode if a file already exists") {
        std::ofstream file(fixture.path);
        if (not file.is_open())
            FAIL("Could not initialize an empty file");

        GL_REQUIRE_THROWS_FS_ERROR(
            hgl::io::save(fixture.sut_out, fixture.path), std::errc::file_exists
        );
    }

    SUBCASE("load should throw if a file does not exist") {
        GL_REQUIRE_THROWS_FS_ERROR(
            discard(hgl::io::load<SutType>(fixture.path)), std::errc::no_such_file_or_directory
        );
    }

    SUBCASE("file io should properly save and load a hypergraph in a hgsf format") {
        hgl::io::save(fixture.sut_out, fixture.path);
        const auto sut_in = hgl::io::load<sut_type>(fixture.path);

        verify_hypergraph_structure(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a hypergraph in a hgsf format with vertex "
            "properties") {
        hgl::io::save(fixture.sut_out, fixture.path, {hgl::io::with_vertex_properties});
        const auto sut_in = hgl::io::load<sut_type>(fixture.path);

        verify_hypergraph_structure(sut_in, fixture.sut_out);
        verify_vertex_properties(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a hypergraph in a hgsf format with hyperedge "
            "properties") {
        hgl::io::save(fixture.sut_out, fixture.path, {hgl::io::with_hyperedge_properties});
        const auto sut_in = hgl::io::load<sut_type>(fixture.path);

        verify_hypergraph_structure(sut_in, fixture.sut_out);
        verify_hyperedge_properties(sut_in, fixture.sut_out);
    }

    SUBCASE("file io should properly save and load a hypergraph in a hgsf format with vertex and "
            "hyperedge properties") {
        hgl::io::save(fixture.sut_out, fixture.path, {hgl::io::with_properties});
        const auto sut_in = hgl::io::load<sut_type>(fixture.path);

        verify_hypergraph_structure(sut_in, fixture.sut_out);
        verify_vertex_properties(sut_in, fixture.sut_out);
        verify_hyperedge_properties(sut_in, fixture.sut_out);
    }
}

// clang-format off

TEST_CASE_TEMPLATE_INSTANTIATE(
    hypergraph_file_io_template,
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

TEST_SUITE_END(); // test_hypergraph_file_io

} // namespace hgl_testing
