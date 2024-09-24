#pragma once

#include "gl/attributes/force_inline.hpp"
#include "gl/util/enum.hpp"

#include <iostream>

namespace gl::io {

using index_type = int;
using iword_type = long;
using bit_position_type = unsigned;

inline constexpr iword_type iword_bit = 1ul;

namespace detail {

template <typename T>
concept c_bit_position_enum =
    std::is_enum_v<T> and std::convertible_to<std::underlying_type_t<T>, bit_position_type>;

} // namespace detail

class stream_option_manipulator {
public:
    stream_option_manipulator() = delete;

    explicit stream_option_manipulator(iword_type bitmask, bool operation = set)
    : _bitmask(bitmask), _operation(operation) {}

    [[nodiscard]] gl_attr_force_inline static stream_option_manipulator from_bit_position(
        bit_position_type bit_position, bool operation = set
    ) {
        return stream_option_manipulator{iword_bit << bit_position, operation};
    }

    friend std::ostream& operator<<(
        std::ostream& os, const stream_option_manipulator& property_manipulator
    ) {
        if (property_manipulator._operation == set)
            os.iword(_iostream_property_map_index()) |= property_manipulator._bitmask;
        else
            os.iword(_iostream_property_map_index()) &= ~property_manipulator._bitmask;

        return os;
    }

    friend std::istream& operator>>(
        std::istream& is, const stream_option_manipulator& property_manipulator
    ) {
        if (property_manipulator._operation == set)
            is.iword(_iostream_property_map_index()) |= property_manipulator._bitmask;
        else
            is.iword(_iostream_property_map_index()) &= ~property_manipulator._bitmask;

        return is;
    }

    [[nodiscard]] gl_attr_force_inline static bool is_option_set(
        std::ios_base& stream, bit_position_type bit_position
    ) {
        return (stream.iword(_iostream_property_map_index()) & (iword_bit << bit_position)) != 0;
    }

    [[nodiscard]] gl_attr_force_inline static bool are_options_set(
        std::ios_base& stream, iword_type bitmask
    ) {
        return (stream.iword(_iostream_property_map_index()) & bitmask) == bitmask;
    }

    static constexpr bool set = true;
    static constexpr bool unset = false;

private:
    [[nodiscard]] gl_attr_force_inline static index_type _iostream_property_map_index() {
        static index_type index = std::ios_base::xalloc();
        return index;
    }

    iword_type _bitmask;
    bool _operation;
};

namespace detail {

[[nodiscard]] inline iword_type get_options_bitmask(const std::initializer_list<bit_position_type>& bit_positions) {
    iword_type options_bitmask = 0ul;
    for (const auto bit_position : bit_positions)
        options_bitmask |= iword_bit << bit_position;
    return options_bitmask;
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] iword_type get_options_bitmask(const std::initializer_list<BitPosition>& bit_positions) {
    iword_type options_bitmask = 0ul;
    for (const auto bit_position : bit_positions)
        options_bitmask |= iword_bit << static_cast<bit_position_type>(util::to_underlying(bit_position));
    return options_bitmask;
}

} // namespace detail

[[nodiscard]] gl_attr_force_inline stream_option_manipulator set_options(iword_type bitmask) {
    return stream_option_manipulator{bitmask, stream_option_manipulator::set};
}

[[nodiscard]] gl_attr_force_inline stream_option_manipulator set_options(
    const std::initializer_list<bit_position_type>& bit_positions
) {
    return stream_option_manipulator{detail::get_options_bitmask(bit_positions), stream_option_manipulator::set};
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline stream_option_manipulator set_options(
    const std::initializer_list<BitPosition>& bit_positions
) {
    return stream_option_manipulator{detail::get_options_bitmask(bit_positions), stream_option_manipulator::set};
}

[[nodiscard]] gl_attr_force_inline stream_option_manipulator
set_option(bit_position_type bit_position) {
    return stream_option_manipulator::from_bit_position(
        bit_position, stream_option_manipulator::set
    );
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline stream_option_manipulator set_option(BitPosition bit_position) {
    return stream_option_manipulator::from_bit_position(
        static_cast<bit_position_type>(util::to_underlying(bit_position)),
        stream_option_manipulator::set
    );
}

[[nodiscard]] gl_attr_force_inline stream_option_manipulator unset_options(iword_type bitmask) {
    return stream_option_manipulator{bitmask, stream_option_manipulator::unset};
}

[[nodiscard]] gl_attr_force_inline stream_option_manipulator unset_options(
    const std::initializer_list<bit_position_type>& bit_positions
) {
    return stream_option_manipulator{detail::get_options_bitmask(bit_positions), stream_option_manipulator::unset};
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline stream_option_manipulator unset_options(
    const std::initializer_list<BitPosition>& bit_positions
) {
    return stream_option_manipulator{detail::get_options_bitmask(bit_positions), stream_option_manipulator::unset};
}

[[nodiscard]] gl_attr_force_inline stream_option_manipulator
unset_option(bit_position_type bit_position) {
    return stream_option_manipulator::from_bit_position(
        bit_position, stream_option_manipulator::unset
    );
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline stream_option_manipulator unset_option(BitPosition bit_position
) {
    return stream_option_manipulator::from_bit_position(
        static_cast<bit_position_type>(util::to_underlying(bit_position)),
        stream_option_manipulator::unset
    );
}

[[nodiscard]] gl_attr_force_inline bool are_option_set(
    std::ios_base& stream, iword_type bitmask
) {
    return stream_option_manipulator::are_options_set(stream, bitmask);
}

[[nodiscard]] gl_attr_force_inline bool are_option_set(
    std::ios_base& stream, const std::initializer_list<bit_position_type>& bit_positions
) {
    return stream_option_manipulator::are_options_set(stream, detail::get_options_bitmask(bit_positions));
}

template <detail::c_bit_position_enum BitPosition>
[[nodiscard]] gl_attr_force_inline bool are_option_set(
    std::ios_base& stream, const std::initializer_list<BitPosition>& bit_positions
) {
    return stream_option_manipulator::are_options_set(stream, detail::get_options_bitmask(bit_positions));
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
    return stream_option_manipulator::is_option_set(
        stream, static_cast<bit_position_type>(util::to_underlying(bit_position))
    );
}

} // namespace gl::io
