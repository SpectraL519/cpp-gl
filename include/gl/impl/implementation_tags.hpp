#pragma once

#include "gl/types/type_traits.hpp"

namespace gl {

namespace impl {

struct list_t;

struct matrix_t;

} // namespace impl

namespace types::traits {

template <typename T>
concept c_graph_impl_tag = c_one_of<T, impl::list_t, impl::matrix_t>;

} // namespace types::traits

struct list_t {};

struct matrix_t {};

} // namespace gl
