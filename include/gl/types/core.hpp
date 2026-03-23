// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <cstdint>
#include <optional>
#include <utility>

namespace gl::types {

using size_type = std::uint64_t;
using id_type = size_type;

template <typename T>
using homogeneous_pair = std::pair<T, T>;

} // namespace gl::types
