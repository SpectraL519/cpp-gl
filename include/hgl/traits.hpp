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

/// @ingroup HGL-Traits
/// @brief Type trait to check if a type is an instantiation of a specific class template.
/// ### See Also
/// - @ref gl::traits::is_instantiation_of "gl::traits::is_instantiation_of" : For the full trait documentation in the GL module.
using gl::traits::is_instantiation_of;

/// @ingroup HGL-Traits
/// @brief Helper variable template for the `is_instantiation_of` trait.
/// ### See Also
/// - @ref gl::traits::is_instantiation_of_v "gl::traits::is_instantiation_of_v" : For the full trait documentation in the GL module.
using gl::traits::is_instantiation_of_v;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is an instantiation of a specific class template.
/// ### See Also
/// - [**c_instantiation_of**](gl_concepts.md#gl-traits-c-instantiation-of) : For the full concept documentation in the GL module.
using gl::traits::c_instantiation_of;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is exactly one of the specified types.
/// ### See Also
/// - [**c_one_of**](gl_concepts.md#gl-traits-c-one-of) : For the full concept documentation in the GL module.
using gl::traits::c_one_of;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::range`.
/// ### See Also
/// - [**c_range**](gl_concepts.md#gl-traits-c-range) : For the full concept documentation in the GL module.
using gl::traits::c_range;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a range containing a specific value type.
/// ### See Also
/// - [**c_range_of**](gl_concepts.md#gl-traits-c-range-of) : For the full concept documentation in the GL module.
using gl::traits::c_range_of;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a range containing a specific value type, strictly matching cv-qualifiers.
/// ### See Also
/// - [**c_range_of_cv**](gl_concepts.md#gl-traits-c-range-of-cv) : For the full concept documentation in the GL module.
using gl::traits::c_range_of_cv;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::forward_range`.
/// ### See Also
/// - [**c_forward_range**](gl_concepts.md#gl-traits-c-forward-range) : For the full concept documentation in the GL module.
using gl::traits::c_forward_range;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a forward range containing a specific value type.
/// ### See Also
/// - [**c_forward_range_of**](gl_concepts.md#gl-traits-c-forward-range-of) : For the full concept documentation in the GL module.
using gl::traits::c_forward_range_of;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a forward range containing a specific value type, strictly matching cv-qualifiers.
/// ### See Also
/// - [**c_forward_range_of_cv**](gl_concepts.md#gl-traits-c-forward-range-of-cv) : For the full concept documentation in the GL module.
using gl::traits::c_forward_range_of_cv;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::sized_range`.
/// ### See Also
/// - [**c_sized_range**](gl_concepts.md#gl-traits-c-sized-range) : For the full concept documentation in the GL module.
using gl::traits::c_sized_range;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a sized range containing a specific value type.
/// ### See Also
/// - [**c_sized_range_of**](gl_concepts.md#gl-traits-c-sized-range-of) : For the full concept documentation in the GL module.
using gl::traits::c_sized_range_of;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a sized range containing a specific value type, strictly matching cv-qualifiers.
/// ### See Also
/// - [**c_sized_range_of_cv**](gl_concepts.md#gl-traits-c-sized-range-of-cv) : For the full concept documentation in the GL module.
using gl::traits::c_sized_range_of_cv;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::random_access_range`.
/// ### See Also
/// - [**c_random_access_range**](gl_concepts.md#gl-traits-c-random-access-range) : For the full concept documentation in the GL module.
using gl::traits::c_random_access_range;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a random access range containing a specific value type.
/// ### See Also
/// - [**c_random_access_range_of**](gl_concepts.md#gl-traits-c-random-access-range-of) : For the full concept documentation in the GL module.
using gl::traits::c_random_access_range_of;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is a random access range containing a specific value type, strictly matching cv-qualifiers.
/// ### See Also
/// - [**c_random_access_range_of_cv**](gl_concepts.md#gl-traits-c-random-access-range-of-cv) : For the full concept documentation in the GL module.
using gl::traits::c_random_access_range_of_cv;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type supports three-way comparison and equality operators.
/// ### See Also
/// - [**c_comparable**](gl_concepts.md#gl-traits-c-comparable) : For the full concept documentation in the GL module.
using gl::traits::c_comparable;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is an arithmetic type (integral or floating-point).
/// ### See Also
/// - [**c_arithmetic**](gl_concepts.md#gl-traits-c-arithmetic) : For the full concept documentation in the GL module.
using gl::traits::c_arithmetic;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type provides a valid `std::numeric_limits<T>::max()` value.
/// ### See Also
/// - [**c_has_numeric_limits_max**](gl_concepts.md#gl-traits-c-has-numeric-limits-max) : For the full concept documentation in the GL module.
using gl::traits::c_has_numeric_limits_max;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type can be extracted from a `std::istream`.
/// ### See Also
/// - [**c_readable**](gl_concepts.md#gl-traits-c-readable) : For the full concept documentation in the GL module.
using gl::traits::c_readable;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type can be inserted into a `std::ostream`.
/// ### See Also
/// - [**c_writable**](gl_concepts.md#gl-traits-c-writable) : For the full concept documentation in the GL module.
using gl::traits::c_writable;

/// @ingroup HGL-Traits
/// @brief Concept checking if a type is an enumeration.
/// ### See Also
/// - [**c_enum**](gl_concepts.md#gl-traits-c-enum) : For the full concept documentation in the GL module.
using gl::traits::c_enum;

} // namespace traits
} // namespace hgl
