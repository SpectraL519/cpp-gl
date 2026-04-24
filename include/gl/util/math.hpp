// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/util/math.hpp
/// @brief Defines utility mathematical functions.

#pragma once

#include "gl/types/core.hpp"

#include <algorithm>
#include <tuple>

namespace gl::util {

/// @ingroup GL GL-Util
/// @brief Computes the value of `base` raised to the power of `exp` using exponentiation by squaring.
/// @param base The base value to be raised to a power.
/// @param exp The exponent to which the base is raised.
/// @return The result of `base` raised to the power of `exp`.
///
/// > [!INFO] Time Complexity
/// >
/// > $O(\log(\text{exp}))$ due to halving the exponent at each step.
[[nodiscard]] inline constexpr size_type upow(size_type base, size_type exp) {
    size_type result = 1uz;
    while (exp) {
        if (exp % 2uz == 1uz)
            result *= base;
        base *= base;
        exp /= 2uz;
    }

    return result;
}

/// @ingroup GL GL-Util
/// @brief Computes the sum of powers of `base` from `base^i_begin` to `base^i_end` inclusive.
/// @param base The base value for the powers.
/// @param i_begin The starting exponent (inclusive).
/// @param i_end The ending exponent (inclusive).
/// @return The sum of `base^i` for all `i` in the range `[i_begin, i_end]`.
///
/// > [!INFO] Time Complexity
/// >
/// > $O(\log(\text{max}(\text{i\_begin}, \text{i\_end})))$ due to the use of the closed-form formula for geometric series.
[[nodiscard]] inline constexpr size_type upow_sum(
    const size_type base, size_type i_begin, size_type i_end
) {
    std::tie(i_begin, i_end) = std::minmax(i_begin, i_end);

    if (base == 0uz)
        return static_cast<size_type>(i_begin == 0uz);

    if (base == 1uz)
        return i_end - i_begin + 1uz;

    return (upow(base, i_end + 1uz) - upow(base, i_begin)) / (base - 1uz);
}

} // namespace gl::util
