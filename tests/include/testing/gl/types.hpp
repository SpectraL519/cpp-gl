#pragma once

#include <cstddef>

namespace gl_testing::types {

using it_distance_type = std::ptrdiff_t;

struct visited_property {
    bool operator==(const visited_property&) const = default;
    bool visited;
};

struct used_property {
    bool operator==(const used_property&) const = default;
    bool used;
};

} // namespace gl_testing::types
