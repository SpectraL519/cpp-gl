#pragma once

#include "namespaces.hpp"

#include <gl/vertex_descriptor.hpp>

#include <memory>

namespace gl_testing::transforms {

template <lib_tt::c_instantiation_of<lib::vertex_descriptor> Vertextype = lib::vertex_descriptor<>>
inline lib_t::id_type extract_vertex_id(const lib_t::vertex_ptr_type<Vertextype>& vertex) {
    return vertex->id();
}

template <typename T>
struct address_projection {
    auto operator()(const std::unique_ptr<T>& ptr) const {
        return ptr.get();
    }

    auto operator()(const std::shared_ptr<T>& ptr) const {
        return ptr.get();
    }
};

} // namespace gl_testing::transforms
