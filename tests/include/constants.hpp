#pragma once

#include "namespaces.hpp"

#include <gl/types/types.hpp>

#include <ranges>

namespace gl_testing::constants {

constexpr lib_t::size_type zero_vertices = 0ull;
constexpr lib_t::size_type one_vertex = 1ull;
constexpr lib_t::size_type no_vertices = 3ull;

constexpr lib_t::id_type vertex_id_1 = 0ull;
constexpr lib_t::id_type vertex_id_2 = 1ull;
constexpr lib_t::id_type vertex_id_3 = 2ull;

constexpr auto vertex_id_view = std::views::iota(constants::vertex_id_1, constants::no_vertices);

constexpr lib_t::size_type zero_edges = 0ull;
constexpr lib_t::size_type one_edge = 1ull;
constexpr lib_t::size_type no_edges = 3ull;

} // namespace gl_testing::constants
