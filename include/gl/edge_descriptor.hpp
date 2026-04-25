// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/edge_descriptor.hpp
/// @brief Defines the edge_descriptor class and related aliases, providing a lightweight wrapper for edges in graphs.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/directional_tags.hpp"
#include "gl/io/options.hpp"
#include "gl/io/ranges.hpp"
#include "gl/types/core.hpp"
#include "gl/vertex_descriptor.hpp"

#include <format>

namespace gl {

/// @ingroup GL GL-Core
/// @brief A lightweight wrapper representing a graph edge with its endpoints and optional properties.
///
/// **Module:** Part of the @ref GL-Core "Core Graph Components" group.
///
/// The `edge_descriptor` class provides a type-safe and efficient way to represent
/// edges in both directed and undirected graph structures. It encapsulates the unique
/// identifier of the edge, its source and target vertices, and optional property data.
///
/// > [!WARNING] This class is not intended to be instantiated directly.
/// >
/// > Instead, `edge_descriptor` objects should be retrieved from the @ref gl::graph class instance that owns the given edge.
///
/// ### Example Usage
/// ```cpp
/// std::cout << gl::io::verbose << gl::io::with_edge_properties;
/// for (auto v : graph.vertices()) {
///     for (auto e : graph.out_edges(v)) { // (1)!
///         if (e.is_loop()) // (2)!
///             e->weight = 0.0;
///         else
///             e->weight += 1.5; // (3)!
///
///         std::cout << e << "\n"; // (4)!
///     }
/// }
/// ```
///
/// 1\. Retrieve edges using valid `graph` traversal methods like `out_edges()`.
///
/// 2\. Utilize built-in edge utility methods to evaluate the edge type.
///
/// 3\. Access and modify custom property fields via the arrow operator `->`.
///
/// 4\. Print the detailed, formatted edge data using the applied stream manipulators.
///     *Example output:* `[id: 12 | source: 3, target: 7 | weight: 1.5]`
///
/// ### Template Parameters
/// | Parameter      | Description | Default | Constraint |
/// | :------------- | :--- | :--- | :--- |
/// | DirectionalTag | Tag specifying if the edge is directed or undirected. | @ref gl::directed_t "directed_t" | [**c_graph_directional_tag**](gl_concepts.md#gl-traits-c-graph-directional-tag) |
/// | Properties     | The type of property data attached to the edge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType         | The underlying integer type used for the IDs. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
///
/// ### See Also
/// * @ref gl::vertex_descriptor : For the corresponding vertex wrapper class.
/// * @ref gl::graph : For the owning graph class that manages edge descriptors.
template <
    traits::c_graph_directional_tag DirectionalTag = directed_t,
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
class edge_descriptor final {
public:
    /// @brief The fully instantiated type of this edge descriptor.
    using type = edge_descriptor<DirectionalTag, Properties, IdType>;
    /// @brief The identifier type used for edges and vertices.
    using id_type = IdType;
    /// @brief The tag denoting whether the edge is directed or undirected.
    using directional_tag = DirectionalTag;
    /// @brief The type of properties associated with the edge.
    using properties_type = Properties;

    friend directional_tag;

    /// @brief Default constructor, initializes to an invalid edge.
    edge_descriptor() {
        *this = edge_descriptor::invalid();
    }

    /// @brief Constructs an edge descriptor with the given IDs (for empty properties).
    /// @param id The unique identifier for the edge.
    /// @param source The unique identifier for the source vertex.
    /// @param target The unique identifier for the target vertex.
    explicit edge_descriptor(const id_type id, const id_type source, const id_type target)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id), _vertices(source, target) {}

    /// @brief Constructs an edge descriptor with the given IDs and properties.
    /// @param id The unique identifier for the edge.
    /// @param source The unique identifier for the source vertex.
    /// @param target The unique identifier for the target vertex.
    /// @param properties A reference to the property data to associate with this edge.
    explicit edge_descriptor(
        const id_type id, const id_type source, const id_type target, properties_type& properties
    )
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _vertices(source, target), _properties(properties) {}

