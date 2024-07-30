#pragma once

#include "namespaces.hpp"

#include <cstdint>
#include <utility>

namespace gl::types {

using id_type = std::uint64_t;
using size_type = std::uint64_t;

template <typename T>
using homogeneous_pair = std::pair<T, T>;

} // namespace gl::types
