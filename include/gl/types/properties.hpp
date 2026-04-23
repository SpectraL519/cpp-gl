// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/traits.hpp"

#include <any>
#include <iomanip>
#include <unordered_map>

namespace gl {

/// @ingroup GL GL-Core
/// @brief A tag struct representing no user-defined properties.
///
/// > [!IMPORTANT]
/// > - This type is used as a default `properties_type` for graph components that do not require any user-defined data.
/// > - It serves as a marker to indicate that the component is "property-less" and can be optimized accordingly.
struct empty_properties {};

/// @ingroup GL GL-Core
/// @brief A tag struct representing an empty property map.
struct empty_properties_map {};

/// @ingroup GL GL-Types
/// @brief A property struct providing a basic string-based naming facility.
struct name_property {
    /// @brief The underlying string type used for the name.
    using value_type = std::string;

    /// @brief The stored name string.
    value_type name;

    /// @brief Assigns a new name from a string view.
    name_property& operator=(std::string_view name) {
        this->name = name;
        return *this;
    }

    [[nodiscard]] bool operator==(const name_property&) const = default;
    [[nodiscard]] auto operator<=>(const name_property&) const = default;

    [[nodiscard]] bool operator==(const std::string_view name) const {
        return this->name == name;
    }

    [[nodiscard]] auto operator<=>(const std::string_view name) const {
        return this->name <=> name;
    }

    /// @brief Serializes the name property to an output stream using quoted formatting.
    friend std::ostream& operator<<(std::ostream& os, const name_property& property) {
        os << std::quoted(property.name);
        return os;
    }

    /// @brief Deserializes the name property from an input stream using quoted formatting.
    friend std::istream& operator>>(std::istream& is, name_property& property) {
        is >> std::quoted(property.name);
        return is;
    }
};

/// @ingroup GL GL-Types
/// @brief A type-safe container for heterogeneous properties stored by string keys.
///
/// Uses `std::any` and `std::unordered_map` to allow runtime attachment of
/// arbitrary data types to graph elements.
class dynamic_properties final {
public:
    using key_type = std::string;
    using value_type = std::any;
    using property_map_type = std::unordered_map<key_type, value_type>;

    dynamic_properties() = default;

    dynamic_properties(const dynamic_properties&) = default;
    dynamic_properties(dynamic_properties&&) noexcept = default;

    dynamic_properties& operator=(const dynamic_properties&) = default;
    dynamic_properties& operator=(dynamic_properties&&) noexcept = default;

    ~dynamic_properties() = default;

    /// @brief Checks if a property with the given key exists.
    [[nodiscard]] gl_attr_force_inline bool is_present(const key_type& key) const {
        return this->_property_map.contains(key);
    }

    /// @brief Retrieves a reference to a property cast to the specified type.
    /// @tparam ValueType The expected type of the property.
    /// @param key The string identifier for the property.
    /// @throws std::bad_any_cast If the property type does not match ValueType.
    template <typename ValueType>
    [[nodiscard]] ValueType& get(const key_type& key) {
        return std::any_cast<ValueType&>(this->_property_map.at(key));
    }

    /// @brief Sets or updates a property value.
    /// @tparam ValueType The type of value being stored.
    template <typename ValueType>
    requires(std::copy_constructible<ValueType>)
    void set(const key_type& key, const ValueType& value) {
        this->_property_map[key] = value;
    }

    /// @brief Moves a value into the property map.
    /// @tparam ValueType The type of value being stored.
    template <typename ValueType>
    requires(std::move_constructible<ValueType>)
    void mvset(const key_type& key, ValueType&& value) {
        this->_property_map[key] = std::forward<ValueType>(value);
    }

    /// @brief Constructs a property value in-place.
    /// @tparam ValueType The type of value to construct.
    template <typename ValueType, typename... Args>
    void emplace(const key_type& key, Args&&... args) {
        this->_property_map[key].emplace<ValueType>(std::forward<Args>(args)...);
    }

    /// @brief Removes the property associated with the given key.
    void remove(const key_type& key) {
        this->_property_map.erase(key);
    }

    /// @brief Returns a reference to the underlying property map.
    [[nodiscard]] gl_attr_force_inline property_map_type& underlying() {
        return this->_property_map;
    }

private:
    property_map_type _property_map;
};

// --- vertex properties ---

/// @ingroup GL GL-Types
/// @brief A specialized color property for algorithms requiring binary states (e.g., bipartition).
class binary_color final {
public:
    /// @brief The discrete states available for binary coloring.
    enum class value : std::uint8_t {
        black = static_cast<std::uint8_t>(0), ///< Represents the first color state.
        white = static_cast<std::uint8_t>(1), ///< Represents the second color state.
        unset = static_cast<std::uint8_t>(2), ///< Represents an uninitialized or neutral state.
    };

    binary_color() = default;

    binary_color(value value) : _value(_restrict(value)) {}

    binary_color(const binary_color&) = default;
    binary_color(binary_color&&) noexcept = default;

    binary_color& operator=(const binary_color&) = default;
    binary_color& operator=(binary_color&&) noexcept = default;

    ~binary_color() = default;

    binary_color& operator=(value value) {
        this->_value = this->_restrict(value);
        return *this;
    }

