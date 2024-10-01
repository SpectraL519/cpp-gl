// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <cstdint>
#include <optional>
#include <utility>

namespace gl::types {

// --- general types ---

template <typename T>
using homogeneous_pair = std::pair<T, T>;

template <typename T>
using optional_ref = std::optional<std::reference_wrapper<std::remove_reference_t<T>>>;

template <typename T>
using optional_cref = std::optional<std::reference_wrapper<const std::remove_cvref_t<T>>>;

template <typename T>
using const_ref_wrap = std::reference_wrapper<const T>;

// --- graph specific types ---

using size_type = std::uint64_t;
using id_type = size_type;

} // namespace gl::types
