// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <cstdint>

namespace hgl::impl {

enum class bf_incidence : std::int8_t {
    none = 0, // v not in E
    backward = -1, // v in Tail(E)
    forward = 1, // v in Head(E)
};

[[nodiscard]] constexpr bool bf_is_incident(const bf_incidence i) noexcept {
    return i != bf_incidence::none;
}

[[nodiscard]] constexpr bool bf_is_tail(const bf_incidence i) noexcept {
    return i == bf_incidence::backward;
}

[[nodiscard]] constexpr bool bf_is_head(const bf_incidence i) noexcept {
    return i == bf_incidence::forward;
}

} // namespace hgl::impl
