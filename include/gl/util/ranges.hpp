// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <ranges>

namespace gl::util {

inline constexpr auto deref_view =
    std::views::transform([](auto&& p) -> decltype(auto) { return *p; });

template <std::ranges::range R>
constexpr auto range_size(R&& r) {
    if constexpr (std::ranges::sized_range<R>)
        return std::ranges::size(r);
    else
        // Will consume input ranges!
        return std::ranges::distance(std::begin(r), std::end(r));
}

} // namespace gl::util
