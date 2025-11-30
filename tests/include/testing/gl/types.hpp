#pragma once

namespace gl_testing::types {

struct visited_property {
    bool operator==(const visited_property&) const = default;
    bool visited;
};

struct used_property {
    bool operator==(const used_property&) const = default;
    bool used;
};

} // namespace gl_testing::types
