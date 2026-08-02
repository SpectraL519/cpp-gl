#pragma once

#include "gl/directional_tags.hpp"
#include "sgl/traits.hpp"

namespace sgl {

using directed_t = gl::directed_t;

using undirected_t = gl::undirected_t;

namespace traits {

template <typename T>
concept c_sketch_directional_tag = c_one_of<T, directed_t, undirected_t>;

using gl::traits::c_graph_directional_tag;

} // namespace traits

} // namespace sgl
