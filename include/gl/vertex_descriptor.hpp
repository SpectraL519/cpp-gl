#pragma once

#include "edge_tags.hpp"
#include "gl/impl/impl_tags_decl.hpp"
#include "types/default_types.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

#include <compare>

namespace gl {

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag,
    type_traits::c_properties VertexProperties,
    type_traits::c_properties EdgeProperties,
    type_traits::c_graph_impl_tag ImplTag>
struct graph_traits;

template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
class graph;

template <type_traits::c_properties Properties = types::empty_properties>
class vertex_descriptor {
public:
    using type = std::type_identity_t<vertex_descriptor<Properties>>;
    using properties_type = Properties;

    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    friend class graph;

    vertex_descriptor() = delete;
    vertex_descriptor(const vertex_descriptor&) = delete;
    vertex_descriptor& operator=(const vertex_descriptor&) = delete;

    explicit vertex_descriptor(const types::id_type id) : _id(id) {}

    explicit vertex_descriptor(const types::id_type id, const properties_type& properties)
    requires(not type_traits::is_default_properties_type_v<properties_type>)
    : _id(id), properties(properties) {}

    vertex_descriptor(vertex_descriptor&&) = default;
    vertex_descriptor& operator=(vertex_descriptor&&) = default;

    ~vertex_descriptor() = default;

    bool operator==(const vertex_descriptor& other) const {
        return this->_id == other._id;
    }

    std::strong_ordering operator<=>(const vertex_descriptor& other) const {
        return this->_id <=> other._id;
    }

    [[nodiscard]] inline types::id_type id() const {
        return this->_id;
    }

    [[no_unique_address]] properties_type properties = properties_type{};

private:
    types::id_type _id;
};

template <type_traits::c_properties Properties = types::empty_properties>
using vertex = vertex_descriptor<Properties>;

} // namespace gl
