// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "types/types.hpp"

#include <limits>

namespace gl::constants {

inline constexpr types::id_type initial_id{std::numeric_limits<types::id_type>::min()};
inline constexpr types::id_type invalid_id{std::numeric_limits<types::id_type>::max()};

} // namespace gl::constants
