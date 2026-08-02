#pragma once

#include "gl/traits.hpp"

#include <concepts>
#include <cstddef>

namespace sgl {

namespace traits {

inline constexpr std::size_t min_hash_word_size = 8uz;

using gl::traits::c_arithmetic;
using gl::traits::c_forward_range;
using gl::traits::c_forward_range_of;
using gl::traits::c_id_type;
using gl::traits::c_instantiation_of;
using gl::traits::c_one_of;
using gl::traits::c_range;
using gl::traits::c_sized_range;
using gl::traits::c_sized_range_of;
using gl::traits::is_instantiation_of;
using gl::traits::is_instantiation_of_v;

template <typename T>
concept c_weight_type = c_arithmetic<T> and std::floating_point<T>;

template <typename T>
concept c_hash_word_type =
    std::unsigned_integral<T> and (sizeof(T) >= min_hash_word_size);

template <typename T>
concept c_uniform_type = std::floating_point<T>;

} // namespace traits

} // namespace sgl
