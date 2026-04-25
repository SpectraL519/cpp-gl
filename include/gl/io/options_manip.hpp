// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/io/options_manip.hpp
/// @brief Defines custom I/O stream manipulators for configuring serialization and deserialization options.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/traits.hpp"

#include <iostream>
#include <utility>

namespace gl::io {

/// @ingroup GL GL-IO
/// @brief Type used for standard I/O stream custom index allocation (`std::ios_base::xalloc`).
using index_type = int;

/// @ingroup GL GL-IO
/// @brief Type used for storing custom option flags within standard I/O streams.
using iword_type = long;

/// @ingroup GL GL-IO
/// @brief Type representing the zero-indexed position of a specific option bit.
using bit_position_type = unsigned;

/// @ingroup GL GL-IO
/// @brief Base bit representing the first position in an `iword` flag map.
inline constexpr iword_type iword_bit = 1ul;

/// @ingroup GL GL-IO
/// @brief A custom stream manipulator for modifying formatting options on standard I/O streams.
///
/// Utilizing `std::ios_base::xalloc` and the `iword` map, this class safely injects custom formatting
/// state directly into C++ standard streams. It acts as an intermediate proxy object produced by
/// functions like @ref gl::io::set_options "set_options" and @ref gl::io::clear_options "clear_options".
class options_manip {
public:
    /// @brief Default constructor is disabled.
    options_manip() = delete;

    /// @brief Constructs a manipulator with specific state masks.
    /// @param set_mask The bitmask of options to enable.
    /// @param clear_mask The bitmask of options to disable (defaults to 0).
    constexpr explicit options_manip(iword_type set_mask, iword_type clear_mask = 0ul)
    : _set_mask(set_mask), _clear_mask(clear_mask) {}

    /// @brief Applies the manipulator's formatting masks to an output stream.
    /// @param os The target output stream.
    /// @param manip The manipulator containing the masks to apply.
    /// @return The stream reference for chaining.
    friend std::ostream& operator<<(std::ostream& os, const options_manip& manip) {
        os.iword(_iostream_property_map_index()) &= ~manip._clear_mask;
        os.iword(_iostream_property_map_index()) |= manip._set_mask;
        return os;
    }

    /// @brief Applies the manipulator's formatting masks to an input stream.
    /// @param is The target input stream.
    /// @param manip The manipulator containing the masks to apply.
    /// @return The stream reference for chaining.
    friend std::istream& operator>>(std::istream& is, const options_manip& manip) {
        is.iword(_iostream_property_map_index()) &= ~manip._clear_mask;
        is.iword(_iostream_property_map_index()) |= manip._set_mask;
        return is;
    }

    /// @brief Checks if a specific option bit is currently enabled on the given stream.
    /// @param stream The stream to check.
    /// @param bit_position The numeric position of the bit to verify.
    /// @return `true` if the specified bit is set, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline static bool is_option_set(
        std::ios_base& stream, bit_position_type bit_position
    ) {
        return (stream.iword(_iostream_property_map_index()) & (iword_bit << bit_position)) != 0;
    }

    /// @brief Checks if a specific enum-based option is currently enabled on the given stream.
    /// @param stream The stream to check.
    /// @param bit The scoped enum value representing the bit to verify.
    /// @return `true` if the specified bit is set, `false` otherwise.
    [[nodiscard]] gl_attr_force_inline static bool is_option_set(
        std::ios_base& stream, traits::c_enum auto bit
    ) {
        return is_option_set(stream, static_cast<bit_position_type>(bit));
    }

