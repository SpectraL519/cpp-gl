#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/types/traits/concepts.hpp"

#include <memory>
#include <type_traits>

namespace gl::io {

/*
Custom format functions
Not std::formatter overloads to avoid collision with
    user defined std::formatter overloads
*/

template <typename T>
[[nodiscard]] gl_attr_force_inline void* format(T* ptr) {
    // std::format is not compatible with all types of ptrs
    return static_cast<void*>(ptr);
}

// clang-format off
// gl_attr_force_inline misplacement

template <typename T>
[[nodiscard]] gl_attr_force_inline const void* format(const T* ptr) {
    // std::format is not compatible with all types format ptrs
    return static_cast<const void*>(ptr);
}

// clang-format on

template <type_traits::c_strong_smart_ptr PtrType>
[[nodiscard]] gl_attr_force_inline void* format(const PtrType& ptr) {
    return formatter(ptr.get());
}

} // namespace gl::io
