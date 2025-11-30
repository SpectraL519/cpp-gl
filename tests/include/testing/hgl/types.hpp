#pragma once

namespace hgl_testing::types {

struct boolean_property {
    bool operator==(const boolean_property&) const = default;
    bool value;
};

} // namespace hgl_testing::types
