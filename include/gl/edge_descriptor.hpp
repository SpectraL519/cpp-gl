// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "edge_tags.hpp"
#include "io/format.hpp"
#include "vertex_descriptor.hpp"

namespace gl {

template <
    type_traits::c_edge_directional_tag DirectionalTag = directed_t,
    type_traits::c_properties Properties = types::empty_properties>
class edge_descriptor final {
public:
    using type = edge_descriptor<DirectionalTag, Properties>;
    using directional_tag = DirectionalTag;
    using properties_type = Properties;
    using properties_ref_type = std::conditional_t<
        type_traits::is_default_properties_type_v<properties_type>,
        types::empty_properties,
        properties_type&>;

    friend directional_tag;

    edge_descriptor() = delete;
    edge_descriptor(const edge_descriptor&) = delete;
    edge_descriptor& operator=(const edge_descriptor&) = delete;

    explicit edge_descriptor(
        const types::id_type id, const types::id_type first, const types::id_type second
    )
    requires(type_traits::is_default_properties_type_v<properties_type>)
    : _id(id), _vertices(first, second) {}

    explicit edge_descriptor(
        const types::id_type id,
        const types::id_type first,
        const types::id_type second,
        properties_type& properties
    )
    requires(not type_traits::is_default_properties_type_v<properties_type>)
    : _id(id), _vertices(first, second), _properties(properties) {}

    [[nodiscard]] static gl_attr_force_inline edge_descriptor invalid() noexcept
    requires(type_traits::is_default_properties_type_v<properties_type>)
    {
        return edge_descriptor(constants::invalid_id, constants::invalid_id, constants::invalid_id);
    }

    [[nodiscard]] static gl_attr_force_inline edge_descriptor invalid() noexcept
    requires(not type_traits::is_default_properties_type_v<properties_type>)
    {
        static properties_type invalid_properties{};
        return edge_descriptor(
            constants::invalid_id, constants::invalid_id, constants::invalid_id, invalid_properties
        );
    }

    edge_descriptor(edge_descriptor&&) = default;
    edge_descriptor& operator=(edge_descriptor&&) = default;

    ~edge_descriptor() = default;

    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(type_traits::is_directed_v<type>)
    {
        return this->_id == other._id and (this->_vertices == other._vertices);
    }

    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(type_traits::is_undirected_v<type>)
    {
        return this->_id == other._id
           and (this->_vertices == other._vertices
                or (this->_vertices == other.incident_vertices_r()));
    }

    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    [[nodiscard]] bool is_valid() const noexcept {
        return this->_id != constants::invalid_id and this->_vertices.first != constants::invalid_id
           and this->_vertices.second != constants::invalid_id;
    }

    [[nodiscard]] constexpr bool is_directed() const noexcept {
        return type_traits::is_directed_v<type>;
    }

    [[nodiscard]] constexpr bool is_undirected() const noexcept {
        return type_traits::is_undirected_v<type>;
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline types::id_type id() const noexcept {
        return this->_id;
    }

    [[nodiscard]] gl_attr_force_inline types::homogeneous_pair<const types::id_type> incident_vertices() const noexcept {
        return this->_vertices;
    }

    [[nodiscard]] gl_attr_force_inline types::homogeneous_pair<const types::id_type> incident_vertices_r() const noexcept {
        return std::make_pair(this->_vertices.second, this->_vertices.first);
    }

    // TODO: rename to source
    [[nodiscard]] gl_attr_force_inline const types::id_type first() const noexcept {
        return this->_vertices.first;
    }

    // TODO: rename to target
    [[nodiscard]] gl_attr_force_inline const types::id_type second() const noexcept {
        return this->_vertices.second;
    }

    // clang-format on

    // returns the `other` vertex or throws error if the given vertex is not incident with the edge
    [[nodiscard]] const types::id_type incident_vertex(const types::id_type vertex_id) const {
        if (vertex_id == this->_vertices.first)
            return this->_vertices.second;

        if (vertex_id == this->_vertices.second)
            return this->_vertices.first;

        throw std::invalid_argument(std::format("Got invalid vertex id: {}", vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline bool is_incident_with(const types::id_type vertex_id
    ) const noexcept {
        return vertex_id == this->_vertices.first or vertex_id == this->_vertices.second;
    }

    // true if the given vertex is the `source` of the edge
    [[nodiscard]] gl_attr_force_inline bool is_incident_from(const types::id_type vertex_id
    ) const noexcept {
        return directional_tag::is_incident_from(*this, vertex_id);
    }

    // true if the given vertex is the `target` vertex of the edge
    [[nodiscard]] gl_attr_force_inline bool is_incident_to(const types::id_type vertex_id
    ) const noexcept {
        return directional_tag::is_incident_to(*this, vertex_id);
    }

    [[nodiscard]] gl_attr_force_inline bool is_loop() const noexcept {
        return this->_vertices.first == this->_vertices.second;
    }

    [[nodiscard]] gl_attr_force_inline properties_type& properties() const noexcept {
        return this->_properties;
    }

    friend inline std::ostream& operator<<(std::ostream& os, const edge_descriptor& edge) {
        edge._write(os);
        return os;
    }

private:
    void _write(std::ostream& os) const {
        if constexpr (not type_traits::c_writable<properties_type>) {
            this->_write_no_properties(os);
            return;
        }
        else {
            if (not io::is_option_set(os, io::graph_option::with_edge_properties)) {
                this->_write_no_properties(os);
                return;
            }

            if (io::is_option_set(os, io::graph_option::verbose)) {
                os << "[first: " << this->_vertices.first << ", second: " << this->_vertices.second
                   << " | properties: " << this->_properties << "]";
            }
            else {
                os << "[" << this->_vertices.first << ", " << this->_vertices.second << " | "
                   << this->_properties << "]";
            }
        }
    }

    void _write_no_properties(std::ostream& os) const {
        if (io::is_option_set(os, io::graph_option::verbose))
            os << "[first: " << this->_vertices.first << ", second: " << this->_vertices.first
               << "]";
        else
            os << "[" << this->_vertices.first << ", " << this->_vertices.second << "]";
    }

    types::id_type _id;
    types::homogeneous_pair<types::id_type> _vertices;
    [[no_unique_address]] properties_ref_type _properties;
};

template <
    type_traits::c_edge_directional_tag DirectionalTag = directed_t,
    type_traits::c_properties Properties = types::empty_properties>
using edge = edge_descriptor<DirectionalTag, Properties>;

template <type_traits::c_properties Properties = types::empty_properties>
using directed_edge = edge_descriptor<directed_t, Properties>;

template <type_traits::c_properties Properties = types::empty_properties>
using undirected_edge = edge_descriptor<undirected_t, Properties>;

} // namespace gl
