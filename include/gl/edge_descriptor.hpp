#pragma once

#include "detail/concepts.hpp"
#include "detail/default_types.hpp"
#include "edge_tags.hpp"
#include "vertex_descriptor.hpp"

#include <memory>
#include <type_traits>
#include <vector>

namespace gl {

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_directional_tag DirectionalTag = directed_t,
    detail::c_properties Properties = detail::empty_properties>
class edge_descriptor {
public:
    using type = edge_descriptor<VertexType, DirectionalTag, Properties>;
    using vertex_type = std::shared_ptr<VertexType>;
    using directional_tag = DirectionalTag;
    using properties_type = Properties;

    edge_descriptor() = delete;

    explicit edge_descriptor(vertex_type u, vertex_type v) : _vertices(u, v) {}

    explicit edge_descriptor(vertex_type u, vertex_type v, const properties_type& properties)
    requires(not std::is_same_v<properties_type, detail::empty_properties>)
    : _vertices(u, v), properties(properties) {}

    edge_descriptor(const edge_descriptor&) = default;
    edge_descriptor(edge_descriptor&&) = default;

    edge_descriptor& operator=(const edge_descriptor&) = default;
    edge_descriptor& operator=(edge_descriptor&&) = default;

    ~edge_descriptor() = default;

    properties_type properties = {};

    [[nodiscard]] inline constexpr bool is_directed() const {
        return std::is_same_v<directional_tag, directed_t>;
    }

    [[nodiscard]] inline constexpr bool is_undirected() const {
        return std::is_same_v<directional_tag, undirected_t>;
    }

    [[nodiscard]] inline const std::pair<vertex_type, vertex_type>& incident_vertices() const {
        return this->_vertices;
    }

    [[nodiscard]] inline vertex_type first() const {
        return this->_vertices.first;
    }

    [[nodiscard]] inline vertex_type second() const {
        return this->_vertices.second;
    }

    [[nodiscard]] vertex_type incident_vertex(const vertex_type& vertex) const {
        if (*vertex == *this->_vertices.first)
            return this->_vertices.second;

        if (*vertex == *this->_vertices.second)
            return this->_vertices.first;

        return nullptr;
    }

    [[nodiscard]] inline bool is_incident_from(const vertex_type& vertex) const {
        return directional_tag::is_incident_from(*this, vertex);
    }

    [[nodiscard]] inline bool is_incident_to(const vertex_type& vertex) const {
        return directional_tag::is_incident_to(*this, vertex);
    }

    friend directional_tag;

private:
    std::pair<vertex_type, vertex_type> _vertices;
};

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_directional_tag DirectionalTag = directed_t,
    detail::c_properties Properties = detail::empty_properties>
using edge = edge_descriptor<VertexType, DirectionalTag, Properties>;

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_properties Properties = detail::empty_properties>
using directed_edge = edge_descriptor<VertexType, directed_t, Properties>;

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_properties Properties = detail::empty_properties>
using undirected_edge = edge_descriptor<VertexType, undirected_t, Properties>;

} // namespace gl
