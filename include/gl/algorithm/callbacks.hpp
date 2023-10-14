#pragma once

#include "gl/graph/graph_traits.hpp"

#include <functional>


namespace gl {

template <typename Graph>
using vertex_callback = std::function<void(const typename Graph::vertex_type&)>;

template <typename Graph>
using vertex_ptr_type_callback =
    std::function<void(const typename Graph::vertex_ptr_type&)>;

}  // namespace gl
