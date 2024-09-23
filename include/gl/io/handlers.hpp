#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/util/enum.hpp"

#include <iostream>

namespace gl::io {

using index_type = int;
using iword_type = long;
using bit_position_type = unsigned;

namespace detail {

template <typename T>
concept c_bit_position_enum =
    std::is_enum_v<T> and std::convertible_to<std::underlying_type_t<T>, bit_position_type>;

} // namespace detail

class stream_option_manipulator {
public:
    stream_option_manipulator() = delete;

    explicit stream_option_manipulator(bit_position_type bit_position, bool operation = set)
    : _bit_position(bit_position), _operation(operation) {}

    template <detail::c_bit_position_enum BitPosition>
    explicit stream_option_manipulator(BitPosition bit_position, bool operation = set)
    : _bit_position(_get_bit_position_value(bit_position)), _operation(operation) {}

    friend std::ostream& operator<<(
        std::ostream& os, const stream_option_manipulator& property_manipulator
    ) {
        if (property_manipulator._operation == set)
            os.iword(_iostream_property_map_index()) |=
                (_iword_bit << property_manipulator._bit_position);
        else
            os.iword(_iostream_property_map_index()) &=
                ~(_iword_bit << property_manipulator._bit_position);

        return os;
    }

    friend std::istream& operator>>(
        std::istream& is, const stream_option_manipulator& property_manipulator
    ) {
        if (property_manipulator._operation == set)
            is.iword(_iostream_property_map_index()) |=
                (_iword_bit << property_manipulator._bit_position);
        else
            is.iword(_iostream_property_map_index()) &=
                ~(_iword_bit << property_manipulator._bit_position);

        return is;
    }

    [[nodiscard]] gl_attr_force_inline static bool is_option_set(
        std::ios_base& stream, bit_position_type bit_position
    ) {
        return (stream.iword(_iostream_property_map_index()) & (_iword_bit << bit_position)) != 0;
    }

    template <detail::c_bit_position_enum BitPosition>
    [[nodiscard]] gl_attr_force_inline static bool is_option_set(
        std::ios_base& stream, BitPosition bit_position
    ) {
        return (stream.iword(_iostream_property_map_index())
                & (_iword_bit << _get_bit_position_value(bit_position)))
            != 0;
    }

    static constexpr bool set = true;
    static constexpr bool unset = false;

private:
    [[nodiscard]] gl_attr_force_inline static index_type _iostream_property_map_index() {
        static index_type index = std::ios_base::xalloc();
        return index;
    }

    template <detail::c_bit_position_enum BitPosition>
    [[nodiscard]] gl_attr_force_inline static bit_position_type _get_bit_position_value(
        BitPosition bit_position
    ) {
        return static_cast<bit_position_type>(util::to_underlying(bit_position));
    }

    static constexpr iword_type _iword_bit = 1ul;

    bit_position_type _bit_position;
    bool _operation;
};

[[nodiscard]] gl_attr_force_inline stream_option_manipulator
set_option(bit_position_type bit_position) {
    return stream_option_manipulator{bit_position, stream_option_manipulator::set};
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline stream_option_manipulator set_option(BitPosition bit_position) {
    return stream_option_manipulator{bit_position, stream_option_manipulator::set};
}

[[nodiscard]] gl_attr_force_inline stream_option_manipulator
unset_option(bit_position_type bit_position) {
    return stream_option_manipulator{bit_position, stream_option_manipulator::unset};
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline stream_option_manipulator unset_option(BitPosition bit_position
) {
    return stream_option_manipulator{bit_position, stream_option_manipulator::unset};
}

[[nodiscard]] gl_attr_force_inline bool is_option_set(
    std::ios_base& stream, bit_position_type bit_position
) {
    return stream_option_manipulator::is_option_set(stream, bit_position);
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline bool is_option_set(
    std::ios_base& stream, BitPosition bit_position
) {
    return stream_option_manipulator::is_option_set(stream, bit_position);
}

} // namespace gl::io
