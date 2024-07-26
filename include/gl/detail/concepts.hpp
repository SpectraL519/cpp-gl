#pragma once

#include <concepts>
#include <type_traits>

namespace gl::detail {

template <typename T>
concept c_properties =
    std::default_initializable<T> and std::copy_constructible<T> and std::move_constructible<T>
    and std::assignable_from<T&, const T&>;

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

} // namespace gl::detail
