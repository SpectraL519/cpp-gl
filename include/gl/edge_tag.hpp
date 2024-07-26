#pragma once

#include "detail/concepts.hpp"
#include "vertex_descriptor.hpp"

namespace gl {

struct directed_t {};

struct undirected_t {};

namespace detail {

template <typename T>
concept c_edge_tag = c_one_of<T, directed_t, undirected_t>;

} // namespace detail

} // namespace gl