    /// @brief Checks if a specific sequence of bits matches the exact stream state.
    /// @param stream The stream to check.
    /// @param bitmask The bitmask to verify against the stream's state.
    /// @return `true` if all bits within the mask are strictly set, `false` otherwise.
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

/// @ingroup GL GL-IO
/// @brief Creates a stream manipulator that enables the specified option bits.
/// @tparam Args Variadic template arguments representing the bits to set.
/// @param bits The specific options/bits to enable.
/// @return An @ref gl::io::options_manip "options_manip" object ready to be piped into a stream.
template <typename... Args>
[[nodiscard]] constexpr options_manip set_options(Args... bits) {
    return options_manip{detail::build_mask(bits...), 0ul};
}

/// @ingroup GL GL-IO
/// @brief Creates a stream manipulator that enables the specified option bits from a list.
/// @tparam T The type of the bits.
/// @param bits An initializer list containing the options/bits to enable.
/// @return An @ref gl::io::options_manip "options_manip" object ready to be piped into a stream.
template <typename T>
[[nodiscard]] constexpr options_manip set_options(std::initializer_list<T> bits) {
    return options_manip{detail::build_mask_from(bits), 0ul};
}

/// @ingroup GL GL-IO
/// @brief Creates a stream manipulator that disables the specified option bits.
/// @tparam Args Variadic template arguments representing the bits to clear.
/// @param bits The specific options/bits to disable.
/// @return An @ref gl::io::options_manip "options_manip" object ready to be piped into a stream.
template <typename... Args>
[[nodiscard]] constexpr options_manip clear_options(Args... bits) {
    return options_manip{0ul, detail::build_mask(bits...)};
}

/// @ingroup GL GL-IO
/// @brief Creates a stream manipulator that disables the specified option bits from a list.
/// @tparam T The type of the bits.
/// @param bits An initializer list containing the options/bits to disable.
/// @return An @ref gl::io::options_manip "options_manip" object ready to be piped into a stream.
template <typename T>
[[nodiscard]] constexpr options_manip clear_options(std::initializer_list<T> bits) {
    return options_manip{0ul, detail::build_mask_from(bits)};
}

/// @ingroup GL GL-IO
/// @brief Convenience wrapper to check if a specific option bit is set on the stream.
/// @param stream The stream to check.
/// @param bit_position The numeric position of the bit to verify.
/// @return `true` if the specified bit is set, `false` otherwise.
[[nodiscard]] gl_attr_force_inline bool is_option_set(
    std::ios_base& stream, bit_position_type bit_position
) {
    return options_manip::is_option_set(stream, bit_position);
}

/// @ingroup GL GL-IO
/// @brief Convenience wrapper to check if a specific enum-based option is set on the stream.
/// @param stream The stream to check.
/// @param bit The scoped enum value representing the bit to verify.
/// @return `true` if the specified bit is set, `false` otherwise.
[[nodiscard]] gl_attr_force_inline bool is_option_set(
    std::ios_base& stream, traits::c_enum auto bit
) {
    return options_manip::is_option_set(stream, bit);
}

/// @ingroup GL GL-IO
/// @brief Convenience wrapper to check if a combined bitmask of options is set on the stream.
/// @param stream The stream to check.
/// @param bitmask The exact bitmask to verify.
/// @return `true` if all bits within the mask are strictly set, `false` otherwise.
[[nodiscard]] gl_attr_force_inline bool are_options_set(std::ios_base& stream, iword_type bitmask) {
    return options_manip::are_options_set(stream, bitmask);
}

/// @ingroup GL GL-IO
/// @brief Convenience wrapper to check if multiple specific options are simultaneously set.
/// @tparam Args Variadic template arguments representing the bits.
/// @param stream The stream to check.
/// @param bits The specific options/bits to verify.
/// @return `true` if all specified bits are set, `false` otherwise.
template <typename... Args>
[[nodiscard]] gl_attr_force_inline bool are_options_set(std::ios_base& stream, Args... bits) {
    return options_manip::are_options_set(stream, detail::build_mask(bits...));
}

/// @ingroup GL GL-IO
/// @brief Convenience wrapper to check if multiple specific options from a list are simultaneously set.
/// @tparam T The type of the elements in the initializer list.
/// @param stream The stream to check.
/// @param bits An initializer list containing the options/bits to verify.
/// @return `true` if all specified bits are set, `false` otherwise.
template <typename T>
[[nodiscard]] gl_attr_force_inline bool are_options_set(
    std::ios_base& stream, std::initializer_list<T> bits
) {
    return options_manip::are_options_set(stream, detail::build_mask_from(bits));
}

} // namespace gl::io
