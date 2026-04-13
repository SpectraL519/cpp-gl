// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/directional_tags.hpp"
#include "gl/io/format.hpp"
#include "gl/io/options.hpp"
#include "gl/types/core.hpp"
#include "gl/vertex_descriptor.hpp"

#include <format>

namespace gl {

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
class edge_descriptor final {
public:
    using type = edge_descriptor<DirectionalTag, Properties, IdType>;
    using id_type = IdType;
    using directional_tag = DirectionalTag;
    using properties_type = Properties;
    using properties_ref_type = std::conditional_t<
        traits::c_empty_properties<properties_type>,
        empty_properties,
        properties_type&>;

    friend directional_tag;

    edge_descriptor() {
        *this = edge_descriptor::invalid();
    }

    explicit edge_descriptor(const id_type id, const id_type source, const id_type target)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id), _vertices(source, target) {}

    explicit edge_descriptor(
        const id_type id, const id_type source, const id_type target, properties_type& properties
    )
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _vertices(source, target), _properties(properties) {}

    [[nodiscard]] gl_attr_force_inline static edge_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return edge_descriptor(invalid_id, invalid_id, invalid_id);
    }

    [[nodiscard]] gl_attr_force_inline static edge_descriptor invalid() noexcept
    requires(traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return edge_descriptor(invalid_id, invalid_id, invalid_id, invalid_properties);
    }

    edge_descriptor(const edge_descriptor&) = default;
    edge_descriptor& operator=(const edge_descriptor&) = default;

    edge_descriptor(edge_descriptor&&) noexcept = default;
    edge_descriptor& operator=(edge_descriptor&&) noexcept = default;

    ~edge_descriptor() = default;

    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(traits::c_directed_edge<type>)
    {
        return this->_id == other._id and (this->_vertices == other._vertices);
    }

    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(traits::c_undirected_edge<type>)
    {
        return this->_id == other._id
           and (this->_vertices == other._vertices
                or (this->_vertices == other.incident_vertices_r()));
    }

    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    [[nodiscard]] constexpr bool is_directed() const noexcept {
        return traits::c_directed_edge<type>;
    }

    [[nodiscard]] constexpr bool is_undirected() const noexcept {
        return traits::c_undirected_edge<type>;
    }

    [[nodiscard]] bool is_valid() const noexcept {
        return this->_id != invalid_id and this->_vertices.first != invalid_id
           and this->_vertices.second != invalid_id;
    }

    [[nodiscard]] gl_attr_force_inline id_type id() const noexcept {
        return this->_id;
    }

    [[nodiscard]] gl_attr_force_inline homogeneous_pair<id_type> incident_vertices(
    ) const noexcept {
        return this->_vertices;
    }

    [[nodiscard]] gl_attr_force_inline homogeneous_pair<id_type> incident_vertices_r(
    ) const noexcept {
        return std::make_pair(this->_vertices.second, this->_vertices.first);
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline const id_type source() const noexcept {
        return this->_vertices.first;
    }

    [[nodiscard]] gl_attr_force_inline const id_type target() const noexcept {
        return this->_vertices.second;
    }

    // clang-format on

    [[nodiscard]] const id_type other(const id_type vertex_id) const {
        if (vertex_id == this->_vertices.first)
            return this->_vertices.second;

        if (vertex_id == this->_vertices.second)
            return this->_vertices.first;

        throw std::invalid_argument(std::format("Got invalid vertex id: {}", vertex_id));
    }

    [[nodiscard]] gl_attr_force_inline bool is_incident_with(const id_type vertex_id
    ) const noexcept {
        return vertex_id == this->_vertices.first or vertex_id == this->_vertices.second;
    }

    // true if the given vertex is the `source` of the edge
    [[nodiscard]] gl_attr_force_inline bool is_incident_from(const id_type vertex_id
    ) const noexcept {
        return directional_tag::is_incident_from(*this, vertex_id);
    }

    // true if the given vertex is the `target` vertex of the edge
    [[nodiscard]] gl_attr_force_inline bool is_incident_to(const id_type vertex_id) const noexcept {
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

    friend std::ostream& operator<<(std::ostream& os, const edge_descriptor& edge) {
        using enum io::detail::option_bit;

        if (io::is_option_set(os, verbose))
            return edge._verbose_write(os);
        else
            return edge._concise_write(os);
    }

private:
    std::ostream& _verbose_write(std::ostream& os) const
    requires std::same_as<directional_tag, undirected_t>
    {
        using enum io::detail::option_bit;

        os << "[id: " << this->_id << " | endpoints: {" << this->_vertices.first << ", "
           << this->_vertices.second << '}';
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_connection_properties))
                os << " | " << this->_properties.get();
        os << ']';

        return os;
    }

    std::ostream& _concise_write(std::ostream& os) const
    requires std::same_as<directional_tag, undirected_t>
    {
        using enum io::detail::option_bit;

        os << '{' << this->_vertices.first << ", " << this->_vertices.second << '}';
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_vertex_properties))
                os << '[' << this->_properties.get() << ']';

        return os;
    }

    std::ostream& _verbose_write(std::ostream& os) const
    requires std::same_as<directional_tag, directed_t>
    {
        using enum io::detail::option_bit;

        os << "[id: " << this->_id << " | source: " << this->_vertices.first
           << ", target: " << this->_vertices.second;
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_connection_properties))
                os << " | " << this->_properties.get();
        os << ']';

        return os;
    }

    std::ostream& _concise_write(std::ostream& os) const
    requires std::same_as<directional_tag, directed_t>
    {
        using enum io::detail::option_bit;

        os << '(' << this->_vertices.first << ", " << this->_vertices.second << ')';
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_vertex_properties))
                os << '[' << this->_properties.get() << ']';

        return os;
    }

    id_type _id;
    homogeneous_pair<id_type> _vertices;
    [[no_unique_address]] std::conditional_t<
        traits::c_empty_properties<properties_type>,
        empty_properties,
        std::reference_wrapper<properties_type>> _properties;
};

template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using edge = edge_descriptor<DirectionalTag, Properties, IdType>;

template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using directed_edge = edge_descriptor<directed_t, Properties, IdType>;

template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using undirected_edge = edge_descriptor<undirected_t, Properties, IdType>;

} // namespace gl