    /// @brief Returns an invalid edge descriptor (for empty properties).
    /// @return An `edge_descriptor` holding `invalid_id` for edge and vertex IDs.
    [[nodiscard]] gl_attr_force_inline static edge_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return edge_descriptor(invalid_id, invalid_id, invalid_id);
    }

    /// @brief Returns an invalid edge descriptor (for non-empty properties).
    /// @return An `edge_descriptor` holding `invalid_id` endpoints and empty properties.
    [[nodiscard]] gl_attr_force_inline static edge_descriptor invalid() noexcept
    requires(traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return edge_descriptor(invalid_id, invalid_id, invalid_id, invalid_properties);
    }

    /// @brief Copy constructor.
    edge_descriptor(const edge_descriptor&) = default;
    /// @brief Copy assignment operator.
    edge_descriptor& operator=(const edge_descriptor&) = default;

    /// @brief Move constructor.
    edge_descriptor(edge_descriptor&&) noexcept = default;
    /// @brief Move assignment operator.
    edge_descriptor& operator=(edge_descriptor&&) noexcept = default;

    /// @brief Destructor.
    ~edge_descriptor() = default;

    /// @brief Equality comparison operator for directed edges.
    /// @param other The edge descriptor to compare against.
    /// @return `true` if IDs and exact endpoint pairs match, `false` otherwise.
    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(traits::c_directed_edge<type>)
    {
        return this->_id == other._id and (this->_vertices == other._vertices);
    }

    /// @brief Equality comparison operator for undirected edges.
    /// @param other The edge descriptor to compare against.
    /// @return `true` if IDs and endpoint pairs match (order independent), `false` otherwise.
    [[nodiscard]] bool operator==(const edge_descriptor& other) const noexcept
    requires(traits::c_undirected_edge<type>)
    {
        return this->_id == other._id
           and (this->_vertices == other._vertices
                or (this->_vertices == other.incident_vertices_r()));
    }

    /// @brief Boolean conversion operator.
    /// @return `true` if the edge is valid.
    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    /// @brief Checks if the edge represents a directed connection.
    /// @return `true` if `DirectionalTag` is `directed_t`.
    [[nodiscard]] constexpr bool is_directed() const noexcept {
        return traits::c_directed_edge<type>;
    }

    /// @brief Checks if the edge represents an undirected connection.
    /// @return `true` if `DirectionalTag` is `undirected_t`.
    [[nodiscard]] constexpr bool is_undirected() const noexcept {
        return traits::c_undirected_edge<type>;
    }

    /// @brief Checks if the edge descriptor is valid.
    /// @return `true` if the edge ID and both endpoints are not equal to `invalid_id`.
    [[nodiscard]] bool is_valid() const noexcept {
        return this->_id != invalid_id and this->_vertices.first != invalid_id
           and this->_vertices.second != invalid_id;
    }

    /// @brief Returns the edge ID.
    /// @return The underlying integer ID of the edge.
    [[nodiscard]] gl_attr_force_inline id_type id() const noexcept {
        return this->_id;
    }

    /// @brief Retrieves the endpoints of the edge as a pair.
    /// @return A `homogeneous_pair` representing `(source, target)`.
    [[nodiscard]] gl_attr_force_inline homogeneous_pair<id_type> incident_vertices(
    ) const noexcept {
        return this->_vertices;
    }

    /// @brief Retrieves the reversed endpoints of the edge as a pair.
    /// @return A `homogeneous_pair` representing `(target, source)`.
    [[nodiscard]] gl_attr_force_inline homogeneous_pair<id_type> incident_vertices_r(
    ) const noexcept {
        return std::make_pair(this->_vertices.second, this->_vertices.first);
    }

    /// @brief Returns the source vertex ID.
    /// @return The underlying integer ID of the source vertex.
    [[nodiscard]] gl_attr_force_inline id_type source() const noexcept {
        return this->_vertices.first;
    }

    /// @brief Returns the target vertex ID.
    /// @return The underlying integer ID of the target vertex.
    [[nodiscard]] gl_attr_force_inline id_type target() const noexcept {
        return this->_vertices.second;
    }

    /// @brief Gets the other endpoint of the edge given one of its incident vertices.
    /// @param vertex_id The ID of one incident vertex.
    /// @return The ID of the opposite vertex.
    /// @throws std::invalid_argument If the provided `vertex_id` is not incident to this edge.
    [[nodiscard]] id_type other(const id_type vertex_id) const {
        if (vertex_id == this->_vertices.first)
            return this->_vertices.second;

        if (vertex_id == this->_vertices.second)
            return this->_vertices.first;

        throw std::invalid_argument(std::format("Got invalid vertex id: {}", vertex_id));
    }

    /// @brief Checks if a specific vertex is incident to this edge.
    /// @param vertex_id The vertex ID to query.
    /// @return `true` if the vertex is either the source or the target, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_incident_with(const id_type vertex_id
    ) const noexcept {
        return vertex_id == this->_vertices.first or vertex_id == this->_vertices.second;
    }

    /// @brief Checks if the given vertex acts as the source for this edge.
    /// @param vertex_id The vertex ID to query.
    /// @return `true` if the vertex is the source (or any endpoint for undirected graphs).
    [[nodiscard]] gl_attr_force_inline bool is_incident_from(const id_type vertex_id
    ) const noexcept {
        return directional_tag::is_incident_from(*this, vertex_id);
    }

    /// @brief Checks if the given vertex acts as the target for this edge.
    /// @param vertex_id The vertex ID to query.
    /// @return `true` if the vertex is the target (or any endpoint for undirected graphs).
    [[nodiscard]] gl_attr_force_inline bool is_incident_to(const id_type vertex_id) const noexcept {
        return directional_tag::is_incident_to(*this, vertex_id);
    }

    /// @brief Checks if the edge is a self-loop.
    /// @return `true` if the source and target are the same vertex, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_loop() const noexcept {
        return this->_vertices.first == this->_vertices.second;
    }

    /// @brief Returns a reference to the edge properties.
    /// @return A reference to the associated `properties_type`.
    /// @throws std::logic_error If the edge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type& properties() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Arrow operator for accessing properties.
    /// @return A pointer to the associated `properties_type`.
    /// @throws std::logic_error If the edge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type* operator->() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return &this->_properties.get();
    }

    /// @brief Dereference operator for accessing properties.
    /// @return A reference to the associated `properties_type`.
    /// @throws std::logic_error If the edge descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type& operator*() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Output stream operator for edge descriptors.
    /// @param os The output stream.
    /// @param edge The edge descriptor to write.
    /// @return A reference to the output stream.
    friend std::ostream& operator<<(std::ostream& os, const edge_descriptor& edge) {
        using enum io::detail::option_bit;

        if (io::is_option_set(os, verbose))
            return edge._verbose_write(os);
        else
            return edge._concise_write(os);
    }

