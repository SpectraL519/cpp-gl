// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/traits.hpp"

#include <iostream>
#include <ranges>

namespace gl::io {

// TODO: add tests
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

template <std::ranges::range R>
range_formatter(R&& r) -> range_formatter<std::views::all_t<R>>;

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

} // namespace gl::io
