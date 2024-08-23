#pragma once

#include <type_traits>

namespace gl::types {

template <typename T>
[[nodiscard]] inline void* formatter(T* ptr) {
    // std::format is not compatible with all types of ptrs
    return static_cast<void*>(ptr);
}

} // namespace gl::types
