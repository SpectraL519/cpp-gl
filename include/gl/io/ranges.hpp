// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/io/ranges.hpp
/// @brief Utilities and manipulators for formatting and printing C++ ranges.

#pragma once

#include <iostream>
#include <ranges>

namespace gl::io {

/// @ingroup GL GL-IO
/// @brief A customizable stream proxy for formatting and printing C++ ranges.
///
/// This struct acts as a wrapper around any `std::ranges::range`, allowing it to be
/// piped into a standard output stream with configurable separators and bounding characters.
///
/// ### Example Usage
/// ```cpp
/// std::vector<int> vec = {1, 2, 3};
/// std::cout << gl::io::range_formatter{vec} << '\n';
/// std::cout << gl::io::range_formatter{vec, " | ", "<", ">"} << '\n';
/// ```
///
/// **Output:**
/// ```text
/// [1, 2, 3]
/// <1 | 2 | 3>
/// ```
///
/// ### See Also
/// - @ref gl::io::set_formatter "set_formatter"
/// - @ref gl::io::multiline_set_formatter "multiline_set_formatter"
///
/// @tparam R The specific type of the range being formatted.
template <std::ranges::range R>
struct range_formatter {
    /// @brief The underlying range view being formatted.
    R range;
    /// @brief The string separating each element.
    std::string_view sep = ", ";
    /// @brief The opening string/bracket.
    std::string_view open = "[";
    /// @brief The closing string/bracket.
    std::string_view close = "]";

    /// @brief Serializes the formatted range to an output stream.
    /// @param os The target output stream.
    /// @param formatter The configured formatter instance.
    /// @return The stream reference for chaining.
    friend std::ostream& operator<<(std::ostream& os, range_formatter formatter) {
        os << formatter.open;
        bool first = true;
        for (const auto& item : formatter.range) {
            if (not first)
                os << formatter.sep;
            os << item;
            first = false;
        }
        os << formatter.close;
        return os;
    }
};

// CTAD helpers

/// @internal
template <std::ranges::range R>
range_formatter(R&& r) -> range_formatter<std::views::all_t<R>>;

/// @internal
template <std::ranges::range R>
range_formatter(R&& r, std::string_view) -> range_formatter<std::views::all_t<R>>;

/// @internal
template <std::ranges::range R>
range_formatter(R&& r, std::string_view, std::string_view) -> range_formatter<std::views::all_t<R>>;

/// @internal
template <std::ranges::range R>
range_formatter(R&& r, std::string_view, std::string_view, std::string_view)
    -> range_formatter<std::views::all_t<R>>;

/// @ingroup GL GL-IO
/// @brief Factory function to format a range as a set.
///
/// Wraps the range output in curly braces `{}` instead of standard brackets `[]`.
///
/// ### Usage example
/// ```cpp
/// std::vector<int> vec = {1, 2, 3};
/// std::cout << gl::io::set_formatter(vec) << '\n';
/// ```
///
/// **Output**:
/// ```text
/// {1, 2, 3}
/// ```
///
/// @tparam R The type of the range.
/// @param range The range object to format.
/// @param sep The separator string between elements. Defaults to `", "`.
/// @return A @ref range_formatter configured for set-style output.
/// ### See Also
/// - @ref gl::io::multiline_set_formatter "multiline_set_formatter"
template <std::ranges::range R>
auto set_formatter(R&& range, std::string_view sep = ", ") {
    using view_type = std::views::all_t<R>;
    return range_formatter<view_type>{std::views::all(std::forward<R>(range)), sep, "{", "}"};
}

/// @ingroup GL GL-IO
/// @brief Factory function to format a range as a multiline set.
///
/// Outputs each element on a new line, bounded by curly braces `{}`.
///
/// ### Usage example
/// ```cpp
/// std::vector<std::string_view> fruits = {"banana", "apple", "cherry"};
/// std::cout << "fruits: " << gl::io::set_formatter(vec) << '\n';
/// ```
///
/// **Output**:
/// ```text
/// fruits: {
///   banana,
///   apple,
///   cherry
/// }
/// ```
///
/// @todo Add an indent_width parameter
///
/// @tparam R The type of the range.
/// @param range The range object to format.
/// @return A @ref range_formatter configured for multiline set-style output.
/// ### See Also
/// - @ref gl::io::set_formatter "set_formatter"
template <std::ranges::range R>
auto multiline_set_formatter(R&& range) {
    using view_type = std::views::all_t<R>;
    return range_formatter<view_type>{
        std::views::all(std::forward<R>(range)), ",\n  ", "{\n  ", "\n}"
    };
}

/// @ingroup GL GL-IO
/// @brief A proxy object for concisely formatting large contiguous numeric ranges.
///
/// Instead of iterating and printing every element of a massive sequence, this formatter
/// intelligently truncates the output using an ellipsis (e.g., `{0, ..., 99}`).
///
/// ### Example Usage
/// ```cpp
/// std::cout << gl::io::implicit_range_formatter<int>{0, 100} << '\n';
/// std::cout << gl::io::implicit_range_formatter<int>{5, 8} << '\n';
/// ```
///
/// **Output:**
/// ```text
/// {0, ..., 99}
/// {5, ..., 7}
/// ```
///
/// ### See Also
/// - [**implicit_range(first, last, inclusive)**](group__GL-IO.md#function-implicit_range)
/// - [**implicit_range(last, inclusive)**](group__GL-IO.md#function-implicit_range_1)
///
/// @tparam T The integral numeric type of the range bounds.
template <std::integral T>
struct implicit_range_formatter {
    /// @brief The starting bound of the range.
    T first;
    /// @brief The exclusive ending bound of the range.
    T last; // exclusive

