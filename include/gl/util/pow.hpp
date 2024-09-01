#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/types/types.hpp"

#include <algorithm>
#include <tuple>

namespace gl::util {

[[nodiscard]] inline constexpr types::size_type upow(types::size_type base, types::size_type exp) {
    constexpr types::size_type one = 1ull;
    constexpr types::size_type two = 2ull;

    types::size_type result = one;
    while (exp) {
        if (exp % two == one)
            result *= base;
        base *= base;
        exp /= two;
    }

    return result;
}

[[nodiscard]] inline types::size_type upow_sum(
    const types::size_type base, types::size_type i_begin, types::size_type i_end
) {
    constexpr types::size_type zero = 0ull;
    constexpr types::size_type one = 1ull;

    std::tie(i_begin, i_end) = std::minmax(i_begin, i_end);

    if (base == zero)
        return static_cast<types::size_type>(i_begin == zero);

    if (base == one)
        return i_end - i_begin + one;

    return (upow(base, i_end + one) - upow(base, i_begin)) / (base - one);
}

} // namespace gl::util
