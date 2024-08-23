#pragma once

#include "gl/attributes/force_inline.hpp"

#include <type_traits>

namespace gl::types {

template <typename T>
[[nodiscard]] gl_attr_force_inline void* formatter(T* ptr) {
    // std::format is not compatible with all types of ptrs
    return static_cast<void*>(ptr);
}

} // namespace gl::types
