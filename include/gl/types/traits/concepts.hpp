#pragma once

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
concept c_const_range = requires(R& r) {
    std::ranges::cbegin(r);
    std::ranges::cend(r);
};

template <typename R>
concept c_reverse_range = requires(R& r) {
    std::ranges::rbegin(r);
    std::ranges::rend(r);
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

} // namespace gl::type_traits
