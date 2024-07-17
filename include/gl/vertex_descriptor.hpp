#pragma once

#include "detail/concepts.hpp"

namespace gl {

template <detail::c_properties Properties>
class vertex_descriptor {
public:
    using properties_type = Properties;

    vertex_descriptor() = delete;

    vertex_descriptor(const std::size_t id) : _id(id) {}

    vertex_descriptor(const std::size_t id, const properties_type& properties)
    : _list_id(id), _properties(properties) {}

    vertex_descriptor(const vertex_descriptor&) = default;
    vertex_descriptor(vertex_descriptor&&) = default;

    vertex_descriptor& operator=(const vertex_descriptor&) = default;
    vertex_descriptor& operator=(vertex_descriptor&&) = default;

    ~vertex_descriptor() = default;

    [[nodiscard]] inline std::size_t id() const {
        return this->_id;
    }

    properties_type properties = properties_type{};

private:
    std::size_t _id;
};

} // namespace gl
