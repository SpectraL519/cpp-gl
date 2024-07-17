#pragma once

#include <concepts>

namespace gl::detail {

template <typename T>
concept c_properties =
    std::default_initializable<T> and std::copy_constructible<T> and std::move_constructible<T>
    and std::assignable_from<T&, const T&>;

} // namespace gl::detail
