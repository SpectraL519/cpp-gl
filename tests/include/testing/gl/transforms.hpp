#pragma once

#include <gl/vertex_descriptor.hpp>

#include <memory>

namespace gl_testing::transforms {

template <
    gl::type_traits::c_instantiation_of<gl::vertex_descriptor> VertexType = gl::vertex_descriptor<>>
inline gl::types::id_type extract_vertex_id(const VertexType& vertex) {
    return vertex.id();
}

template <typename T>
struct address_projection {
    auto operator()(const T& ref) const {
        return &ref;
    }

    auto operator()(const std::unique_ptr<T>& ptr) const {
        return ptr.get();
    }

    auto operator()(const std::shared_ptr<T>& ptr) const {
        return ptr.get();
    }
};

} // namespace gl_testing::transforms
