#pragma once

namespace gl_testing {

struct visited_property {
    bool visited;
    bool operator==(const visited_property&) const = default;
};

struct used_property {
    bool used;
    bool operator==(const used_property&) const = default;
};

} // namespace gl_testing
