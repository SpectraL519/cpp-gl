// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file vertex_descriptor.hpp
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

/// @ingroup GL GL-Core
/// @brief A lightweight wrapper around a vertex identifier with optional properties.
///
/// **Module:** Part of the @ref GL-Core "Core Graph Components" group.
///
/// The vertex_descriptor class provides a type-safe and efficient way to represent
/// vertices in graph structures. It acts as a lightweight wrapper that combines
/// a unique identifier with optional property data, ensuring safe access and
/// comparison operations.
template <
    traits::c_properties Properties = empty_properties,
    traits::c_id_type IdType = default_id_type>
class vertex_descriptor final {
public:
    /// @brief Type alias for the vertex_descriptor itself.
    using type = std::type_identity_t<vertex_descriptor<Properties, IdType>>;
    /// @brief The type used for vertex identifiers.
    using id_type = IdType;
    /// @brief The type of properties associated with the vertex.
    using properties_type = Properties;

    /// @brief Default constructor, initializes to an invalid vertex.
    vertex_descriptor() {
        *this = vertex_descriptor::invalid();
    }

    /// @brief Constructs a vertex descriptor with the given ID (for empty properties).
    explicit vertex_descriptor(const id_type id)
    requires(traits::c_empty_properties<properties_type>)
    : _id(id) {}

    /// @brief Constructs a vertex descriptor with the given ID and properties.
    explicit vertex_descriptor(const id_type id, properties_type& properties)
    requires(traits::c_non_empty_properties<properties_type>)
    : _id(id), _properties(properties) {}

    /// @brief Returns an invalid vertex descriptor (for empty properties).
    [[nodiscard]] gl_attr_force_inline static vertex_descriptor invalid() noexcept
    requires(traits::c_empty_properties<properties_type>)
    {
        return vertex_descriptor(invalid_id);
    }

    /// @brief Returns an invalid vertex descriptor (for non-empty properties).
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

    /// @brief Equality comparison operator.
    [[nodiscard]] gl_attr_force_inline bool operator==(const vertex_descriptor& other
    ) const noexcept {
        return this->_id == other._id;
    }

    /// @brief Three-way comparison operator.
    [[nodiscard]] gl_attr_force_inline std::strong_ordering operator<=>(
        const vertex_descriptor& other
    ) const noexcept {
        return this->_id <=> other._id;
    }

    /// @brief Boolean conversion operator, returns true if the vertex is valid.
    [[nodiscard]] gl_attr_force_inline operator bool() const noexcept {
        return this->is_valid();
    }

    /// @brief Checks if the vertex descriptor is valid.
    [[nodiscard]] gl_attr_force_inline bool is_valid() const noexcept {
        return this->_id != invalid_id;
    }

    /// @brief Returns the vertex ID.
    [[nodiscard]] gl_attr_force_inline id_type id() const noexcept {
        return this->_id;
    }

    /// @brief Returns a reference to the vertex properties.
    [[nodiscard]] gl_attr_force_inline properties_type& properties() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Arrow operator for accessing properties.
    [[nodiscard]] gl_attr_force_inline properties_type* operator->() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return &this->_properties.get();
    }

    /// @brief Dereference operator for accessing properties.
    [[nodiscard]] gl_attr_force_inline properties_type& operator*() const
    requires(traits::c_non_empty_properties<properties_type>)
    {
        this->_validate();
        return this->_properties.get();
    }

    /// @brief Output stream operator for vertex descriptors.
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
