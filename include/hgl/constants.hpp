// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/constants.hpp"
#include "hgl/traits.hpp"

namespace hgl {

namespace constants {

template <traits::c_id_type IdType>
inline constexpr IdType invalid_id{std::numeric_limits<IdType>::max()};

} // namespace constants

using gl::initial_id;
using gl::initial_id_t;
using gl::initial_id_v;

using gl::invalid_id;
using gl::invalid_id_t;
using gl::invalid_id_v;

} // namespace hgl
