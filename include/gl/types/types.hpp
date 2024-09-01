#pragma once

#include <cstdint>
#include <optional>
#include <utility>

namespace gl::types {

using size_type = std::uint64_t;
using id_type = size_type;

template <typename T>
using homogeneous_pair = std::pair<T, T>;

template <typename T>
using optional_ref = std::optional<std::reference_wrapper<std::remove_reference_t<T>>>;

} // namespace gl::types
