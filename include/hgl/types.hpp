// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/types/core.hpp"
#include "gl/types/flat_jagged_vector.hpp"
#include "gl/types/properties.hpp"

namespace hgl {

// --- core types ---

using gl::id_type;
using gl::size_type;

// --- generic data structures ---

using gl::flat_jagged_vector;
using gl::homogeneous_pair;

// --- property types ---

using gl::bin_color_value;
using gl::binary_color;
using gl::binary_color_property;
using gl::dynamic_properties;
using gl::empty_properties;
using gl::empty_properties_map;
using gl::name_property;
using gl::weight_property;

} // namespace hgl
