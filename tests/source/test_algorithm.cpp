#include "constants.hpp"

#include <gl/algorithms.hpp>
#include <gl/topologies.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_algorithm");

TEST_CASE("breadth_virst_search") {
    using graph_type = lib::graph<>;
    const auto graph = lib::topology::full_bipartite<graph_type>(constants::three, constants::two);

    lib::algorithm::breadth_first_search<lib::algorithm::no_return>(graph);
}

TEST_SUITE_END(); // test_algorithm

} // namespace gl_testing
