// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/util/enum.hpp"
#include "traits/concepts.hpp"

#include <any>
#include <iomanip>
#include <unordered_map>
#include <variant>

#ifdef GL_CONFIG_PROPERTY_TYPES_NOT_FINAL
#define _GL_PROPERTY_TYPES_NOT_FINAL
#else
#undef _GL_PROPERTY_TYPES_NOT_FINAL
#endif

namespace gl {

namespace types {

// --- common properties ---

using empty_properties = std::monostate;

class name_property
#ifndef _GL_PROPERTY_TYPES_NOT_FINAL
    final
#endif
{
public:
    using value_type = std::string;

    name_property() = default;

    name_property(std::string_view name) : _name(name) {}

    name_property(const name_property&) = default;
    name_property(name_property&&) = default;

    name_property& operator=(const name_property&) = default;
    name_property& operator=(name_property&&) = default;

#ifndef _GL_PROPERTY_TYPES_NOT_FINAL
    ~name_property() = default;
#else
    virtual ~name_property() = default;
#endif

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline const std::string& name() const {
        return this->_name;
    }

    // clang-format on

    [[nodiscard]] bool operator==(const name_property&) const = default;
    [[nodiscard]] auto operator<=>(const name_property&) const = default;

    friend std::ostream& operator<<(std::ostream& os, const name_property& property) {
        os << std::quoted(property._name);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, name_property& property) {
        is >> std::quoted(property._name);
        return is;
    }

private:
    std::string _name;
};

class dynamic_properties
#ifndef _GL_PROPERTY_TYPES_NOT_FINAL
    final
#endif
{
public:
    using key_type = std::string;
    using value_type = std::any;
    using property_map_type = std::unordered_map<key_type, value_type>;

    dynamic_properties() = default;

    dynamic_properties(const dynamic_properties&) = default;
    dynamic_properties(dynamic_properties&&) = default;

    dynamic_properties& operator=(const dynamic_properties&) = default;
    dynamic_properties& operator=(dynamic_properties&&) = default;

#ifndef _GL_PROPERTY_TYPES_NOT_FINAL
    ~dynamic_properties() = default;
#else
    virtual ~dynamic_properties() = default;
#endif

    [[nodiscard]] gl_attr_force_inline bool is_present(const key_type& key) const {
        return this->_property_map.contains(key);
    }

    template <typename ValueType>
    [[nodiscard]] ValueType& get(const key_type& key) {
        return std::any_cast<ValueType&>(this->_property_map.at(key));
    }

    template <typename ValueType>
    requires(std::copy_constructible<ValueType>)
    void set(const key_type& key, const ValueType& value) {
        this->_property_map[key] = value;
    }

    template <typename ValueType>
    requires(std::move_constructible<ValueType>)
    void mvset(const key_type& key, ValueType&& value) {
        this->_property_map[key] = std::move(value);
    }

    template <typename ValueType, typename... Args>
    void emplace(const key_type& key, Args&&... args) {
        this->_property_map[key].emplace<ValueType>(std::forward<Args>(args)...);
    }

    void remove(const key_type& key) {
        this->_property_map.erase(key);
    }

    [[nodiscard]] gl_attr_force_inline property_map_type& underlying() {
        return this->_property_map;
    }

private:
    property_map_type _property_map;
};

// --- vertex properties ---

class binary_color
#ifndef _GL_PROPERTY_TYPES_NOT_FINAL
    final
#endif
{
public:
    enum class value : std::uint16_t {
        black = static_cast<std::uint16_t>(0),
        white = static_cast<std::uint16_t>(1),
        unset = static_cast<std::uint16_t>(2),
    };

    binary_color() = default;

    binary_color(value value) : _value(_restrict(value)) {}

    binary_color(const binary_color&) = default;
    binary_color(binary_color&&) = default;

    binary_color& operator=(const binary_color&) = default;
    binary_color& operator=(binary_color&&) = default;

#ifndef _GL_PROPERTY_TYPES_NOT_FINAL
    ~binary_color() = default;
#else
    virtual ~binary_color() = default;
#endif

    binary_color& operator=(value value) {
        this->_value = this->_restrict(value);
        return *this;
    }

    [[nodiscard]] auto operator<=>(const binary_color&) const = default;
    [[nodiscard]] bool operator==(const binary_color&) const = default;

    gl_attr_force_inline operator bool() const {
        return this->is_set();
    }

    [[nodiscard]] gl_attr_force_inline bool is_set() const {
        return this->_value < value::unset;
    }

    [[nodiscard]] gl_attr_force_inline std::underlying_type_t<value> to_underlying() const {
        return util::to_underlying(this->_value);
    }

    [[nodiscard]] gl_attr_force_inline binary_color next() const {
        return value{not this->to_underlying()};
    }

    // TODO: iostream operators

private:
    [[nodiscard]] value _restrict(const value value) {
        return std::min(value, value::unset);
    }

    value _value{value::unset};
};

struct binary_color_property {
    using color_type = binary_color;
    color_type color;
};

// --- edge properties ---

template <type_traits::c_basic_arithmetic WeightType = double>
struct weight_property {
    using weight_type = WeightType;
    weight_type weight = static_cast<weight_type>(1ll);

    friend std::ostream& operator<<(std::ostream& os, const weight_property& property)
    requires(type_traits::c_writable<weight_type>)
    {
        os << property.weight;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, weight_property& property)
    requires(type_traits::c_readable<weight_type>)
    {
        is >> property.weight;
        return is;
    }
};

} // namespace types

using bin_color_value = typename types::binary_color::value;

namespace type_traits {

template <c_properties Properties>
constexpr inline bool is_default_properties_type_v =
    std::is_same_v<Properties, gl::types::empty_properties>;

template <typename Properties>
concept c_binary_color_properties_type = c_properties<Properties> and requires(Properties p) {
    typename Properties::color_type;
    { p.color } -> std::same_as<typename Properties::color_type&>;
    { p.color == types::binary_color{} } -> std::convertible_to<bool>;
    requires std::constructible_from<typename Properties::color_type, types::binary_color>;
};

template <typename Properties>
concept c_weight_properties_type = c_properties<Properties> and requires(Properties p) {
    typename Properties::weight_type;
    { p.weight } -> std::same_as<typename Properties::weight_type&>;
    requires c_basic_arithmetic<typename Properties::weight_type>;
};

} // namespace type_traits

} // namespace gl
