#pragma once

#include "namespaces.hpp"

#include <gl/vertex_descriptor.hpp>

#include <memory>

namespace gl_testing::transforms {

template <lib_tt::c_instantiation_of<lib::vertex_descriptor> Vertextype = lib::vertex_descriptor<>>
inline lib_t::id_type extract_vertex_id(const std::shared_ptr<Vertextype>& vertex) {
    return vertex->id();
}

} // namespace gl_testing::transforms
