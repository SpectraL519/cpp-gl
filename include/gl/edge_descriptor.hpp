#pragma once

#include "edge_tags.hpp"
#include "gl/attributes/force_inline.hpp"
#include "types/formatter.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"
#include "vertex_descriptor.hpp"

#include <format>
#include <memory>
#include <type_traits>
#include <vector>

namespace gl {

template <
    type_traits::c_instantiation_of<vertex_descriptor> VertexType,
    type_traits::c_edge_directional_tag DirectionalTag = directed_t,
    type_traits::c_properties Properties = types::empty_properties>
class edge_descriptor {
public:
    using type = edge_descriptor<VertexType, DirectionalTag, Properties>;
    using vertex_type = VertexType;
    using directional_tag = DirectionalTag;
    using properties_type = Properties;

    friend directional_tag;

    edge_descriptor() = delete;
    edge_descriptor(const edge_descriptor&) = delete;
    edge_descriptor& operator=(const edge_descriptor&) = delete;

    explicit edge_descriptor(const vertex_type& first, const vertex_type& second)
    : _vertices(first, second) {}

    explicit edge_descriptor(
        const vertex_type& first, const vertex_type& second, const properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<properties_type>)
    : _vertices(first, second), properties(properties) {}

    edge_descriptor(edge_descriptor&&) = default;
    edge_descriptor& operator=(edge_descriptor&&) = default;

    ~edge_descriptor() = default;

    [[nodiscard]] constexpr bool is_directed() const {
        return type_traits::is_directed_v<type>;
    }

    [[nodiscard]] constexpr bool is_undirected() const {
        return type_traits::is_undirected_v<type>;
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline
    const types::homogeneous_pair<const vertex_type&>& incident_vertices() const {
        return this->_vertices;
    }

    [[nodiscard]] gl_attr_force_inline const vertex_type& first() const {
        return this->_vertices.first;
    }

    [[nodiscard]] gl_attr_force_inline const vertex_type& second() const {
        return this->_vertices.second;
    }

    // clang-format on

    [[nodiscard]] const vertex_type& incident_vertex(const vertex_type& vertex) const {
        if (&vertex == &this->_vertices.first)
            return this->_vertices.second;

        if (&vertex == &this->_vertices.second)
            return this->_vertices.first;

        throw std::invalid_argument(std::format(
            "Got invalid vertex [id = {} | addr = ]", vertex.id(), types::formatter(&vertex)
        ));
    }

    [[nodiscard]] gl_attr_force_inline bool is_incident_with(const vertex_type& vertex) const {
        return &vertex == &this->_vertices.first or &vertex == &this->_vertices.second;
    }

    [[nodiscard]] gl_attr_force_inline bool is_incident_from(const vertex_type& vertex) const {
        return directional_tag::is_incident_from(*this, vertex);
    }

    [[nodiscard]] gl_attr_force_inline bool is_incident_to(const vertex_type& vertex) const {
        return directional_tag::is_incident_to(*this, vertex);
    }

    [[nodiscard]] gl_attr_force_inline bool is_loop() const {
        return this->_vertices.first == this->_vertices.second;
    }

    [[no_unique_address]] mutable properties_type properties{};

private:
    types::homogeneous_pair<const vertex_type&> _vertices;
};

template <
    type_traits::c_instantiation_of<vertex_descriptor> VertexType,
    type_traits::c_edge_directional_tag DirectionalTag = directed_t,
    type_traits::c_properties Properties = types::empty_properties>
using edge = edge_descriptor<VertexType, DirectionalTag, Properties>;

template <
    type_traits::c_instantiation_of<vertex_descriptor> VertexType,
    type_traits::c_properties Properties = types::empty_properties>
using directed_edge = edge_descriptor<VertexType, directed_t, Properties>;

template <
    type_traits::c_instantiation_of<vertex_descriptor> VertexType,
    type_traits::c_properties Properties = types::empty_properties>
using undirected_edge = edge_descriptor<VertexType, undirected_t, Properties>;

} // namespace gl
