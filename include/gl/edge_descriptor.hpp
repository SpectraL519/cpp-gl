#pragma once

#include "detail/concepts.hpp"
#include "edge_tags.hpp"
#include "types/default_types.hpp"
#include "vertex_descriptor.hpp"

#include <memory>
#include <type_traits>
#include <vector>

namespace gl {

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_directional_tag DirectionalTag = directed_t,
    detail::c_properties Properties = types::empty_properties>
class edge_descriptor {
public:
    using type = edge_descriptor<VertexType, DirectionalTag, Properties>;
    using vertex_type = VertexType;
    using vertex_ptr_type = std::shared_ptr<vertex_type>;
    using directional_tag = DirectionalTag;
    using properties_type = Properties;

    friend directional_tag;

    edge_descriptor() = delete;

    explicit edge_descriptor(vertex_ptr_type u, vertex_ptr_type v) : _vertices(u, v) {}

    explicit edge_descriptor(vertex_ptr_type u, vertex_ptr_type v, const properties_type& properties)
    requires(not detail::is_default_properties_type_v<properties_type>)
    : _vertices(u, v), properties(properties) {}

    edge_descriptor(const edge_descriptor&) = default;
    edge_descriptor(edge_descriptor&&) = default;

    edge_descriptor& operator=(const edge_descriptor&) = default;
    edge_descriptor& operator=(edge_descriptor&&) = default;

    ~edge_descriptor() = default;

    [[nodiscard]] inline constexpr bool is_directed() const {
        return std::is_same_v<directional_tag, directed_t>;
    }

    [[nodiscard]] inline constexpr bool is_undirected() const {
        return std::is_same_v<directional_tag, undirected_t>;
    }

    [[nodiscard]] inline const std::pair<vertex_ptr_type, vertex_ptr_type>& incident_vertices(
    ) const {
        return this->_vertices;
    }

    [[nodiscard]] inline vertex_ptr_type first() const {
        return this->_vertices.first;
    }

    [[nodiscard]] inline vertex_ptr_type second() const {
        return this->_vertices.second;
    }

    [[nodiscard]] vertex_ptr_type incident_vertex(const vertex_ptr_type& vertex) const {
        if (*vertex == *this->_vertices.first)
            return this->_vertices.second;

        if (*vertex == *this->_vertices.second)
            return this->_vertices.first;

        return nullptr;
    }

    [[nodiscard]] inline bool is_incident_from(const vertex_ptr_type& vertex) const {
        return directional_tag::is_incident_from(*this, vertex);
    }

    [[nodiscard]] inline bool is_incident_to(const vertex_ptr_type& vertex) const {
        return directional_tag::is_incident_to(*this, vertex);
    }

    properties_type properties = {};

private:
    std::pair<vertex_ptr_type, vertex_ptr_type> _vertices;
};

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_directional_tag DirectionalTag = directed_t,
    detail::c_properties Properties = types::empty_properties>
using edge = edge_descriptor<VertexType, DirectionalTag, Properties>;

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_properties Properties = types::empty_properties>
using directed_edge = edge_descriptor<VertexType, directed_t, Properties>;

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_properties Properties = types::empty_properties>
using undirected_edge = edge_descriptor<VertexType, undirected_t, Properties>;

} // namespace gl
