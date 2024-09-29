#pragma once

#include "gl/types/types.hpp"

#include <concepts>
#include <memory>
#include <ranges>
#include <type_traits>

namespace gl::type_traits {

template <typename T>
concept c_properties =
    std::semiregular<T> and std::move_constructible<T> and std::assignable_from<T&, const T&>;

template <typename, template <typename...> typename>
struct is_instantiation_of : std::false_type {};

template <template <typename...> typename U, typename... Args>
struct is_instantiation_of<U<Args...>, U> : std::true_type {};

template <typename T, template <typename...> typename U>
constexpr inline bool is_instantiation_of_v = is_instantiation_of<T, U>::value;

template <typename T, template <typename...> typename Template>
concept c_instantiation_of = is_instantiation_of_v<T, Template>;

template <typename T, typename... Types>
struct is_one_of : std::disjunction<std::is_same<T, Types>...> {};

template <typename T, typename... Types>
inline constexpr bool is_one_of_v = is_one_of<T, Types...>::value;

template <typename T, typename... Types>
concept c_one_of = is_one_of_v<T, Types...>;

template <typename R>
concept c_range = std::ranges::range<R>;

template <typename R>
concept c_sized_range = std::ranges::sized_range<R>;

template <typename R, typename T>
concept c_range_of =
    c_range<R> and std::same_as<T, std::remove_cv_t<std::ranges::range_value_t<R>>>;

template <typename R, typename T>
concept c_sized_range_of =
    c_sized_range<R> and std::same_as<T, std::remove_cv_t<std::ranges::range_value_t<R>>>;

// preserves cv qualifiers
template <typename R, typename T>
concept c_range_of_cv = c_range<R> and std::same_as<T, std::ranges::range_value_t<R>>;

// preserves cv qualifiers
template <typename R, typename T>
concept c_sized_range_of_cv = c_sized_range<R> and std::same_as<T, std::ranges::range_value_t<R>>;

template <typename R>
concept c_const_range = requires(R& r) {
    std::ranges::cbegin(r);
    std::ranges::cend(r);
};

template <typename T>
concept c_strong_smart_ptr =
    c_instantiation_of<T, std::unique_ptr> or c_instantiation_of<T, std::shared_ptr>;

template <typename T>
concept c_strong_ptr = c_strong_smart_ptr<T> or std::is_pointer_v<T>;

template <typename T>
concept c_const_iterator = requires(T iter) {
    { *iter } -> std::same_as<const std::remove_cvref_t<decltype(*iter)>&>;
};

template <typename T>
concept c_comparable = requires(const T lhs, const T rhs) {
    { lhs <=> rhs } -> std::convertible_to<std::partial_ordering>;
    { lhs == rhs } -> std::convertible_to<bool>;
};

template <typename T>
concept c_basic_arithmetic =
    std::semiregular<T> and c_comparable<T> and requires(const T a, const T b, T c) {
        { a + b } -> std::same_as<T>;
        { a - b } -> std::same_as<T>;
        { a* b } -> std::same_as<T>;
        { a / b } -> std::same_as<T>;
        { c += a } -> std::same_as<T&>;
        { c -= a } -> std::same_as<T&>;
        { c *= a } -> std::same_as<T&>;
        { c /= a } -> std::same_as<T&>;
    };

template <typename T>
concept c_readable = requires(T value, std::istream& is) { is >> value; };

template <typename T>
concept c_writable = requires(T value, std::ostream& os) { os << value; };

} // namespace gl::type_traits
