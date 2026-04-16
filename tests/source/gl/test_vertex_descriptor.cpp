#include "doctest.h"
#include "testing/common/functional.hpp"
#include "testing/gl/constants.hpp"
#include "testing/gl/types.hpp"

#include <gl/vertex_descriptor.hpp>

namespace gl_testing {

TEST_SUITE_BEGIN("test_vertex_descriptor");

TEST_CASE("id() should return the correct vertex id") {
    const gl::vertex_descriptor sut{constants::v1_id};
    CHECK_EQ(sut.id(), constants::v1_id);
}

TEST_CASE("vertex_descriptor objects should be compared by id") {
    const gl::vertex_descriptor v1{constants::v1_id};
    const gl::vertex_descriptor v2{constants::v2_id};

    REQUIRE_NE(v1, v2);
    CHECK_EQ(v1, v1);
    CHECK_EQ(v2, v2);

    CHECK_LE(v1, v1);
    CHECK_LE(v1, v2);
    CHECK_LT(v1, v2);

    CHECK_GE(v2, v2);
    CHECK_GE(v2, v1);
    CHECK_GT(v2, v1);
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

TEST_CASE("operator* should return a reference to the properties") {
    visited_property property{constants::visited};
    const gl::vertex_descriptor<visited_property> sut{constants::v1_id, property};
    CHECK_EQ(&(*sut), &property);
}

TEST_CASE("operator-> should return a pointer to the properties") {
    visited_property property{constants::visited};
    const gl::vertex_descriptor<visited_property> sut{constants::v1_id, property};
    CHECK_EQ(sut.operator->(), &property);
    CHECK_EQ(sut->visited, property.visited);
}

TEST_CASE("accessing properties should throw for an invalid vertex") {
    using sut_type = gl::vertex_descriptor<visited_property>;
    visited_property property{constants::visited};

    // .properties()
    CHECK_THROWS_AS(discard(sut_type::invalid().properties()), std::logic_error);
    CHECK_THROWS_AS(discard(sut_type{gl::invalid_id, property}.properties()), std::logic_error);

    // operator*
    CHECK_THROWS_AS(discard(*sut_type::invalid()), std::logic_error);
    CHECK_THROWS_AS(discard(*sut_type{gl::invalid_id, property}), std::logic_error);

    // operator->
    CHECK_THROWS_AS(discard(sut_type::invalid().operator->()), std::logic_error);
    CHECK_THROWS_AS(discard(sut_type{gl::invalid_id, property}.operator->()), std::logic_error);
}

TEST_SUITE_END(); // test_vertex_descriptor

} // namespace gl_testing
