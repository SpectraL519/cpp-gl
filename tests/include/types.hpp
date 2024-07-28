#pragma once

namespace gl_testing {

namespace vertex_types {

struct visited_property {
    bool operator==(const visited_property&) const = default;
    bool visited;
};

} // namespace vertex_types

namespace edge_types {

struct used_property {
    bool operator==(const used_property&) const = default;
    bool used;
};

} // namespace edge_types

} // namespace gl_testing
