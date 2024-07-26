#pragma once

#include "detail/concepts.hpp"
#include "detail/default_types.hpp"
#include "edge_tag.hpp"
#include "vertex_descriptor.hpp"

#include <memory>
#include <type_traits>
#include <vector>

namespace gl {

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_tag EdgeTag = directed_t,
    detail::c_properties Properties = detail::empty_properties>
class edge_descriptor {
public:
    using vertex_type = VertexType;
    using vertex_ptr_type = std::shared_ptr<vertex_type>;
    using edge_tag = EdgeTag;
    using properties_type = Properties;

    edge_descriptor() = delete;

    explicit edge_descriptor(vertex_ptr_type u, vertex_ptr_type v) : _vertices(u, v) {}

    explicit edge_descriptor(vertex_ptr_type u, vertex_ptr_type v, const properties_type& properties)
    requires(not std::is_same_v<properties_type, detail::empty_properties>)
    : _vertices(u, v), properties(properties) {}

    edge_descriptor(const edge_descriptor&) = default;
    edge_descriptor(edge_descriptor&&) = default;

    edge_descriptor& operator=(const edge_descriptor&) = default;
    edge_descriptor& operator=(edge_descriptor&&) = default;

    ~edge_descriptor() = default;

    properties_type properties = {};

    [[nodiscard]] inline constexpr bool is_directed() const {
        return std::is_same_v<edge_tag, directed_t>;
    }

    [[nodiscard]] inline constexpr bool is_undirected() const {
        return std::is_same_v<edge_tag, undirected_t>;
    }

    [[nodiscard]] inline const std::pair<vertex_ptr_type, vertex_ptr_type>& incident_vertices() const {
        return this->_vertices;
    }

    [[nodiscard]] inline vertex_ptr_type first() const {
        return this->_vertices.first;
    }

    [[nodiscard]] inline vertex_ptr_type second() const {
        return this->_vertices.second;
    }

    [[nodiscard]] vertex_ptr_type incident_vertex(const vertex_ptr_type& v) {
        if (*v == *this->_vertices.first)
            return this->_vertices.second;

        if (*v == *this->_vertices.second)
            return this->_vertices.first;

        return nullptr;
    }

    [[nodiscard]] inline bool is_incident_form(const vertex_ptr_type& v)
    requires(std::is_same_v<edge_tag, directed_t>)
    {
        return *v == *this->_vertices.first;
    }

    [[nodiscard]] inline bool is_incident_form(const vertex_ptr_type& v)
    requires(std::is_same_v<edge_tag, undirected_t>)
    {
        return this->incident_vertex(v) != nullptr;
    }

    [[nodiscard]] inline bool is_incident_to(const vertex_ptr_type& v)
    requires(std::is_same_v<edge_tag, directed_t>)
    {
        return *v == *this->_vertices.second;
    }

    [[nodiscard]] inline bool is_incident_to(const vertex_ptr_type& v)
    requires(std::is_same_v<edge_tag, undirected_t>)
    {
        return this->incident_vertex(v) != nullptr;
    }

private:
    std::pair<vertex_ptr_type, vertex_ptr_type> _vertices;
};

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_edge_tag EdgeTag = directed_t,
    detail::c_properties Properties = detail::empty_properties>
using edge = edge_descriptor<VertexType, EdgeTag, Properties>;

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_properties Properties = detail::empty_properties>
using directed_edge = edge_descriptor<VertexType, directed_t, Properties>;

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_properties Properties = detail::empty_properties>
using undirected_edge = edge_descriptor<VertexType, undirected_t, Properties>;

} // namespace gl
