#pragma once

// required for the type alias
namespace gl {

namespace detail {}
namespace types {}

} // namespace gl

namespace gl_testing {
    namespace lib = gl;
    namespace lib_d = lib::detail;
    namespace lib_t = lib::types;
}
