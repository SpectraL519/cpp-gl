#pragma once

#include <gl/vertex_descriptor.hpp>

#include <memory>

namespace gl_testing::transforms {

inline gl::types::id_type get_id(auto&& element) {
    return element.id();
}

// TODO: remove
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
