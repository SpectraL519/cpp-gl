// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"

#include <type_traits>

namespace gl::util {

template <typename Enum>
requires(std::is_enum_v<Enum>)
[[nodiscard]] gl_attr_force_inline constexpr std::underlying_type_t<Enum> to_underlying(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

} // namespace gl::util
