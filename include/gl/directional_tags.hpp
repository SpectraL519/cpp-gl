// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/traits.hpp"
#include "gl/types/properties.hpp"

namespace gl {

struct directed_t;

struct undirected_t;

namespace traits {

template <typename T>
concept c_graph_directional_tag = c_one_of<T, directed_t, undirected_t>;

} // namespace traits

template <traits::c_graph_directional_tag DirectionalTag, traits::c_properties Properties>
class edge_descriptor;

namespace traits {

template <typename E>
concept c_directed_edge =
    c_instantiation_of<E, edge_descriptor>
    and std::same_as<typename E::directional_tag, directed_t>;

template <typename E>
concept c_undirected_edge =
    c_instantiation_of<E, edge_descriptor>
    and std::same_as<typename E::directional_tag, undirected_t>;

} // namespace traits

struct directed_t {
    using type = std::type_identity_t<directed_t>;

    template <traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(traits::c_directed_edge<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return vertex_id == edge._vertices.first;
    }

    template <traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(traits::c_directed_edge<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return vertex_id == edge._vertices.second;
    }
};

struct undirected_t {
    using type = std::type_identity_t<undirected_t>;

    template <traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(traits::c_undirected_edge<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return edge.is_incident_with(vertex_id);
    }

    template <traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(traits::c_undirected_edge<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, const types::id_type vertex_id
    ) {
        return edge.is_incident_with(vertex_id);
    }
};

} // namespace gl
