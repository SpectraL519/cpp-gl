#include "hgl/hypergraph.hpp"

#include <doctest.h>

namespace hgl_testing {

TEST_SUITE_BEGIN("test_hypergraph");

struct test_hypergraph {
    static constexpr hgl::types::id_type id1 = 0ull;
    static constexpr hgl::types::id_type id2 = 1ull;
};

TEST_CASE_FIXTURE(test_hypergraph, "dummy") {
    hgl::hypergraph<> sut{};
    CHECK(true);
}

TEST_SUITE_END(); // test_hypergraph

} // namespace hgl_testing
