#pragma once

#include "doctest.h"

#include <concepts>
#include <ranges>
#include <sstream>
#include <string_view>

namespace doctest {

template <std::ranges::input_range R>
requires(not std::convertible_to<R, std::string_view>)
struct StringMaker<R> {
    static String convert(const R& rng) {
        std::ostringstream oss;
        oss << "[ ";
        for (const auto& v : rng)
            oss << v << " ";
        oss << "]";
        return oss.str().c_str();
    }
};

} // namespace doctest
