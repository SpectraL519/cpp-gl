// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/types/types.hpp"

#include <algorithm>
#include <tuple>

namespace gl::util {

// exponentation function for u64 integral type
[[nodiscard]] inline constexpr types::size_type upow(types::size_type base, types::size_type exp) {
    types::size_type result = constants::one;
    while (exp) {
        if (exp % constants::two == constants::one)
            result *= base;
        base *= base;
        exp /= constants::two;
    }

    return result;
}

// sum of exponents: base ^ i_begin + base ^ (i_begin + 1) + ... + base ^ (i_end)
[[nodiscard]] inline types::size_type upow_sum(
    const types::size_type base, types::size_type i_begin, types::size_type i_end
) {
    std::tie(i_begin, i_end) = std::minmax(i_begin, i_end);

    if (base == constants::zero)
        return static_cast<types::size_type>(i_begin == constants::zero);

    if (base == constants::one)
        return i_end - i_begin + constants::one;

    return (upow(base, i_end + constants::one) - upow(base, i_begin)) / (base - constants::one);
}

} // namespace gl::util
