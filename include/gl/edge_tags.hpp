// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "types/properties.hpp"
#include "types/type_traits.hpp"

namespace gl {

struct directed_t;

struct undirected_t;

namespace type_traits {

template <typename T>
concept c_edge_directional_tag = c_one_of<T, directed_t, undirected_t>;

} // namespace type_traits

template <type_traits::c_edge_directional_tag EdgeTag, type_traits::c_properties Properties>
class edge_descriptor;

namespace type_traits {

template <typename T>
inline constexpr bool is_directed_v = false;

template <c_instantiation_of<edge_descriptor> EdgeType>
inline constexpr bool is_directed_v<EdgeType> =
    std::is_same_v<typename EdgeType::directional_tag, directed_t>;

template <typename T>
inline constexpr bool is_undirected_v = false;

template <c_instantiation_of<edge_descriptor> EdgeType>
inline constexpr bool is_undirected_v<EdgeType> =
    std::is_same_v<typename EdgeType::directional_tag, undirected_t>;

} // namespace type_traits

struct directed_t {
    using type = std::type_identity_t<directed_t>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_directed_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return vertex_id == edge._vertices.first;
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_directed_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return vertex_id == edge._vertices.second;
    }
};

struct undirected_t {
    using type = std::type_identity_t<undirected_t>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_undirected_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return edge.is_incident_with(vertex_id);
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_undirected_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return edge.is_incident_with(vertex_id);
    }
};

} // namespace gl
