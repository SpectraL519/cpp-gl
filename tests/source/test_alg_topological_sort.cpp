#include "alg_common.hpp"
#include "constants.hpp"

#include <gl/algorithms.hpp>
#include <gl/graph_file_io.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

namespace fs = std::filesystem;

namespace gl_testing {

TEST_SUITE_BEGIN("test_alg_topological_sort");

std::vector<lib_t::id_type> read_topological_order(
    const lib_t::size_type n_vertices, const fs::path& order_file_path
) {
    std::vector<lib_t::id_type> order(n_vertices);

    std::ifstream order_file(order_file_path);
    if (not order_file) {
        throw std::filesystem::filesystem_error(
            "Cannot read file",
            order_file_path,
            std::make_error_code(std::errc::no_such_file_or_directory)
        );
    }

    for (lib_t::size_type i = 0; i < n_vertices; i++)
        order_file >> order[i];

    return order;
}

static const fs::path data_path(TEST_DATA_PATH);

TEST_CASE_TEMPLATE_DEFINE(
    "topological sort tests for directed graphs", TraitsType, directed_traits_type_template
) {
    using sut_type = lib::graph<TraitsType>;

    SUBCASE("acyclic graph") {
        std::cout << "---------------" << std::endl;

        std::cout << "cwd = " << fs::current_path() << std::endl;
        std::cout << "data_path = " << data_path << std::endl;

        const fs::path gsf_file_path = data_path / "topological_sort_directed_acyclic_graph.gsf";
        const auto sut = lib::io::load<sut_type>(gsf_file_path);
        std::cout << sut << std::endl;

        const fs::path order_file_path = data_path / "topological_sort_directed_acyclic_order.txt";
        const auto expected_topological_order =
            read_topological_order(sut.n_vertices(), order_file_path);
        std::cout << "expected order:";
        for (const auto id : expected_topological_order)
            std::cout << " " << id;
        std::cout << std::endl;

        const auto topological_order_opt = lib::algorithm::topological_sort(sut);

        REQUIRE(topological_order_opt.has_value());
        std::cout << "actual order:";
        for (const auto id : topological_order_opt.value())
            std::cout << " " << id;
        std::cout << std::endl;

        CHECK(std::ranges::equal(topological_order_opt.value(), expected_topological_order));

        std::cout << "---------------" << std::endl;
    }

    SUBCASE("not acyclic graph") {
        std::cout << "---------------" << std::endl;

        std::cout << "cwd = " << fs::current_path() << std::endl;
        std::cout << "data_path = " << data_path << std::endl;

        const fs::path gsf_file_path =
            data_path / "topological_sort_directed_not_acyclic_graph.gsf";
        const auto sut = lib::io::load<sut_type>(gsf_file_path);
        std::cout << sut << std::endl;

        const auto topological_order_opt = lib::algorithm::topological_sort(sut);

        CHECK_FALSE(topological_order_opt.has_value());

        std::cout << "---------------" << std::endl;
    }
}

TEST_CASE_TEMPLATE_INSTANTIATE(directed_traits_type_template, lib::list_graph_traits<lib::directed_t>, lib::matrix_graph_traits<lib::directed_t>);

TEST_SUITE_END(); // test_alg_topological_sort

} // namespace gl_testing
