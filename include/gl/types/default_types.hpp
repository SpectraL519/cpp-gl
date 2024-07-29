#pragma once

#include "gl/detail/concepts.hpp"

namespace gl {

namespace types {

struct empty_properties {};

} // namespace types

namespace detail {

template <c_properties Properties>
constexpr inline bool is_default_properties_type_v =
    std::is_same_v<Properties, gl::types::empty_properties>;

} // namespace detail

} // namespace gl
