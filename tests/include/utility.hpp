#pragma once

#include "types.hpp"

#include <gl/edge_descriptor.hpp>
#include <gl/vertex_descriptor.hpp>

#include <memory>

namespace gl_testing::util {

template <lib_tt::c_instantiation_of<lib::vertex_descriptor> VertexType>
[[nodiscard]] inline constexpr std::shared_ptr<VertexType> make_vertex(const lib_t::id_type id) {
    return std::make_shared<VertexType>(id);
}

} // namespace gl_testing::util
