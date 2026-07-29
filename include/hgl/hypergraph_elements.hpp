// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/hypergraph_elements.hpp
/// @brief Defines the hyperedge_descriptor class, element tags, and aliases for hypergraph elements.

#pragma once

#include "gl/traits.hpp"
#include "gl/types/core.hpp"
#include "gl/vertex_descriptor.hpp"
#include "hgl/constants.hpp"
#include "hgl/io/core.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

namespace hgl {

// --- hypergraph elements ---

/// @ingroup HGL-Core
/// @brief Type alias adapting the standard graph vertex descriptor for hypergraphs.
///
/// Because hypergraphs and standard graphs share the same fundamental vertex representation,
/// this alias imports the @ref gl::vertex_descriptor type into the HGL module. It provides a
/// type-safe wrapper combining a unique identifier with optional property data.
///
/// > [!WARNING] This class is not intended to be instantiated directly.
/// >
/// > Instead, `vertex_descriptor` objects should be retrieved from the @ref hgl::hypergraph class instance that owns the given vertex.
///
/// ### Example Usage
/// ```cpp
/// std::cout << gl::io::verbose << gl::io::with_vertex_properties; // (1)!
///
/// for (const auto& vertex : hypergraph.vertices()) {
///     const auto deg = hypergraph.degree(vertex); // (2)!
///
///     if (deg == 0uz)
///         vertex->description = "Isolated node"; // (3)!
///     else
///         vertex->description = std::format("Connected to {} hyperedges", deg);
///
///     std::cout << "Vertex details: " << vertex << '\n';
/// }
/// ```
///
/// 1. Stream manipulators inject persistent formatting state globally before iterating.
/// 2. Use the owning hypergraph to query topological information about the vertex.
/// 3. Safely access and modify the underlying properties via the `->` operator (assuming the vertex properties type contains a `description` member).
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :--- | :--- | :--- |
/// | Properties | The type of property data attached to the vertex. | @ref hgl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType | The underlying integer type used for the IDs. | @ref hgl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
///
/// ### See Also
/// - @ref gl::vertex_descriptor for the full definition of the type.
/// - @ref hgl::hyperedge_descriptor "hyperedge_descriptor" for the corresponding hyperedge wrapper class.
/// - @ref hgl::hypergraph "hypergraph" for the owning hypergraph class that manages vertex descriptors.
template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
using vertex_descriptor = gl::vertex_descriptor<Properties, IdType>;

/// @ingroup HGL-Core
/// @brief A lightweight wrapper representing a hypergraph edge with optional properties.
///
/// The `hyperedge_descriptor` class provides a type-safe and efficient way to represent
/// hyperedges in hypergraph structures. It acts as a lightweight wrapper that encapsulates
/// the unique identifier of the hyperedge and its optional property data, ensuring safe
/// access and comparison operations.
///
/// > [!WARNING] This class is not intended to be instantiated directly.
/// >
/// > Instead, `hyperedge_descriptor` objects should be retrieved from the @ref hgl::hypergraph class instance that owns the given hyperedge.
///
/// ### Example Usage
/// ```cpp
/// std::cout << gl::io::verbose << gl::io::with_hyperedge_properties; // (1)!
///
/// for (const auto& hyperedge : hypergraph.hyperedges()) {
///     const auto size = hypergraph.hyperedge_size(hyperedge); // (2)!
///     hyperedge->weight = static_cast<double>(size) * 1.5; // (3)!
///     std::cout << "Hyperedge details: " << hyperedge << '\n';
/// }
/// ```
///
/// 1. Stream manipulators like `with_hyperedge_properties` apply persistently to `std::cout`.
/// 2. Use the owning hypergraph to query topological information about the hyperedge.
/// 3. Update the payload properties through the overloaded `->` operator (assuming the hyperedge properties type contains a `weight` member).
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :-------- | :--- | :--- | :--- |
/// | Properties | The type of property data attached to the hyperedge. | @ref hgl::empty_properties "empty_properties" | [**c_properties**](gl_concepts.md#gl-traits-c-properties) |
/// | IdType | The underlying integer type used for the IDs. | @ref hgl::default_id_type "default_id_type" | [**c_id_type**](gl_concepts.md#gl-traits-c-id-type) |
///
/// ### See Also
/// - @ref hgl::vertex_descriptor "vertex_descriptor" for the corresponding vertex wrapper class.
/// - @ref hgl::hypergraph "hypergraph" for the owning hypergraph class that manages hyperedge descriptors.
template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
class hyperedge_descriptor final {
public:
    /// @brief Self type alias.
    using type = hyperedge_descriptor<Properties>;
    /// @brief The underlying integer type used for the hyperedge identifier.
    using id_type = IdType;
    /// @brief The type of property data attached to the hyperedge.
    using properties_type = Properties;

    /// @brief Default constructor creating an invalid hyperedge descriptor.
    hyperedge_descriptor() {
        *this = hyperedge_descriptor::invalid();
    }

    /// @brief Constructs a property-less hyperedge descriptor from a raw ID.
    /// @param id The raw identifier of the hyperedge.
    explicit hyperedge_descriptor(const id_type id)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id) {}

    /// @brief Constructs a hyperedge descriptor binding an ID to its properties.
    /// @param id The raw identifier of the hyperedge.
    /// @param properties A reference to the underlying properties payload.
    explicit hyperedge_descriptor(const id_type id, properties_type& properties)
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _properties(properties) {}

