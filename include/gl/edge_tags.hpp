#pragma once

#include "gl/attributes/force_inline.hpp"
#include "types/default_types.hpp"
#include "types/type_traits.hpp"

#include <memory>

namespace gl {

struct directed_t;

struct undirected_t;

namespace type_traits {

template <typename T>
concept c_edge_directional_tag = c_one_of<T, directed_t, undirected_t>;

} // namespace type_traits

template <type_traits::c_properties Properties>
class vertex_descriptor;

template <
    type_traits::c_instantiation_of<vertex_descriptor> VertexType,
    type_traits::c_edge_directional_tag EdgeTag,
    type_traits::c_properties Properties>
class edge_descriptor;

namespace type_traits {

template <c_instantiation_of<edge_descriptor> EdgeType>
inline constexpr bool is_directed_v =
    std::is_same_v<typename EdgeType::directional_tag, directed_t>;

template <c_instantiation_of<edge_descriptor> EdgeType>
inline constexpr bool is_undirected_v =
    std::is_same_v<typename EdgeType::directional_tag, undirected_t>;

} // namespace type_traits

struct directed_t {
    using type = std::type_identity_t<directed_t>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_directed_v<EdgeType>)
    using edge_ptr_type = std::unique_ptr<EdgeType>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_directed_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static edge_ptr_type<EdgeType> make(
        const typename EdgeType::vertex_ptr_type& first,
        const typename EdgeType::vertex_ptr_type& second
    ) {
        return std::make_unique<EdgeType>(first, second);
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_directed_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static edge_ptr_type<EdgeType> make(
        const typename EdgeType::vertex_ptr_type& first,
        const typename EdgeType::vertex_ptr_type& second,
        const typename EdgeType::properties_type& properties
    ) {
        return std::make_unique<EdgeType>(first, second, properties);
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_directed_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return vertex == edge._vertices.first;
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_directed_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return vertex == edge._vertices.second;
    }
};

struct undirected_t {
    using type = std::type_identity_t<undirected_t>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_undirected_v<EdgeType>)
    using edge_ptr_type = std::shared_ptr<EdgeType>;

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_undirected_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static edge_ptr_type<EdgeType> make(
        const typename EdgeType::vertex_ptr_type& first,
        const typename EdgeType::vertex_ptr_type& second
    ) {
        return std::make_shared<EdgeType>(first, second);
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_undirected_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static edge_ptr_type<EdgeType> make(
        const typename EdgeType::vertex_ptr_type& first,
        const typename EdgeType::vertex_ptr_type& second,
        const typename EdgeType::properties_type& properties
    ) {
        return std::make_shared<EdgeType>(first, second, properties);
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_undirected_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_from(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return edge.is_incident_with(vertex);
    }

    template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
    requires(type_traits::is_undirected_v<EdgeType>)
    [[nodiscard]] gl_attr_force_inline static bool is_incident_to(
        const EdgeType& edge, const typename EdgeType::vertex_ptr_type& vertex
    ) {
        return edge.is_incident_with(vertex);
    }
};

namespace types {

template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
using edge_ptr_type = typename EdgeType::directional_tag::template edge_ptr_type<EdgeType>;

} // namespace types

template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
[[nodiscard]] gl_attr_force_inline types::edge_ptr_type<EdgeType> make_edge(
    const typename EdgeType::vertex_ptr_type& first,
    const typename EdgeType::vertex_ptr_type& second
) {
    return EdgeType::directional_tag::template make<EdgeType>(first, second);
}

template <type_traits::c_instantiation_of<edge_descriptor> EdgeType>
[[nodiscard]] gl_attr_force_inline types::edge_ptr_type<EdgeType> make_edge(
    const typename EdgeType::vertex_ptr_type& first,
    const typename EdgeType::vertex_ptr_type& second,
    const typename EdgeType::properties_type& properties
) {
    return EdgeType::directional_tag::template make<EdgeType>(first, second, properties);
}

} // namespace gl
