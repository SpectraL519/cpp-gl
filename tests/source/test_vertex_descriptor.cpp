#include "common_constants.hpp"
#include "types.hpp"

#include <doctest.h>
#include <gl/vertex_descriptor.hpp>

using namespace gl;

namespace gl_testing {

TEST_SUITE_BEGIN("test_vertex_descriptor");

TEST_CASE("id() should return the correct vertex id") {
    const vertex_descriptor sut{vertex_id_1};
    CHECK_EQ(sut.id(), vertex_id_1);
}

TEST_CASE("properties should be properly initialized") {
    const vertex_types::visited_property visited{true};
    const vertex_descriptor<vertex_types::visited_property> sut{vertex_id_1, visited};

    CHECK_EQ(sut.properties, visited);
}

TEST_CASE("vertex_descriptor objects should be compared by id") {
    const vertex_descriptor vd_1{vertex_id_1};
    const vertex_descriptor vd_2{vertex_id_2};

    REQUIRE_NE(vd_1, vd_2);
    CHECK_EQ(vd_1, vd_1);
    CHECK_EQ(vd_2, vd_2);

    CHECK_LE(vd_1, vd_1);
    CHECK_LE(vd_1, vd_2);
    CHECK_LT(vd_1, vd_2);

    CHECK_GE(vd_2, vd_2);
    CHECK_GE(vd_2, vd_1);
    CHECK_GT(vd_2, vd_1);
}

TEST_SUITE_END(); // test_vertex_descriptor

} // namespace gl_testing