    /// @brief Implicit converting constructor from a non-const descriptor to a const descriptor.
    /// @tparam MutProperties The mutable property type.
    /// @param other The hyperedge descriptor to convert from.
    template <typename MutProperties>
    requires(std::same_as<Properties, const MutProperties>)
    hyperedge_descriptor(const hyperedge_descriptor<MutProperties, IdType>& other) noexcept
    : _id(other.id()) {
        if constexpr (traits::c_non_empty_properties<Properties>) {
            this->_properties = other.properties();
        }
    }

    /// @brief Returns a special descriptor representing an invalid or uninitialized property-less hyperedge.
    /// @return An invalid `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline static hyperedge_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return hyperedge_descriptor(invalid_id);
    }

    /// @brief Returns a special descriptor representing an invalid or uninitialized hyperedge with properties.
    /// @return An invalid `hyperedge_descriptor`.
    [[nodiscard]] gl_attr_force_inline static hyperedge_descriptor invalid() noexcept
    requires(traits::c_non_empty_properties<properties_type>)
    {
        static properties_type invalid_properties{};
        return hyperedge_descriptor(invalid_id, invalid_properties);
    }

    /// @brief Default copy constructor.
    hyperedge_descriptor(const hyperedge_descriptor&) = default;
    /// @brief Default copy assignment operator.
    hyperedge_descriptor& operator=(const hyperedge_descriptor&) = default;

    /// @brief Default move constructor.
    hyperedge_descriptor(hyperedge_descriptor&&) noexcept = default;
    /// @brief Default move assignment operator.
    hyperedge_descriptor& operator=(hyperedge_descriptor&&) noexcept = default;

    /// @brief Default destructor.
    ~hyperedge_descriptor() = default;

    /// @brief Compares two hyperedge descriptors for equality.
    /// @tparam OtherProperties The property type of the other descriptor.
    /// @param other The descriptor to compare against.
    /// @return `true` if both descriptors hold the same ID, `false` otherwise.
    template <traits::c_properties OtherProperties>
    requires(std::same_as<std::remove_cv_t<properties_type>, std::remove_cv_t<OtherProperties>>)
    [[nodiscard]] bool operator==(const hyperedge_descriptor<OtherProperties, id_type>& other
    ) const noexcept {
        return this->_id == other.id();
    }

    /// @brief Compares two hyperedge descriptors to establish a strict ordering based on their IDs.
    /// @tparam OtherProperties The property type of the other descriptor.
    /// @param other The descriptor to compare against.
    /// @return The result of the three-way comparison between the underlying IDs.
    template <traits::c_properties OtherProperties>
    requires(std::same_as<std::remove_cv_t<properties_type>, std::remove_cv_t<OtherProperties>>)
    [[nodiscard]] gl_attr_force_inline std::strong_ordering operator<=>(
        const hyperedge_descriptor<OtherProperties, id_type>& other
    ) const noexcept {
        return this->_id <=> other.id();
    }

    /// @brief Contextually converts the descriptor to a boolean.
    /// @return `true` if the descriptor is valid, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    /// @brief Checks if the descriptor represents a valid hyperedge.
    /// @return `true` if the underlying ID is not the invalid ID constant, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline bool is_valid() const noexcept {
        return this->_id != invalid_id;
    }

    /// @brief Retrieves the raw underlying ID of the hyperedge.
    /// @return The hyperedge's integer ID.
    [[nodiscard]] gl_attr_force_inline id_type id() const noexcept {
        return this->_id;
    }

    /// @brief Retrieves a reference to the property payload attached to the hyperedge.
    /// @return A mutable reference to the underlying properties.
    /// @throws std::logic_error If the descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type& properties() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Arrow operator providing direct access to the hyperedge's properties.
    /// @return A pointer to the underlying properties.
    /// @throws std::logic_error If the descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type* operator->() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return &this->_properties.get();
    }

    /// @brief Dereference operator providing direct access to the hyperedge's properties.
    /// @return A reference to the underlying properties.
    /// @throws std::logic_error If the descriptor is invalid.
    [[nodiscard]] gl_attr_force_inline properties_type& operator*() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Serializes the hyperedge descriptor to an output stream.
    ///
    /// Depending on active stream flags, this outputs the descriptor in verbose or concise formats.
    ///
    /// @param os The target output stream.
    /// @param hyperedge The hyperedge descriptor to format.
    /// @return The stream reference for chaining.
    friend std::ostream& operator<<(std::ostream& os, const hyperedge_descriptor& hyperedge) {
        using enum io::detail::option_bit;

        if (io::is_option_set(os, verbose))
            return hyperedge._verbose_write(os);
        else
            return hyperedge._concise_write(os);
    }

private:
    [[noreturn]] void _throw_invalid_access() const {
        throw std::logic_error("Cannot access properties of an invalid hyperedge");
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

// --- hypergraph element tags ---

/// @ingroup HGL-Core
/// @brief Tag type representing a vertex element in a hypergraph.
struct vertex_tag {};

/// @ingroup HGL-Core
/// @brief Tag type representing a hyperedge element in a hypergraph.
struct hyperedge_tag {};

/// @ingroup HGL-Core
/// @brief A constant instance of `vertex_tag` used for tagging and generic dispatching.
inline constexpr vertex_tag vertex{};
/// @ingroup HGL-Core
/// @brief A constant instance of `hyperedge_tag` used for tagging and generic dispatching.
inline constexpr hyperedge_tag hyperedge{};

namespace traits {

/// @ingroup HGL-Traits
/// @brief Validates if a type is a valid hypergraph element tag.
///
/// The valid hypergraph element tags are @ref hgl::vertex_tag "vertex_tag" and @ref hgl::hyperedge_tag "hyperedge_tag".
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_hypergraph_element_tag = c_one_of<T, vertex_tag, hyperedge_tag>;

} // namespace traits
} // namespace hgl
