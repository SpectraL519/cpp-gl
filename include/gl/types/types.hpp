#pragma once

#include <cstdint>
#include <optional>
#include <utility>

namespace gl::types {

using id_type = std::uint64_t;
using size_type = std::uint64_t;

template <typename T>
using homogeneous_pair = std::pair<T, T>;

template <typename T>
using optional_ref = std::optional<std::reference_wrapper<std::remove_reference_t<T>>>;

} // namespace gl::types
