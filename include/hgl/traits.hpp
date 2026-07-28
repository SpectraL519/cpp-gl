// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file hgl/traits.hpp
/// @brief Contains C++20 concepts and type traits used to constrain hypergraph library templates.

#pragma once

#include "gl/traits.hpp"

namespace hgl {

/// @ingroup HGL-Traits
/// @brief Traits and concepts for the HGL module.
///
/// This namespace contains the definitions of all general and hypergraph-specifc concepts and type
/// traits used to constrain library types and pulls in all general traits, concept checkers,
/// and metaprogramming utilities from `gl::traits`. Because hypergraphs share the same underlying
/// implementation design and mechanisms as standard graphs, they seamlessly reuse the same
/// fundamental C++20 concepts.
///
/// > [!NOTE]
/// >
/// > To get a detailed overview of these shared utilities, please refer to the GL module's @ref GL-Traits documentation page.
namespace traits {

using gl::traits::c_instantiation_of;
using gl::traits::is_instantiation_of;
using gl::traits::is_instantiation_of_v;

using gl::traits::c_one_of;

using gl::traits::c_const_iterator;
using gl::traits::c_const_range;
using gl::traits::c_forward_range;
using gl::traits::c_forward_range_of;
using gl::traits::c_forward_range_of_cv;
using gl::traits::c_random_access_range;
using gl::traits::c_random_access_range_of;
using gl::traits::c_random_access_range_of_cv;
using gl::traits::c_range;
using gl::traits::c_range_of;
using gl::traits::c_range_of_cv;
using gl::traits::c_sized_range;
using gl::traits::c_sized_range_of;
using gl::traits::c_sized_range_of_cv;

using gl::traits::c_arithmetic;
using gl::traits::c_comparable;
using gl::traits::c_enum;
using gl::traits::c_has_numeric_limits_max;
using gl::traits::c_readable;
using gl::traits::c_writable;

} // namespace traits
} // namespace hgl