private:
    [[noreturn]] void _throw_invalid_access() const {
        throw std::logic_error("Cannot access properties of an invalid edge");
    }

    gl_attr_force_inline void _validate() const {
        if (not this->is_valid())
            this->_throw_invalid_access();
    }

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

/// @ingroup GL GL-Core
/// @brief Type alias for a directed edge descriptor.
///
/// Pre-binds the `DirectionalTag` of `edge_descriptor` to `directed_t`.
///
/// ### Template Parameters
/// | Parameter  | Description | Default | Constraint |
/// | :--------- | :--- | :--- | :--- |
/// | Properties | The type of property data attached to the edge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType     | The underlying integer type used for the IDs. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using directed_edge = edge_descriptor<directed_t, Properties, IdType>;

/// @ingroup GL GL-Core
/// @brief Type alias for an undirected edge descriptor.
///
/// Pre-binds the `DirectionalTag` of `edge_descriptor` to `undirected_t`.
///
/// ### Template Parameters
/// | Parameter  | Description | Default | Constraint |
/// | :--------- | :--- | :--- | :--- |
/// | Properties | The type of property data attached to the edge. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType     | The underlying integer type used for the IDs. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using undirected_edge = edge_descriptor<undirected_t, Properties, IdType>;

} // namespace gl
