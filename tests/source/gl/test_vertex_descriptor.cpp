#include "testing/gl/constants.hpp"
#include "testing/gl/functional.hpp"
#include "testing/gl/types.hpp"

#include <gl/vertex_descriptor.hpp>

#include <doctest.h>

namespace gl_testing {

TEST_SUITE_BEGIN("test_vertex_descriptor");

TEST_CASE("id() should return the correct vertex id") {
    const gl::vertex_descriptor sut{constants::v1_id};
    CHECK_EQ(sut.id(), constants::v1_id);
}

TEST_CASE("vertex_descriptor objects should be compared by id") {
    const gl::vertex_descriptor vd_1{constants::v1_id};
    const gl::vertex_descriptor vd_2{constants::v2_id};

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

TEST_CASE("vertex_descriptor should be valid only if it has a valid id") {
    CHECK(gl::vertex_descriptor{constants::v1_id}.is_valid());

    CHECK_FALSE(gl::vertex_descriptor<>::invalid().is_valid());
    CHECK_FALSE(gl::vertex_descriptor{gl::invalid_id_v<gl::default_id_type>}.is_valid());
}

TEST_CASE("properties should be properly initialized") {
    visited_property property{constants::visited};

    const gl::vertex_descriptor<visited_property> sut{constants::v1_id, property};
    CHECK_EQ(&sut.properties(), &property);
}

TEST_CASE("accessing properties should throw for an invalid vertex") {
    using sut_type = gl::vertex_descriptor<visited_property>;
    visited_property property{constants::visited};

    CHECK_THROWS_AS(discard_result(sut_type::invalid().properties()), std::logic_error);
    CHECK_THROWS_AS(
        discard_result(sut_type{gl::invalid_id, property}.properties()), std::logic_error
    );
}

TEST_SUITE_END(); // test_vertex_descriptor

} // namespace gl_testing
