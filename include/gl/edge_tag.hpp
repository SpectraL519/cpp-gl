#pragma once

#include "detail/concepts.hpp"

namespace gl {

struct bidirectional {};

struct directed {};

namespace detail {

template <typename T>
concept c_edge_tag = c_one_of<T, bidirectional, directed>;

} // namespace detail

} // namespace gl
