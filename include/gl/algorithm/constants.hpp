#pragma once

#include <optional>

namespace gl::algorithm {

inline constexpr std::nullopt_t no_root_vertex = std::nullopt;

inline constexpr bool run_recursively = true;
inline constexpr bool run_iteratively = false;

} // namespace gl::algorithm
