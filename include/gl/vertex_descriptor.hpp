#pragma once

#include "detail/concepts.hpp"
#include "detail/default_types.hpp"

#include <compare>

namespace gl {

/*
TODO: define the properties member only when Properties is not void
Possible solutions:
- Template specialization
- std::enable_if_t ?
Requires:
- Modification of the c_properties concept to be a valid type or void
*/

template <detail::c_properties Properties = detail::empty_properties>
class vertex_descriptor {
public:
    using properties_type = Properties;

    vertex_descriptor() = delete;

    vertex_descriptor(const std::size_t id) : _id(id) {}

    vertex_descriptor(const std::size_t id, const properties_type& properties)
    requires(not std::is_same_v<properties_type, detail::empty_properties>)
    : _id(id), properties(properties) {}

    vertex_descriptor(const vertex_descriptor&) = default;
    vertex_descriptor(vertex_descriptor&&) = default;

    vertex_descriptor& operator=(const vertex_descriptor&) = default;
    vertex_descriptor& operator=(vertex_descriptor&&) = default;

    ~vertex_descriptor() = default;

    bool operator==(const vertex_descriptor& other) const {
        return this->_id == other._id;
    }

    std::strong_ordering operator<=>(const vertex_descriptor& other) const {
        return this->_id <=> other._id;
    }

    [[nodiscard]] inline std::size_t id() const {
        return this->_id;
    }

    properties_type properties = properties_type{};

private:
    std::size_t _id;
};

template <detail::c_properties Properties = detail::empty_properties>
using vertex = vertex_descriptor<Properties>;

} // namespace gl
