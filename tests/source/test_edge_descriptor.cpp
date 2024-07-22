#include <doctest.h>
#include <gl/edge_descriptor.hpp>

using namespace gl;

namespace {

constexpr std::size_t vertex_id_1 = 1234ull;
constexpr std::size_t vertex_id_2 = 4321ull;

} // namespace

namespace gl_testing {

TEST_SUITE_BEGIN("test_edge_descriptor");

TEST_CASE("vertices() should return the pair of vertices the edge was initialized with") {
    auto vd_1 = std::make_shared<vertex<>>(vertex_id_1);
    auto vd_2 = std::make_shared<vertex<>>(vertex_id_2);

    edge_descriptor<vertex<>> sut{vd_1, vd_2};

    const auto& vertices = sut.vertices();
    CHECK_EQ(*vertices.first, *vd_1);
    CHECK_EQ(*vertices.second, *vd_2);
}

TEST_SUITE_END(); // test_edge_descriptor

} // namespace gl_testing
