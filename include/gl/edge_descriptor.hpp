// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "edge_tags.hpp"
#include "io/format.hpp"
#include "vertex_descriptor.hpp"

namespace gl {

template <
    type_traits::c_instantiation_of<vertex_descriptor> VertexType,
    type_traits::c_edge_directional_tag DirectionalTag = directed_t,
    type_traits::c_properties Properties = types::empty_properties>
class edge_descriptor final {
public:
    using type = edge_descriptor<VertexType, DirectionalTag, Properties>;
    using vertex_type = VertexType;
    using directional_tag = DirectionalTag;
    using properties_type = Properties;

    friend directional_tag;

    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    friend class graph;

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

    [[nodiscard]] gl_attr_force_inline types::homogeneous_pair<types::id_type> incident_vertex_ids(
    ) const {
        return std::make_pair(this->_vertices.first.id(), this->_vertices.second.id());
    }

    [[nodiscard]] gl_attr_force_inline types::id_type first_id() const {
        return this->first().id();
    }

    [[nodiscard]] gl_attr_force_inline types::id_type second_id() const {
        return this->second().id();
    }

    // returns the `other` vertex or throws error if the given vertex is not incident with the edge
    [[nodiscard]] const vertex_type& incident_vertex(const vertex_type& vertex) const {
        if (&vertex == &this->_vertices.first)
            return this->_vertices.second;

        if (&vertex == &this->_vertices.second)
            return this->_vertices.first;

        throw std::invalid_argument(std::format(
            "Got invalid vertex [id = {} | addr = {}]", vertex.id(), io::format(&vertex)
        ));
    }

    [[nodiscard]] const types::id_type incident_vertex_id(const types::id_type vertex_id) const {
        if (vertex_id == this->first_id())
            return this->second_id();

        if (vertex_id == this->second_id())
            return this->first_id();

        throw std::invalid_argument(std::format("Got invalid vertex id: {}", vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline bool is_incident_with(const vertex_type& vertex) const {
        return &vertex == &this->_vertices.first or &vertex == &this->_vertices.second;
    }

    // true if the given vertex is the `source` of the edge
    [[nodiscard]] gl_attr_force_inline bool is_incident_from(const vertex_type& vertex) const {
        return directional_tag::is_incident_from(*this, vertex);
    }

    // true if the given vertex is the `target` vertex of the edge
    [[nodiscard]] gl_attr_force_inline bool is_incident_to(const vertex_type& vertex) const {
        return directional_tag::is_incident_to(*this, vertex);
    }

    [[nodiscard]] gl_attr_force_inline bool is_loop() const {
        return this->_vertices.first == this->_vertices.second;
    }

    [[no_unique_address]] mutable properties_type properties{};

    friend inline std::ostream& operator<<(std::ostream& os, const edge_descriptor& edge) {
        edge._write(os);
        return os;
    }

private:
    class vertex_writer {
    public:
        vertex_writer(const vertex_type& vertex, bool within_context)
        : _vertex_ref(vertex), _within_context(within_context) {}

        friend std::ostream& operator<<(std::ostream& os, const vertex_writer& vw) {
            if (vw._within_context)
                os << vw._vertex_ref.id();
            else
                os << vw._vertex_ref;
            return os;
        }

    private:
        const vertex_type& _vertex_ref;
        bool _within_context;
    };

    void _write(std::ostream& os, bool within_context = false) const {
        if constexpr (not type_traits::c_writable<properties_type>) {
            this->_write_no_properties(os, within_context);
            return;
        }
        else {
            if (not io::is_option_set(os, io::graph_option::with_edge_properties)) {
                this->_write_no_properties(os, within_context);
                return;
            }

            if (io::is_option_set(os, io::graph_option::verbose)) {
                os << "[first: " << vertex_writer(this->first(), within_context)
                   << ", second: " << vertex_writer(this->second(), within_context)
                   << " | properties: " << this->properties << "]";
            }
            else {
                os << "[" << vertex_writer(this->first(), within_context) << ", "
                   << vertex_writer(this->second(), within_context) << " | " << this->properties
                   << "]";
            }
        }
    }

    void _write_no_properties(std::ostream& os, bool within_context = false) const {
        if (io::is_option_set(os, io::graph_option::verbose))
            os << "[first: " << vertex_writer(this->first(), within_context)
               << ", second: " << vertex_writer(this->second(), within_context) << "]";
        else
            os << "[" << vertex_writer(this->first(), within_context) << ", "
               << vertex_writer(this->second(), within_context) << "]";
    }

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
