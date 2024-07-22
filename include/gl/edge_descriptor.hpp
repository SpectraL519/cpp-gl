#pragma once

#include "detail/concepts.hpp"
#include "detail/default_types.hpp"
#include "edge_tag.hpp"
#include "vertex_descriptor.hpp"

#include <memory>
#include <type_traits>

namespace gl {

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_tag EdgeTag = bidirectional,
    detail::c_properties Properties = detail::empty_properties>
class edge_descriptor {
public:
    using vertex_type = VertexType;
    using vertex_ptr_type = std::shared_ptr<vertex_type>;
    using edge_tag = EdgeTag;
    using properties_type = Properties;

    edge_descriptor() = delete;

    edge_descriptor(vertex_ptr_type u, vertex_ptr_type v) : _vertices(u, v) {}

    edge_descriptor(vertex_ptr_type u, vertex_ptr_type v, const properties_type& properties)
    requires(not std::is_same_v<properties_type, detail::empty_properties>)
    : _vertices(u, v), properties(properties) {}

    edge_descriptor(const edge_descriptor&) = default;
    edge_descriptor(edge_descriptor&&) = default;

    edge_descriptor& operator=(const edge_descriptor&) = default;
    edge_descriptor& operator=(edge_descriptor&&) = default;

    properties_type properties = {};

    [[nodiscard]] inline const std::pair<vertex_ptr_type, vertex_ptr_type>& vertices() const {
        return this->_vertices;
    }

    [[nodiscard]] inline bool is_directed() const {
        return std::is_same_v<edge_tag, directed>;
    }

    [[nodiscard]] inline bool is_bidirectional() const {
        return std::is_same_v<edge_tag, bidirectional>;
    }

private:
    std::pair<vertex_ptr_type, vertex_ptr_type> _vertices;
};

} // namespace gl
