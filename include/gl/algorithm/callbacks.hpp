#pragma once

#include <functional>

#include "gl/graph/graph_traits.hpp"



namespace gl {

template <typename graph_t>
using vertex_callback = std::function<void(const typename graph_t::vertex_type&)>;

template <typename graph_t>
using vertex_ptr_type_callback = std::function<void(const typename graph_t::vertex_ptr_type&)>;

} // namespace gl
