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
