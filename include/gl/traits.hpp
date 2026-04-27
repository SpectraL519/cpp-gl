// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

/// @file gl/traits.hpp
/// @brief Contains C++20 concepts and type traits used to constrain library templates.

#pragma once

#include "gl/types/core.hpp"

#include <concepts>
#include <memory>
#include <ranges>
#include <type_traits>

namespace gl::traits {

/// @ingroup GL GL-Traits
/// @brief Type trait to check if a type is an instantiation of a specific class template.
/// @tparam T The type to check.
/// @tparam Template The class template to match against.
/// ### See Also
/// - [**c_instantiation_of**](gl_concepts.md#gl-traits-c-instantiation-of) concept
template <typename T, template <typename...> typename Template>
struct is_instantiation_of : std::false_type {};

/// @ingroup GL GL-Traits
/// @brief Specialization of the @ref gl::traits::is_instantiation_of "is_instantiation_of" trait
///        for when the type is an instantiation of the provided class template.
template <template <typename...> typename U, typename... Args>
struct is_instantiation_of<U<Args...>, U> : std::true_type {};

/// @ingroup GL GL-Traits
/// @brief Helper variable template for @ref gl::traits::is_instantiation_of "is_instantiation_of" trait.
/// @tparam T The type to check.
/// @tparam U The class template to match against.
template <typename T, template <typename...> typename U>
constexpr inline bool is_instantiation_of_v = is_instantiation_of<T, U>::value;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is an instantiation of a specific class template.
/// @tparam T The type to check.
/// @tparam Template The class template to match against.
template <typename T, template <typename...> typename Template>
concept c_instantiation_of = is_instantiation_of_v<T, Template>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is exactly one of the specified types.
/// @tparam T The type to check.
/// @tparam Types The parameter pack of valid types.
template <typename T, typename... Types>
concept c_one_of = std::disjunction_v<std::is_same<T, Types>...>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::range`.
/// @tparam R The type to check.
template <typename R>
concept c_range = std::ranges::range<R>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::forward_range`.
/// @tparam R The type to check.
template <typename R>
concept c_forward_range = std::ranges::forward_range<R>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::sized_range`.
/// @tparam R The type to check.
template <typename R>
concept c_sized_range = std::ranges::sized_range<R>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type satisfies `std::ranges::random_access_range`.
/// @tparam R The type to check.
template <typename R>
concept c_random_access_range = std::ranges::random_access_range<R>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a range containing a specific value type.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept strips cv-qualifiers from the range's value type before comparison.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_range_of =
    c_range<R> and std::same_as<T, std::remove_cv_t<std::ranges::range_value_t<R>>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a forward range containing a specific value type.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept strips cv-qualifiers from the range's value type before comparison.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_forward_range_of =
    c_forward_range<R> and std::same_as<T, std::remove_cv_t<std::ranges::range_value_t<R>>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a sized range containing a specific value type.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept strips cv-qualifiers from the range's value type before comparison.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_sized_range_of =
    c_sized_range<R> and std::same_as<T, std::remove_cv_t<std::ranges::range_value_t<R>>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a random access range containing a specific value type.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept strips cv-qualifiers from the range's value type before comparison.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_random_access_range_of =
    c_random_access_range<R> and std::same_as<T, std::remove_cv_t<std::ranges::range_value_t<R>>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a range containing a specific value type, strictly matching cv-qualifiers.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept preserves cv-qualifiers. The value type of the range must match `T` exactly.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_range_of_cv = c_range<R> and std::same_as<T, std::ranges::range_value_t<R>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a forward range containing a specific value type, strictly matching cv-qualifiers.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept preserves cv-qualifiers. The value type of the range must match `T` exactly.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_forward_range_of_cv =
    c_forward_range<R> and std::same_as<T, std::ranges::range_value_t<R>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a sized range containing a specific value type, strictly matching cv-qualifiers.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept preserves cv-qualifiers. The value type of the range must match `T` exactly.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_sized_range_of_cv = c_sized_range<R> and std::same_as<T, std::ranges::range_value_t<R>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is a random access range containing a specific value type, strictly matching cv-qualifiers.
///
/// > [!NOTE] CV qualifiers
/// >
/// > This concept preserves cv-qualifiers. The value type of the range must match `T` exactly.
///
/// @tparam R The type of the range.
/// @tparam T The expected value type.
template <typename R, typename T>
concept c_random_access_range_of_cv =
    c_random_access_range<R> and std::same_as<T, std::ranges::range_value_t<R>>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a range provides valid const iterators via `cbegin()` and `cend()`.
/// @tparam R The type of the range.
template <typename R>
concept c_const_range = requires(R& r) {
    std::ranges::cbegin(r);
    std::ranges::cend(r);
};

/// @ingroup GL GL-Traits
/// @brief Concept checking if dereferencing an iterator yields a const reference.
/// @tparam T The iterator type to check.
template <typename T>
concept c_const_iterator = requires(T iter) {
    { *iter } -> std::same_as<const std::remove_cvref_t<decltype(*iter)>&>;
};

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type supports three-way comparison and equality operators.
/// @tparam T The type to check.
template <typename T>
concept c_comparable = requires(const T lhs, const T rhs) {
    { lhs <=> rhs } -> std::convertible_to<std::partial_ordering>;
    { lhs == rhs } -> std::convertible_to<bool>;
};

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is an arithmetic type (integral or floating-point).
/// @tparam T The type to check.
template <typename T>
concept c_arithmetic = std::is_arithmetic_v<T>;

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type provides a valid `std::numeric_limits<T>::max()` value.
/// @tparam T The type to check.
template <typename T>
concept c_has_numeric_limits_max = requires {
    { std::numeric_limits<T>::max() } -> std::same_as<T>;
};

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type can be extracted from a `std::istream`.
/// @tparam T The type to check.
template <typename T>
concept c_readable = requires(T value, std::istream& is) { is >> value; };

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type can be inserted into a `std::ostream`.
/// @tparam T The type to check.
template <typename T>
concept c_writable = requires(T value, std::ostream& os) { os << value; };

/// @ingroup GL GL-Traits
/// @brief Concept checking if a type is an enumeration.
/// @tparam T The type to check.
template <typename T>
concept c_enum = std::is_enum_v<T>;

} // namespace gl::traits
