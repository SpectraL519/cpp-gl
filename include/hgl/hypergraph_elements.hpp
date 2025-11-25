// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "constants.hpp"
#include "gl/vertex_descriptor.hpp"
#include "types/type_traits.hpp"
#include "types/types.hpp"

namespace hgl {

template <type_traits::c_properties Properties = types::empty_properties>
using vertex_descriptor = gl::vertex_descriptor<Properties>;

} // namespace hgl
