#pragma once

#include "namespaces.hpp"

#include <gl/types/types.hpp>

#include <ranges>

namespace gl_testing::constants {

constexpr lib_t::size_type n_elements = 3ull;
constexpr lib_t::size_type zero_elements = 0ull;
constexpr lib_t::size_type one_element = 1ull;

constexpr lib_t::size_type first_element_idx = 0ull;
constexpr lib_t::size_type last_element_idx = n_elements - one_element;

constexpr lib_t::id_type vertex_id_1 = first_element_idx;
constexpr lib_t::id_type vertex_id_2 = vertex_id_1 + one_element;
constexpr lib_t::id_type vertex_id_3 = vertex_id_2 + one_element;

constexpr auto vertex_id_view = std::views::iota(first_element_idx, n_elements);

} // namespace gl_testing::constants
