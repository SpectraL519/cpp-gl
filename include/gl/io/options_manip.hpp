// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/traits.hpp"

#include <iostream>
#include <utility>

namespace gl::io {

using index_type = int;
using iword_type = long;
using bit_position_type = unsigned;

inline constexpr iword_type iword_bit = 1ul;

class options_manip {
public:
    options_manip() = delete;

    constexpr explicit options_manip(iword_type set_mask, iword_type clear_mask = 0ul)
    : _set_mask(set_mask), _clear_mask(clear_mask) {}

    friend std::ostream& operator<<(std::ostream& os, const options_manip& manip) {
        os.iword(_iostream_property_map_index()) &= ~manip._clear_mask;
        os.iword(_iostream_property_map_index()) |= manip._set_mask;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, const options_manip& manip) {
        is.iword(_iostream_property_map_index()) &= ~manip._clear_mask;
        is.iword(_iostream_property_map_index()) |= manip._set_mask;
        return is;
    }

    [[nodiscard]] gl_attr_force_inline static bool is_option_set(
        std::ios_base& stream, bit_position_type bit_position
    ) {
        return (stream.iword(_iostream_property_map_index()) & (iword_bit << bit_position)) != 0;
    }

    [[nodiscard]] gl_attr_force_inline static bool is_option_set(
        std::ios_base& stream, traits::c_enum auto bit
    ) {
        return is_option_set(stream, static_cast<bit_position_type>(bit));
    }

    [[nodiscard]] gl_attr_force_inline static bool are_options_set(
        std::ios_base& stream, iword_type bitmask
    ) {
        return (stream.iword(_iostream_property_map_index()) & bitmask) == bitmask;
    }

private:
    [[nodiscard]] gl_attr_force_inline static index_type _iostream_property_map_index() {
        static index_type index = std::ios_base::xalloc();
        return index;
    }

    iword_type _set_mask;
    iword_type _clear_mask;
};

namespace detail {

// clang-format off

template <typename... Args>
[[nodiscard]] constexpr iword_type build_mask(Args... bits) {
    return ((iword_bit << static_cast<bit_position_type>(bits)) | ...);
}

// clang-format on

template <typename T>
[[nodiscard]] constexpr iword_type build_mask_from(std::initializer_list<T> bits) {
    iword_type mask = 0ul;
    for (auto b : bits)
        mask |= iword_bit << static_cast<bit_position_type>(b);
    return mask;
}

} // namespace detail

template <typename... Args>
[[nodiscard]] constexpr options_manip set_options(Args... bits) {
    return options_manip{detail::build_mask(bits...), 0ul};
}

template <typename T>
[[nodiscard]] constexpr options_manip set_options(std::initializer_list<T> bits) {
    return options_manip{detail::build_mask_from(bits), 0ul};
}

template <typename... Args>
[[nodiscard]] constexpr options_manip clear_options(Args... bits) {
    return options_manip{0ul, detail::build_mask(bits...)};
}

template <typename T>
[[nodiscard]] constexpr options_manip clear_options(std::initializer_list<T> bits) {
    return options_manip{0ul, detail::build_mask_from(bits)};
}

[[nodiscard]] gl_attr_force_inline bool is_option_set(
    std::ios_base& stream, bit_position_type bit_position
) {
    return options_manip::is_option_set(stream, bit_position);
}

[[nodiscard]] gl_attr_force_inline bool is_option_set(
    std::ios_base& stream, traits::c_enum auto bit
) {
    return options_manip::is_option_set(stream, bit);
}

[[nodiscard]] gl_attr_force_inline bool are_options_set(std::ios_base& stream, iword_type bitmask) {
    return options_manip::are_options_set(stream, bitmask);
}

template <typename... Args>
[[nodiscard]] gl_attr_force_inline bool are_options_set(std::ios_base& stream, Args... bits) {
    return options_manip::are_options_set(stream, detail::build_mask(bits...));
}

template <typename T>
[[nodiscard]] gl_attr_force_inline bool are_options_set(
    std::ios_base& stream, std::initializer_list<T> bits
) {
    return options_manip::are_options_set(stream, detail::build_mask_from(bits));
}

} // namespace gl::io
