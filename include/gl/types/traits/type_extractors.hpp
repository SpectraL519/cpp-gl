#pragma once

#include "concepts.hpp"

namespace gl::type_traits {

template <typename T>
struct ptr_element_type;

template <c_strong_smart_ptr T>
struct ptr_element_type<T> {
    using type = typename T::element_type;
};

template <typename T>
struct ptr_element_type<T*> {
    using type = T;
};

template <typename T>
using ptr_element_type_t = typename ptr_element_type<T>::type;

} // namespace gl::type_traits
