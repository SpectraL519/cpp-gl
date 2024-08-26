#pragma once

#include "gl/attributes/force_inline.hpp"

#include <memory>
#include <type_traits>

namespace gl::types {

template <typename T>
[[nodiscard]] gl_attr_force_inline void* formatter(T* ptr) {
    // std::format is not compatible with all types of ptrs
    return static_cast<void*>(ptr);
}

template <type_traits::c_strong_smart_ptr PtrType>
[[nodiscard]] gl_attr_force_inline void* formatter(const PtrType& ptr) {
    return formatter(ptr.get());
}

} // namespace gl::types
