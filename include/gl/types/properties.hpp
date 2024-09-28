#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/util/enum.hpp"
#include "traits/concepts.hpp"

#include <any>
#include <iomanip>
#include <unordered_map>
#include <variant>

namespace gl {

namespace types {

// --- common properties ---

using empty_properties = std::monostate;

class name_property {
public:
    using value_type = std::string;

    name_property() = default;

    name_property(std::string_view name) : _name(name) {}

    name_property(const name_property&) = default;
    name_property(name_property&&) = default;

    name_property& operator=(const name_property&) = default;
    name_property& operator=(name_property&&) = default;

    ~name_property() = default;

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

class dynamic_properties {
public:
    using key_type = std::string;
    using value_type = std::any;
    using property_map_type = std::unordered_map<key_type, value_type>;

    dynamic_properties() = default;

    dynamic_properties(const dynamic_properties&) = default;
    dynamic_properties(dynamic_properties&&) = default;

    dynamic_properties& operator=(const dynamic_properties&) = default;
    dynamic_properties& operator=(dynamic_properties&&) = default;

    ~dynamic_properties() = default;

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

class binary_color {
public:
    enum class value : std::uint8_t {
        black = static_cast<std::uint8_t>(0),
        white = static_cast<std::uint8_t>(1),
        unset = static_cast<std::uint8_t>(2),
    };

    binary_color() = default;

    binary_color(value value) : _value(_restrict(value)) {}

    binary_color(const binary_color&) = default;
    binary_color(binary_color&&) = default;

    binary_color& operator=(const binary_color&) = default;
    binary_color& operator=(binary_color&&) = default;

    ~binary_color() = default;

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
    weight_type weight;
};

} // namespace types

// TODO: rename -> bin_color_value
using bin_color_value = typename types::binary_color::value;

namespace type_traits {

template <c_properties Properties>
constexpr inline bool is_default_properties_type_v =
    std::is_same_v<Properties, gl::types::empty_properties>;

} // namespace type_traits

} // namespace gl

/*

for future reference:

template <typename Properties>
concept c_weight_property = requires(Properties p) {
    typename Properties::weight_type;
    requires c_basic_arithmetic<typename Properties::weight_type>;
    { p.weight } -> std::same_as<typename Properties::weight_type&>;
};

*/
