// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <cstdint>

namespace hgl::impl {

enum class bf_incidence : std::uint8_t {
    none = 0, // 00
    backward = 1 << 0, // 01: Tail
    forward = 1 << 1, // 10: Head
    both = backward | forward // 11
};

[[nodiscard]] constexpr bool bf_is_incident(const bf_incidence i) noexcept {
    return i != bf_incidence::none;
}

[[nodiscard]] constexpr bool bf_is_tail(const bf_incidence i) noexcept {
    return (std::to_underlying(i) & std::to_underlying(bf_incidence::backward)) != 0;
}

[[nodiscard]] constexpr bool bf_is_head(const bf_incidence i) noexcept {
    return (std::to_underlying(i) & std::to_underlying(bf_incidence::forward)) != 0;
}

} // namespace hgl::impl
