// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/vertex_descriptor.hpp
/// @brief Defines the vertex_descriptor class, a lightweight wrapper for vertex representations in graphs.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/constants.hpp"
#include "gl/decl/graph_traits.hpp"
#include "gl/io/options.hpp"
#include "gl/io/options_manip.hpp"
#include "gl/traits.hpp"
#include "gl/types/core.hpp"
#include "gl/types/properties.hpp"

#include <compare>

namespace gl {

/// @ingroup GL-Core
/// @brief A lightweight wrapper around a vertex identifier with optional properties.
///
/// The `vertex_descriptor` class provides a type-safe and efficient way to represent
/// vertices in graph structures. It acts as a lightweight wrapper that combines
/// a unique identifier with optional property data, ensuring safe access and
/// comparison operations.
///
/// > [!WARNING] This class is not intended to be instantiated directly.
/// >
/// > Instead, `vertex_descriptor` objects should be retrieved from the @ref gl::graph "graph" class instance that owns the given vertex.
///
/// ### Example Usage
/// ```cpp
/// std::cout << gl::io::with_vertex_properties; // (1)!
/// for (auto v : graph.vertices()) {
///     if (v->parent == gl::invalid_id) // (2)!
///         v->level = 0;
///     else
///         v->level = graph.vertex(v->parent)->level + 1; // (3)!
///
///     std::cout << v << "\n"; // (4)!
/// }
/// ```
///
/// 1\. Apply the stream manipulator to ensure custom property data is included in the output.
///
/// 2\. Use the arrow operator `->` to read custom properties attached to the vertex (assuming the vertex properties type contains `parent` and `level` members).
///
/// 3\. Access another vertex via the `graph` using its ID, and modify the current vertex's properties.
///
/// 4\. Idiomatic output formatting.
///     *Example output:* `5[parent: 0, level: 1]`
///
/// ### Template Parameters
/// | Parameter  | Description | Default | Constraint |
/// | :--------- | :--- | :--- | :--- |
/// | Properties | The type of property data attached to the vertex. | @ref gl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType     | The underlying integer type used for the vertex ID. | @ref gl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
///
/// ### See Also
/// * @ref gl::edge_descriptor "edge_descriptor" : For the corresponding edge wrapper class.
/// * @ref gl::graph "graph" : For the owning graph class that manages vertex descriptors.
template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
class vertex_descriptor final {
public:
    /// @brief The vertex identifier type
    using id_type = IdType;
    /// @brief The type of properties associated with the vertex.
    using properties_type = Properties;

    /// @brief Default constructor, initializes to an invalid vertex.
    vertex_descriptor() {
        *this = vertex_descriptor::invalid();
    }

    /// @brief Constructs a vertex descriptor with the given ID (for empty properties).
    /// @param id The unique identifier for the vertex.
    explicit vertex_descriptor(const id_type id)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id) {}

    /// @brief Constructs a vertex descriptor with the given ID and properties.
    /// @param id The unique identifier for the vertex.
    /// @param properties A reference to the property data to associate with this vertex.
    explicit vertex_descriptor(const id_type id, properties_type& properties)
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _properties(properties) {}

    /// @brief Implicit converting constructor from a non-const descriptor to a const descriptor.
    /// @tparam NonConstProps The non-const property type.
    /// @param other The vertex descriptor to convert from.
    template <typename NonConstProperties>
    requires(std::same_as<Properties, const NonConstProperties>)
    vertex_descriptor(const vertex_descriptor<NonConstProperties, IdType>& other) noexcept
    : _id(other.id()) {
        if constexpr (traits::c_non_empty_properties<Properties>) {
            this->_properties = other.properties();
        }
    }

