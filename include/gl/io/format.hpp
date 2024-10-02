// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/types/traits/concepts.hpp"

namespace gl::io {

/*
Custom format functions (casts to types compatible with std::formatter)
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
