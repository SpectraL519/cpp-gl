// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <iostream>
#include <ranges>

namespace gl::io {

template <std::ranges::range Range>
struct range_formatter {
    Range range;
    std::string_view sep = ", ";
    std::string_view open = "[";
    std::string_view close = "]";

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

template <std::ranges::range R>
range_formatter(R&& r) -> range_formatter<std::views::all_t<R>>;

template <std::ranges::range R>
range_formatter(R&& r, std::string_view) -> range_formatter<std::views::all_t<R>>;

template <std::ranges::range R>
range_formatter(R&& r, std::string_view, std::string_view) -> range_formatter<std::views::all_t<R>>;

template <std::ranges::range R>
range_formatter(R&& r, std::string_view, std::string_view, std::string_view)
    -> range_formatter<std::views::all_t<R>>;

template <std::ranges::range R>
auto set_formatter(R&& range, std::string_view sep = ", ") {
    using view_type = std::views::all_t<R>;
    return range_formatter<view_type>{std::views::all(std::forward<R>(range)), sep, "{", "}"};
}

/// @todo Add an indent_width parameter
template <std::ranges::range R>
auto multiline_set_formatter(R&& range) {
    using view_type = std::views::all_t<R>;
    return range_formatter<view_type>{
        std::views::all(std::forward<R>(range)), ",\n  ", "{\n  ", "\n}"
    };
}

template <std::integral T>
struct implicit_range_formatter {
    T first;
    T last; // exclusive

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

template <std::integral T>
[[nodiscard]] constexpr auto implicit_range(T first, T last, bool inclusive = false) {
    return implicit_range_formatter<T>{first, last + static_cast<T>(inclusive)};
}

template <std::integral T>
[[nodiscard]] constexpr auto implicit_range(T last, bool inclusive = false) {
    return implicit_range(static_cast<T>(0), last, inclusive);
}

} // namespace gl::io
