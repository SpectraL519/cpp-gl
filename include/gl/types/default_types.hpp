#pragma once

#include "traits/concepts.hpp"

#include <variant>

namespace gl {

namespace types {

using empty_properties = std::monostate;

} // namespace types

namespace types::traits {

template <c_properties Properties>
constexpr inline bool is_default_properties_type_v =
    std::is_same_v<Properties, gl::types::empty_properties>;

} // namespace types::traits

} // namespace gl