    /// @brief Returns an invalid vertex descriptor (for empty properties).
    /// @return A `vertex_descriptor` holding the `invalid_id`.
    [[nodiscard]] gl_attr_force_inline static vertex_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return vertex_descriptor(invalid_id);
    }

    /// @brief Returns an invalid vertex descriptor (for non-empty properties).
    /// @return A `vertex_descriptor` holding the `invalid_id` and empty properties.
    [[nodiscard]] gl_attr_force_inline static vertex_descriptor invalid() noexcept
    requires(traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return vertex_descriptor(invalid_id, invalid_properties);
    }

    /// @brief Copy constructor.
    vertex_descriptor(const vertex_descriptor&) = default;
    /// @brief Copy assignment operator.
    vertex_descriptor& operator=(const vertex_descriptor&) = default;

    /// @brief Move constructor.
    vertex_descriptor(vertex_descriptor&&) noexcept = default;
    /// @brief Move assignment operator.
    vertex_descriptor& operator=(vertex_descriptor&&) noexcept = default;

    /// @brief Destructor.
    ~vertex_descriptor() = default;

    /// @brief Cross-type equality comparison operator.
    /// @tparam OtherProperties The property type of the other descriptor.
    /// @param other The vertex descriptor to compare against.
    /// @return `true` if both descriptors hold the same ID, `false` otherwise.
    template <traits::c_properties OtherProperties>
    [[nodiscard]] gl_attr_force_inline bool operator==(
        const vertex_descriptor<OtherProperties, IdType>& other
    ) const noexcept {
        return this->_id == other.id();
    }

    /// @brief Cross-type three-way comparison operator.
    /// @tparam OtherProperties The property type of the other descriptor.
    /// @param other The vertex descriptor to compare against.
    /// @return The strong ordering result based on the underlying IDs.
    template <traits::c_properties OtherProperties>
    [[nodiscard]] gl_attr_force_inline std::strong_ordering operator<=>(
        const vertex_descriptor<OtherProperties, IdType>& other
    ) const noexcept {
        return this->_id <=> other.id();
    }

    /// @brief Boolean conversion operator.
    /// @return `true` if the vertex is valid.
    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    /// @brief Checks if the vertex descriptor is valid.
    /// @return `true` if the ID is not equal to `invalid_id`.
    [[nodiscard]] gl_attr_force_inline bool is_valid() const noexcept {
        return this->_id != invalid_id;
    }

    /// @brief Returns the vertex ID.
    /// @return The underlying integer ID of the vertex.
    [[nodiscard]] gl_attr_force_inline id_type id() const noexcept {
        return this->_id;
    }

    /// @brief Returns a reference to the vertex properties.
    /// @return A reference to the associated `properties_type`.
    /// @throws std::logic_error If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type& properties() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Arrow operator for accessing properties.
    /// @return A pointer to the associated `properties_type`.
    /// @throws std::logic_error If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type* operator->() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return &this->_properties.get();
    }

    /// @brief Dereference operator for accessing properties.
    /// @return A reference to the associated `properties_type`.
    /// @throws std::logic_error If the vertex descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type& operator*() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Output stream operator for vertex descriptors.
    /// @param os The output stream.
    /// @param vertex The vertex descriptor to write.
    /// @return A reference to the output stream.
    friend std::ostream& operator<<(std::ostream& os, const vertex_descriptor& vertex) {
        using enum io::detail::option_bit;

        if (io::is_option_set(os, verbose))
            return vertex._verbose_write(os);
        else
            return vertex._concise_write(os);
    }

private:
    [[noreturn]] void _throw_invalid_access() const {
        throw std::logic_error("Cannot access properties of an invalid vertex");
    }

    gl_attr_force_inline void _validate() const {
        if (not this->is_valid())
            this->_throw_invalid_access();
    }

    std::ostream& _verbose_write(std::ostream& os) const {
        using enum io::detail::option_bit;

        os << "[id: " << this->_id;
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_vertex_properties))
                os << " | " << this->_properties.get();
        os << ']';

        return os;
    }

    std::ostream& _concise_write(std::ostream& os) const {
        using enum io::detail::option_bit;

        os << this->_id;
        if constexpr (traits::c_writable<properties_type>)
            if (io::is_option_set(os, with_vertex_properties))
                os << '[' << this->_properties.get() << ']';

        return os;
    }

    id_type _id;
    [[no_unique_address]] std::conditional_t<
        traits::c_empty_properties<properties_type>,
        empty_properties,
        std::reference_wrapper<properties_type>> _properties;
};

} // namespace gl
