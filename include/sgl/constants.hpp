#pragma once

#include "gl/constants.hpp"
#include "sgl/traits.hpp"

#include <cstddef>
#include <limits>

namespace sgl {

using gl::initial_id;
using gl::initial_id_t;
using gl::initial_id_v;
using gl::invalid_id;
using gl::invalid_id_t;
using gl::invalid_id_v;

inline constexpr std::size_t default_sketch_width = 64uz;

inline constexpr std::size_t cache_line_size = 64uz;

using traits::min_hash_word_size;

template <traits::c_weight_type WeightType>
inline constexpr WeightType sketch_infinity_v = std::numeric_limits<WeightType>::infinity();

[[nodiscard]] constexpr std::size_t align_sketch_stride(
    const std::size_t m, const std::size_t slot_size
) noexcept {
    if (m == 0uz or slot_size == 0uz)
        return m;

    const auto slots_per_line = cache_line_size / slot_size;
    if (slots_per_line == 0uz)
        return m;

    return ((m + slots_per_line - 1uz) / slots_per_line) * slots_per_line;
}

} // namespace sgl
