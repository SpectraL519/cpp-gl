#pragma once

#include "edge_tags.hpp"
#include "gl/attributes/force_inline.hpp"
#include "gl/impl/impl_tags_decl.hpp"
#include "io.hpp"
#include "types/properties.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

#include <compare>
#include <sstream>

namespace gl {

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag,
    type_traits::c_properties VertexProperties,
    type_traits::c_properties EdgeProperties,
    type_traits::c_graph_impl_tag ImplTag>
struct graph_traits;

template <type_traits::c_properties Properties = types::empty_properties>
class vertex_descriptor {
public:
    using type = std::type_identity_t<vertex_descriptor<Properties>>;
    using properties_type = Properties;

    template <type_traits::c_instantiation_of<graph_traits> GraphTraits>
    friend class graph;

    vertex_descriptor() = default;
    vertex_descriptor(const vertex_descriptor&) = delete;
    vertex_descriptor& operator=(const vertex_descriptor&) = delete;

    explicit vertex_descriptor(const types::id_type id) : _id(id) {}

    explicit vertex_descriptor(const types::id_type id, const properties_type& properties)
    requires(not type_traits::is_default_properties_type_v<properties_type>)
    : _id(id), properties(properties) {}

    vertex_descriptor(vertex_descriptor&&) = default;
    vertex_descriptor& operator=(vertex_descriptor&&) = default;

    ~vertex_descriptor() = default;

    gl_attr_force_inline bool operator==(const vertex_descriptor& other) const {
        return this->_id == other._id;
    }

    gl_attr_force_inline std::strong_ordering operator<=>(const vertex_descriptor& other) const {
        return this->_id <=> other._id;
    }

    [[nodiscard]] gl_attr_force_inline types::id_type id() const {
        return this->_id;
    }

    [[no_unique_address]] mutable properties_type properties{};

    friend std::ostream& operator<<(std::ostream& os, const vertex_descriptor& vertex) {
        if constexpr (not type_traits::is_default_properties_type_v<properties_type>
                      and type_traits::c_writable<properties_type>) {
            if (io::is_option_set(os, io::option::with_vertex_properties)) {
                os << _io_quote << vertex._id << " | " << vertex.properties << _io_quote;
                return os;
            }
        }

        os << _io_quote << vertex._id << _io_quote;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, vertex_descriptor& vertex) {
        std::string input_str;
        std::stringstream ss;

        if constexpr (not type_traits::is_default_properties_type_v<properties_type>
                      and type_traits::c_readable<properties_type>) {
            if (io::is_option_set(is, io::option::with_vertex_properties)) {
                is >> std::quoted(input_str, _io_quote);
                ss << input_str;

                types::id_type id;
                std::string separator;

                ss >> id >> separator;
                if (separator.length() != constants::one or separator.front() != _io_separator)
                    throw std::ios_base::failure("Invalid property vertex ios format! Expected: `id | <properties>`");

                ss >> vertex.properties;
                vertex._id = id;

                return is;
            }
        }

        is >> std::quoted(input_str, _io_quote);
        ss << input_str;
        ss >> vertex._id;

        return is;
    }

private:
    static constexpr char _io_quote = '`';
    static constexpr char _io_separator = '|';

    types::id_type _id;
};

template <type_traits::c_properties Properties = types::empty_properties>
using vertex = vertex_descriptor<Properties>;

namespace types {

template <type_traits::c_instantiation_of<vertex_descriptor> VertexType>
using vertex_ptr_type = std::unique_ptr<VertexType>;

} // namespace types

namespace detail {

template <type_traits::c_instantiation_of<vertex_descriptor> VertexType>
[[nodiscard]] gl_attr_force_inline types::vertex_ptr_type<VertexType> make_vertex(
    const types::id_type id
) {
    return std::make_unique<VertexType>(id);
}

template <type_traits::c_instantiation_of<vertex_descriptor> VertexType>
[[nodiscard]] gl_attr_force_inline types::vertex_ptr_type<VertexType> make_vertex(
    const types::id_type id, const typename VertexType::properties_type& properties
) {
    return std::make_unique<VertexType>(id, properties);
}

} // namespace detail

} // namespace gl
