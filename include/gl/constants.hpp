// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/types/core.hpp"

#include <limits>

namespace gl {
namespace constants {

template <traits::c_id_type IdType>
inline constexpr IdType initial_id_v{0};

} // namespace constants

struct initial_id_t {
    template <traits::c_id_type IdType>
    [[nodiscard]] constexpr operator IdType() const noexcept {
        return constants::initial_id_v<IdType>;
    }
};

inline constexpr initial_id_t initial_id{};

// --- invalid id ---

template <traits::c_id_type IdType>
inline constexpr IdType invalid_id_v{std::numeric_limits<IdType>::max()};

struct invalid_id_t {
    template <traits::c_id_type IdType>
    [[nodiscard]] constexpr operator IdType() const noexcept {
        return invalid_id_v<IdType>;
    }

    template <traits::c_id_type IdType>
    [[nodiscard]] friend constexpr bool operator==(const IdType& lhs, invalid_id_t) noexcept {
        return lhs == invalid_id_v<IdType>;
    }
};

inline constexpr invalid_id_t invalid_id{};

} // namespace gl
