#include "constants.hpp"
#include "types.hpp"

#include <gl/vertex_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_vertex_descriptor");

TEST_CASE("id() should return the correct vertex id") {
    const lib::vertex_descriptor sut{constants::vertex_id_1};
    CHECK_EQ(sut.id(), constants::vertex_id_1);
}

TEST_CASE("properties should be properly initialized") {
    const lib::vertex_descriptor<types::visited_property> sut{constants::vertex_id_1, constants::visited};
    CHECK_EQ(sut.properties, constants::visited);
}

TEST_CASE("vertex_descriptor objects should be compared by id") {
    const lib::vertex_descriptor vd_1{constants::vertex_id_1};
    const lib::vertex_descriptor vd_2{constants::vertex_id_2};

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
