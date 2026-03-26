// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/traits.hpp"

namespace hgl::constants {

template <traits::c_id_type IdType>
inline constexpr IdType initial_id{0};

template <traits::c_id_type IdType>
inline constexpr IdType invalid_id{std::numeric_limits<IdType>::max()};

} // namespace hgl::constants