    [[nodiscard]] auto operator<=>(const binary_color&) const = default;
    [[nodiscard]] bool operator==(const binary_color&) const = default;

    /// @brief Returns `true` if the color is either Black or White (i.e., not Unset).
    gl_attr_force_inline operator bool() const {
        return this->is_set();
    }

    /// @brief Explicit check to see if the color state is not `unset`.
    [[nodiscard]] gl_attr_force_inline bool is_set() const {
        return this->_value < value::unset;
    }

    /// @brief Returns the integer representation of the current color state.
    [[nodiscard]] gl_attr_force_inline std::underlying_type_t<value> to_underlying() const {
        return std::to_underlying(this->_value);
    }

    /// @brief Returns a new `binary_color` representing the opposite state (Black <-> White).
    [[nodiscard]] gl_attr_force_inline binary_color next() const {
        return value{not this->to_underlying()};
    }

private:
    [[nodiscard]] value _restrict(const value value) {
        return std::min(value, value::unset);
    }

    value _value{value::unset};
};

/// @ingroup GL GL-Types
/// @brief Alias for the underlying `binary_color::value` enum.
using bin_color_value = typename binary_color::value;

/// @ingroup GL GL-Types
/// @brief A property struct wrapping a `binary_color`.
struct binary_color_property {
    using color_type = binary_color;
    color_type color;
};

/// @ingroup GL GL-Types
/// @brief A property struct providing arithmetic weight for edges or vertices.
///
/// ### Template Parameters
/// | Parameter | Description | Default | Constraint |
/// | :--- | :--- | :--- | :--- |
/// | `WeightType` | The numeric type used to store the weight value. | `double` | [c_arithmetic](gl_traits.md#gl-traits-c-arithmetic) |
template <traits::c_arithmetic WeightType = double>
struct weight_property {
    using weight_type = WeightType;

    /// @brief The stored numeric weight.
    weight_type weight = static_cast<weight_type>(1ll);

    friend std::ostream& operator<<(std::ostream& os, const weight_property& property)
    requires(traits::c_writable<weight_type>)
    {
        os << property.weight;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, weight_property& property)
    requires(traits::c_readable<weight_type>)
    {
        is >> property.weight;
        return is;
    }
};

namespace traits {

/// @ingroup GL GL-Traits
/// @brief Defines the minimal requirements for a type to be used as a property.
///
/// A valid property type must be **semiregular** (default constructible and copyable).
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_properties = std::semiregular<T>;

/// @ingroup GL GL-Traits
/// @brief Validates if a type is specifically the @ref gl::empty_properties tag.
///
/// This concept is used to specialize behavior for graph components that
/// do not carry any user-defined data.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_empty_properties = c_properties<T> and std::same_as<T, gl::empty_properties>;

/// @ingroup GL GL-Traits
/// @brief Validates if a property type contains actual user-defined data.
///
/// Requires that the type satisfies @ref gl::traits::c_properties and is not the @ref gl::empty_properties tag.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_non_empty_properties = c_properties<T> and not c_empty_properties<T>;

/// @ingroup GL GL-Traits
/// @brief Checks if a type/component has a nested `properties_type` that is the @ref gl::empty_properties tag.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_has_empty_properties =
    requires { typename T::properties_type; } and c_empty_properties<typename T::properties_type>;

/// @ingroup GL GL-Traits
/// @brief Checks if a type/component has a nested `properties_type` that is not the @ref gl::empty_properties tag.
///
/// @tparam T The type to evaluate against the concept.
template <typename T>
concept c_has_non_empty_properties = requires {
    typename T::properties_type;
} and not c_empty_properties<typename T::properties_type>;

/// @ingroup GL GL-Traits
/// @brief Requirements for properties that support binary coloring algorithms.
///
/// Requires a property type that:
/// 1. Satisfies @ref gl::traits::c_properties.
/// 2. Has a nested `color_type`.
/// 3. Has a public `color` member of the `color_type` type.
/// 4. Supports construction and comparison with @ref gl::binary_color.
///
/// @tparam T The type to evaluate against the concept.
template <typename Properties>
concept c_binary_color_properties_type = c_properties<Properties> and requires(Properties p) {
    typename Properties::color_type;
    { p.color } -> std::same_as<typename Properties::color_type&>;
    { p.color == binary_color{} } -> std::convertible_to<bool>;
    requires std::constructible_from<typename Properties::color_type, binary_color>;
};

/// @ingroup GL GL-Traits
/// @brief Requirements for properties that support arithmetic weight values.
///
/// Requires a property type that:
/// 1. Satisfies @ref gl::traits::c_properties.
/// 2. Has a nested `weight_type` that satisfies @ref gl::traits::c_arithmetic.
/// 3. Has a public `weight` member of the `weight_type` type.
///
/// @tparam T The type to evaluate against the concept.
template <typename Properties>
concept c_weight_properties_type = c_properties<Properties> and requires(Properties p) {
    typename Properties::weight_type;
    { p.weight } -> std::same_as<typename Properties::weight_type&>;
    requires c_arithmetic<typename Properties::weight_type>;
};

} // namespace traits

} // namespace gl
