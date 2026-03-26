// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include <concepts>
#include <cstdint>
#include <optional>
#include <utility>

namespace gl {

using size_type = std::size_t;
using default_id_type = std::uint32_t;

namespace traits {

template <typename T>
concept c_id_type = std::unsigned_integral<T>;

} // namespace traits

// TODO: use for all indexing
[[nodiscard]] constexpr size_type to_idx(const traits::c_id_type auto id) noexcept {
    return static_cast<size_type>(id);
}

template <typename T>
using homogeneous_pair = std::pair<T, T>;

} // namespace gl
