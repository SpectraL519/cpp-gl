#pragma once

#include "doctest.h"

#include <concepts>
#include <filesystem>
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

namespace fs = std::filesystem;

#define GL_REQUIRE_THROWS_FS_ERROR(expr, errc)                                                 \
    try {                                                                                      \
        expr;                                                                                  \
        FAIL("Expected `std::filesystem::filesystem_error` but no exception was thrown");      \
    }                                                                                          \
    catch (const fs::filesystem_error& e) {                                                    \
        const auto expected_code = std::make_error_code(errc);                                 \
        if (e.code() != expected_code) {                                                       \
            FAIL(std::format(                                                                  \
                "Expected error code {}, but got {}.", expected_code.value(), e.code().value() \
            ));                                                                                \
        }                                                                                      \
    }                                                                                          \
    catch (...) {                                                                              \
        FAIL("Expected `std::filesystem::filesystem_error` but caught a different exception"); \
    }
