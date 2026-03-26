#include "testing/gl/constants.hpp"

#include <gl/graph.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <deque>

namespace gl_testing {

TEST_SUITE_BEGIN("test_vertex_degree_getters");

inline constexpr auto get_id = [](auto&& element) -> gl::default_id_type { return element.id(); };

TEST_CASE_TEMPLATE_DEFINE(
    "vertex degree getter tests for directed graphs", TraitsType, directed_graph_traits_template
) {
    using sut_type = gl::graph<TraitsType>;
    using vertex_type = typename sut_type::vertex_type;

    const auto n_vertices = constants::n_elements_top;

    sut_type sut;
    std::deque<gl::size_type> expected_in_deg_list, expected_out_deg_list;

    SUBCASE("clique") {
        sut = gl::topology::clique<sut_type>(n_vertices);
        expected_in_deg_list = std::deque<gl::size_type>(n_vertices, n_vertices - 1uz);
        expected_out_deg_list = expected_in_deg_list;
    }

    SUBCASE("clique with an additional loop") {
        sut = gl::topology::clique<sut_type>(n_vertices);
        sut.add_edge(0uz, 0uz);

        expected_in_deg_list = std::deque<gl::size_type>(n_vertices, n_vertices - 1uz);
        expected_in_deg_list.front()++;

        expected_out_deg_list = expected_in_deg_list;
    }

    SUBCASE("cycle") {
        sut = gl::topology::cycle<sut_type>(n_vertices);
        expected_in_deg_list = std::deque<gl::size_type>(n_vertices, 1uz);
        expected_out_deg_list = expected_in_deg_list;
    }

    SUBCASE("path") {
        sut = gl::topology::path<sut_type>(n_vertices);

        expected_in_deg_list = std::deque<gl::size_type>(n_vertices - 1uz, 1uz);
        expected_in_deg_list.push_front(0uz);

        expected_out_deg_list = std::deque<gl::size_type>(n_vertices - 1uz, 1uz);
        expected_out_deg_list.push_back(0uz);
    }

    CAPTURE(sut);
    CAPTURE(expected_in_deg_list);
    CAPTURE(expected_out_deg_list);

    std::deque<gl::size_type> expected_deg_list(n_vertices);
    std::ranges::transform(
        expected_in_deg_list,
        expected_out_deg_list,
        expected_deg_list.begin(),
        std::plus<gl::size_type>{}
    );

    gl::size_type i = 0uz;
    CHECK(std::ranges::all_of(sut.vertices(), [&](const auto& vertex) {
        const bool result =
            sut.in_degree(vertex) == expected_in_deg_list[i]
            and sut.out_degree(vertex) == expected_out_deg_list[i]
            and sut.degree(vertex) == expected_deg_list[i];
        ++i;
        return result;
    }));

    i = 0uz;
    CHECK(std::ranges::all_of(
        sut.vertices(),
        [&](const gl::default_id_type vertex_id) {
            const bool result =
                sut.in_degree(vertex_id) == expected_in_deg_list[i]
                and sut.out_degree(vertex_id) == expected_out_deg_list[i]
                and sut.degree(vertex_id) == expected_deg_list[i];
            ++i;
            return result;
        },
        get_id
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    directed_graph_traits_template,
    gl::list_graph_traits<gl::directed_t>, // directed adjacency list graph
    gl::flat_list_graph_traits<gl::directed_t>, // directed flat adjacency list graph
    gl::matrix_graph_traits<gl::directed_t> // directed adjacency matrix graph
);

TEST_CASE_TEMPLATE_DEFINE(
    "vertex degree getter tests for undirected graphs", TraitsType, undirected_graph_traits_template
) {
    using sut_type = gl::graph<TraitsType>;
    using vertex_type = typename sut_type::vertex_type;

    const auto n_vertices = constants::n_elements_top;

    sut_type sut;
    std::deque<gl::size_type> expected_deg_list;

    SUBCASE("clique") {
        sut = gl::topology::clique<sut_type>(n_vertices);
        expected_deg_list = std::deque<gl::size_type>(n_vertices, n_vertices - 1uz);
    }

    SUBCASE("clique with an additional loop") {
        sut = gl::topology::clique<sut_type>(n_vertices);
        sut.add_edge(0uz, 0uz);

        expected_deg_list = std::deque<gl::size_type>(n_vertices, n_vertices - 1uz);
        expected_deg_list.front() += 2uz; // loops counted twice
    }

    SUBCASE("cycle") {
        sut = gl::topology::cycle<sut_type>(n_vertices);
        expected_deg_list = std::deque<gl::size_type>(n_vertices, 2uz);
    }

    SUBCASE("path") {
        sut = gl::topology::path<sut_type>(n_vertices);

        expected_deg_list = std::deque<gl::size_type>(n_vertices - 2uz, 2uz);
        expected_deg_list.push_front(1uz);
        expected_deg_list.push_back(1uz);
    }

    CAPTURE(sut);
    CAPTURE(expected_deg_list);

    gl::size_type i = 0uz;
    CHECK(std::ranges::all_of(sut.vertices(), [&](const auto& vertex) {
        const auto expected_deg = expected_deg_list[i];
        const bool result =
            sut.in_degree(vertex) == expected_deg and sut.out_degree(vertex) == expected_deg
            and sut.degree(vertex) == expected_deg;
        ++i;
        return result;
    }));

    i = 0uz;
    CHECK(std::ranges::all_of(
        sut.vertices(),
        [&](const gl::default_id_type vertex_id) {
            const auto expected_deg = expected_deg_list[i];
            const bool result =
                sut.in_degree(vertex_id) == expected_deg
                and sut.out_degree(vertex_id) == expected_deg
                and sut.degree(vertex_id) == expected_deg;
            ++i;
            return result;
        },
        get_id
    ));
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    undirected_graph_traits_template,
    gl::list_graph_traits<gl::undirected_t>, // undirected adjacency list graph
    gl::flat_list_graph_traits<gl::undirected_t>, // undirected flat adjacency list graph
    gl::matrix_graph_traits<gl::undirected_t> // undirected adjacency matrix graph
);

TEST_SUITE_END(); // test_vertex_degree_getters

} // namespace gl_testing
