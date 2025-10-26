#pragma once

// required for the type alias
namespace gl {

namespace impl {} // namespace impl

namespace types {} // namespace types

namespace type_traits {} // namespace type_traits

} // namespace gl

namespace gl_testing {

namespace lib = gl;
namespace lib_i = lib::impl;
namespace lib_t = lib::types;
namespace lib_tt = lib::type_traits;

} // namespace gl_testing
