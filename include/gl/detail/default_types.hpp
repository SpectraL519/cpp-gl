#pragma once

#include "concepts.hpp"

namespace gl::detail {

struct empty_properties {};

template <c_properties Properties>
constexpr inline bool is_default_properties_type_v = std::is_same_v<Properties, empty_properties>;

} // namespace gl::detail
