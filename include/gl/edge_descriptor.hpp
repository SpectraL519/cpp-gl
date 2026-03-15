// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "gl/edge_tags.hpp"
#include "gl/io/format.hpp"
#include "gl/vertex_descriptor.hpp"

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
        type_traits::c_empty_properties<properties_type>,
        types::empty_properties,
        properties_type&>;

    friend directional_tag;

    edge_descriptor() {
        *this = edge_descriptor::invalid();
    }

    explicit edge_descriptor(
        const types::id_type id, const types::id_type source, const types::id_type target
    )
    requires(type_traits::c_empty_properties<properties_type>)
    : _id(id), _vertices(source, target) {}

    explicit edge_descriptor(
        const types::id_type id,
        const types::id_type source,
        const types::id_type target,
        properties_type& properties
    )
    requires(type_traits::c_non_empty_properties<properties_type>)
    : _id(id), _vertices(source, target), _properties(properties) {}

    [[nodiscard]] gl_attr_force_inline static edge_descriptor invalid() noexcept
    requires(type_traits::c_empty_properties<properties_type>)
    {
        return edge_descriptor(constants::invalid_id, constants::invalid_id, constants::invalid_id);
    }

    [[nodiscard]] gl_attr_force_inline static edge_descriptor invalid() noexcept
    requires(type_traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return edge_descriptor(
            constants::invalid_id, constants::invalid_id, constants::invalid_id, invalid_properties
        );
    }

    edge_descriptor(const edge_descriptor&) = default;
    edge_descriptor& operator=(const edge_descriptor&) = default;

    edge_descriptor(edge_descriptor&&) = default;
    edge_descriptor& operator=(edge_descriptor&&) = default;

    ~edge_descriptor() = default;

    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(type_traits::c_directed_edge<type>)
    {
        return this->_id == other._id and (this->_vertices == other._vertices);
    }

    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(type_traits::c_undirected_edge<type>)
    {
        return this->_id == other._id
           and (this->_vertices == other._vertices
                or (this->_vertices == other.incident_vertices_r()));
    }

    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    [[nodiscard]] constexpr bool is_directed() const noexcept {
        return type_traits::c_directed_edge<type>;
    }

    [[nodiscard]] constexpr bool is_undirected() const noexcept {
        return type_traits::c_undirected_edge<type>;
    }

    [[nodiscard]] bool is_valid() const noexcept {
        return this->_id != constants::invalid_id and this->_vertices.first != constants::invalid_id
           and this->_vertices.second != constants::invalid_id;
    }

    [[nodiscard]] gl_attr_force_inline types::id_type id() const noexcept {
        return this->_id;
    }

    [[nodiscard]] gl_attr_force_inline types::homogeneous_pair<types::id_type> incident_vertices(
    ) const noexcept {
        return this->_vertices;
    }

    [[nodiscard]] gl_attr_force_inline types::homogeneous_pair<types::id_type> incident_vertices_r(
    ) const noexcept {
        return std::make_pair(this->_vertices.second, this->_vertices.first);
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline const types::id_type source() const noexcept {
        return this->_vertices.first;
    }

    [[nodiscard]] gl_attr_force_inline const types::id_type target() const noexcept {
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

    [[nodiscard]] properties_ref_type properties() const {
        if (not this->is_valid())
            throw std::logic_error("Cannot access properties of an invalid edge");

        return this->_properties.get();
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
                os << "[source: " << this->_vertices.first << ", target: " << this->_vertices.second
                   << " | properties: " << this->_properties.get() << "]";
            }
            else {
                os << "[" << this->_vertices.first << ", " << this->_vertices.second << " | "
                   << this->_properties.get() << "]";
            }
        }
    }

    void _write_no_properties(std::ostream& os) const {
        if (io::is_option_set(os, io::graph_option::verbose))
            os << "[source: " << this->_vertices.first << ", target: " << this->_vertices.first
               << "]";
        else
            os << "[" << this->_vertices.first << ", " << this->_vertices.second << "]";
    }

    types::id_type _id;
    types::homogeneous_pair<types::id_type> _vertices;
    [[no_unique_address]] std::conditional_t<
        type_traits::c_empty_properties<properties_type>,
        types::empty_properties,
        std::reference_wrapper<properties_type>> _properties;
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
