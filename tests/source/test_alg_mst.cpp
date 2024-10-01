#include "alg_common.hpp"
#include "constants.hpp"
#include "functional.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

#include <cmath>

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_mst");

TEST_CASE_TEMPLATE_DEFINE(
    "prim mst finding tests for graphs with explicitly weighted edges",
    TraitsType,
    wieghted_edge_traits_type_template
) {
    using sut_type = lib::graph<TraitsType>;
    using weight_type = typename sut_type::edge_properties_type::weight_type;
    using distance_type = weight_type;

    static_assert(lib_tt::c_weight_properties_type<typename sut_type::edge_properties_type>);

    SUBCASE("should throw if there is an edge with a negative weight") {
        const auto sut = lib::topology::clique<sut_type>(constants::n_elements_alg);
        sut.get_edge(constants::vertex_id_1, constants::vertex_id_2)
            .value()
            .get()
            .properties.weight = -static_cast<weight_type>(constants::n_elements_alg);

        CHECK_THROWS_AS(
            func::discard_result(lib::algorithm::prim_mst(sut, constants::vertex_id_1)),
            std::invalid_argument
        );
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(
    wieghted_edge_traits_type_template,
    lib::list_graph_traits<
        lib::directed_t,
        lib_t::empty_properties,
        lib_t::weight_property<>> // directed adjacency list graph
    // lib::list_graph_traits<
    //     lib::undirected_t,
    //     lib_t::empty_properties,
    //     lib_t::weight_property<>>, // undirected adjacency list graph
    // lib::matrix_graph_traits<
    //     lib::directed_t,
    //     lib_t::empty_properties,
    //     lib_t::weight_property<>>, // directed adjacency matrix graph
    // lib::matrix_graph_traits<
    //     lib::undirected_t,
    //     lib_t::empty_properties,
    //     lib_t::weight_property<>> // undirected adjacency matrix graph
);

TEST_SUITE_END(); // test_alg_mst

} // namespace gl_testing
