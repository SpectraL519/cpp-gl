#include <doctest.h>
#include <gl/vertex_descriptor.hpp>

using namespace gl;

namespace {
constexpr std::size_t id = 1234ull;
} // namespace

namespace gl_testing {

TEST_SUITE_BEGIN("test_vertex_descriptor");

TEST_CASE("id() should return the correct vertex id") {
    const vertex_descriptor sut{id};
    CHECK_EQ(sut.id(), id);
}

TEST_CASE("properties should be properly initialized") {
    struct visited_property {
        bool operator==(const visited_property&) const = default;
        bool visited;
    };

    const visited_property visited{true};
    const vertex_descriptor<visited_property> sut{id, visited};

    CHECK_EQ(sut.properties, visited);
}

TEST_SUITE_END(); // test_vertex_descriptor

} // namespace gl_testing
