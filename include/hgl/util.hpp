// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/util.hpp
/// @brief Defines utility functions and views for working with C++20 ranges in the HGL module.

#pragma once

#include "gl/util/ranges.hpp"

namespace hgl {

/// @ingroup HGL-Util
/// @brief General utilities, ranges, and helpers for the HGL module (originating in the GL module).
///
/// This namespace pulls in practical, domain-agnostic C++ **range** utilities from `gl::util`.
/// Because hypergraphs share the same underlying memory models and algorithmic requirements as
/// standard graphs, they seamlessly reuse the same fundamental C++20 range utilities and helpers.
///
/// > [!NOTE]
/// >
/// > To get a detailed overview of these shared utilities, please refer to the GL module's @ref GL-Util documentation page.
namespace util {

using gl::util::all_equal;
using gl::util::is_constant;
using gl::util::range_size;

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::concat_view
/// ### See Also
/// - @ref gl::util::concat_view : For the full type definition.
template <std::ranges::view V1, std::ranges::view V2>
using concat_view = gl::util::concat_view<V1, V2>;

/// @ingroup HGL-Util
/// @brief @copybrief gl::util::concat_fn
/// ### See Also
/// - @ref gl::util::concat_fn : For the full type definition.
using concat_fn = gl::util::concat_fn;

/// @ingroup HGL-Util
/// @brief Concatenates two viewable ranges into a `concat_view`.
///
/// ### Example usage
/// ```cpp
/// std::vector<int> v1 = {1, 2, 3};
/// std::vector<int> v2 = {4, 5, 6};
/// auto concatenated = gl::util::concat(v1, v2);
/// for (int x : concatenated)
///     std::cout << x << " "; // Output: 1 2 3 4 5 6
/// ```
///
/// @param r1 First range to concatenate.
/// @param r2 Second range to concatenate.
/// @todo Replace with `std::views::concat` (C++26).
/// ### See Also
/// - @ref hgl::util::concat_view "concat_view": The view type that represents the concatenation of two ranges.
/// - @ref hgl::util::concat_fn "concat_fn": A helper compile-time constant function object for creating `concat_view` instances.
inline constexpr concat_fn concat{};

} // namespace util
} // namespace hgl