    /// @brief Serializes the implicit range to an output stream.
    /// @param os The target output stream.
    /// @param proxy The implicit range proxy.
    /// @return The stream reference for chaining.
    friend std::ostream& operator<<(std::ostream& os, implicit_range_formatter proxy) {
        if (proxy.first >= proxy.last)
            return os << "{}";

        const auto dist = proxy.last - proxy.first;
        if (dist == 1)
            return os << '{' << proxy.first << '}';
        if (dist == 2)
            return os << '{' << proxy.first << ", " << proxy.first + 1 << '}';

        return os << '{' << proxy.first << ", ..., " << proxy.last - 1 << '}';
    }
};

/// @ingroup GL GL-IO
/// @brief Factory function to create an implicit range formatter with a defined start and end.
///
/// ### Example Usage
/// ```cpp
/// std::cout << gl::io::implicit_range(10, 100) << '\n';
/// std::cout << gl::io::implicit_range(10, 100, true) << '\n';
/// ```
///
/// **Output:**
/// ```text
/// {10, ..., 99}
/// {10, ..., 100}
/// ```
///
/// ### See Also
/// - @ref gl::io::implicit_range_formatter "implicit_range_formatter"
/// - [**implicit_range(last, inclusive)**](group__GL-IO.md#function-implicit_range_1)
///
/// @tparam T The integral numeric type of the range.
/// @param first The starting value of the range.
/// @param last The ending value of the range.
/// @param inclusive Set to `true` if `last` should be included in the bounds, `false` otherwise.
/// @return An @ref implicit_range_formatter representing the bounds.
template <std::integral T>
[[nodiscard]] constexpr implicit_range_formatter<T> implicit_range(
    T first, T last, bool inclusive = false
) {
    return implicit_range_formatter<T>{first, last + static_cast<T>(inclusive)};
}

/// @ingroup GL GL-IO
/// @brief Factory function to create an implicit range formatter starting from 0.
///
/// ### Example Usage
/// ```cpp
/// std::cout << gl::io::implicit_range(100) << '\n';
/// std::cout << gl::io::implicit_range(100, true) << '\n';
/// ```
///
/// **Output:**
/// ```text
/// {0, ..., 99}
/// {0, ..., 100}
/// ```
///
/// ### See Also
/// - @ref gl::io::implicit_range_formatter "implicit_range_formatter"
/// - [**implicit_range(first, last, inclusive)**](group__GL-IO.md#function-implicit_range)
///
/// @tparam T The integral numeric type of the range.
/// @param last The ending value of the range.
/// @param inclusive Set to `true` if `last` should be included in the bounds, `false` otherwise.
/// @return An @ref implicit_range_formatter representing bounds from `0` to `last`.
template <std::integral T>
[[nodiscard]] constexpr implicit_range_formatter<T> implicit_range(T last, bool inclusive = false) {
    return implicit_range(static_cast<T>(0), last, inclusive);
}

} // namespace gl::io
