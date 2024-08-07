#pragma once

// required for the type alias
namespace gl {

namespace types {} // namespace types

namespace type_traits {} // namespace type_traits

namespace impl {} // namespace impl

} // namespace gl

namespace gl_testing {

namespace lib = gl;
namespace lib_t = lib::types;
namespace lib_tt = lib::type_traits;
namespace lib_i = lib::impl;

} // namespace gl_testing
