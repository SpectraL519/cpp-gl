// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/util.hpp
/// @brief Defines utility functions and views for working with C++20 ranges in the HGL module.

#pragma once

#include "gl/util/ranges.hpp"

namespace hgl::util {

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::range_size
/// @see gl::util::range_size
using gl::util::range_size;

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::is_constant
/// @see gl::util::is_constant
using gl::util::is_constant;

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::all_equal
/// @see gl::util::all_equal
using gl::util::all_equal;

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::concat_view
/// @see gl::util::concat_view
using gl::util::concat_view;

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::concat_fn
/// @see gl::util::concat_fn
using gl::util::concat_fn;

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::concat
/// @see gl::util::concat
using gl::util::concat;

} // namespace hgl::util
