#pragma once

#include "gl/attributes/force_inline.hpp"

#include <iostream>

namespace gl::io {

using index_type = int;
using iword_type = long;
using bit_position_type = unsigned;

class stream_property_manipulator {
public:
    stream_property_manipulator() = delete;

    explicit stream_property_manipulator(bit_position_type bit_position, bool operation = set)
    : _bit_position(bit_position), _operation(operation) {}

    friend std::ostream& operator<<(
        std::ostream& os, const stream_property_manipulator& property_manipulator
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
        std::istream& is, const stream_property_manipulator& property_manipulator
    ) {
        if (property_manipulator._operation == set)
            is.iword(_iostream_property_map_index()) |=
                (_iword_bit << property_manipulator._bit_position);
        else
            is.iword(_iostream_property_map_index()) &=
                ~(_iword_bit << property_manipulator._bit_position);

        return is;
    }

    [[nodiscard]] inline static bool is_property_set(
        std::ios_base& stream, bit_position_type bit_position
    ) {
        return (stream.iword(_iostream_property_map_index()) & (_iword_bit << bit_position)) != 0;
    }

    static constexpr bool set = true;
    static constexpr bool unset = false;

private:
    [[nodiscard]] gl_attr_force_inline static index_type _iostream_property_map_index() {
        static index_type index = std::ios_base::xalloc();
        return index;
    }

    static constexpr iword_type _iword_bit = 1ul;

    bit_position_type _bit_position;
    bool _operation;
};

[[nodiscard]] gl_attr_force_inline stream_property_manipulator
set_property_at(bit_position_type bit_position) {
    return stream_property_manipulator{bit_position, stream_property_manipulator::set};
}

[[nodiscard]] gl_attr_force_inline stream_property_manipulator
unset_property_at(bit_position_type bit_position) {
    return stream_property_manipulator{bit_position, stream_property_manipulator::unset};
}

[[nodiscard]] gl_attr_force_inline bool has_property_at(
    std::ios_base& stream, bit_position_type bit_position
) {
    return stream_property_manipulator::is_property_set(stream, bit_position);
}

} // namespace gl::io
