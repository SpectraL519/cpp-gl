#pragma once

#include "gl/attributes/force_inline.hpp"
#include "traits/concepts.hpp"

#include <iomanip>
#include <variant>
#include <any>
#include <unordered_map>

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

template <typename Color>
requires(std::is_enum_v<Color>)
struct color_property {
    using color_type = Color;
    color_type color;
};

enum class binary_color : bool {
    black = static_cast<bool>(0),
    white = static_cast<bool>(1)
};

using binary_color_property = color_property<binary_color>;

enum class ext_binary_color : std::uint8_t {
    black = static_cast<uint8_t>(0),
    white = static_cast<uint8_t>(1),
    unset = static_cast<uint8_t>(2),
};

using ext_binary_color_property = color_property<ext_binary_color>;

// --- edge properties ---

template <type_traits::c_basic_arithmetic WeightType>
struct weight_property {
    using weight_type = WeightType;
    weight_type weight;
};

} // namespace types

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
