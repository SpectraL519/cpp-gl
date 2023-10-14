#pragma once

#include <concepts>
#include <type_traits>


namespace gl {

// general traits
namespace detail {

template <typename T>
concept valid_requirement = requires {
    T::value;
    std::is_same_v<decltype(T::value), const bool>;
};

template <typename T, template <typename> typename Requirement>
concept satisfies_or_void = valid_requirement<Requirement<T>> &&
                            (Requirement<T>::value || std::is_void_v<T>);

template <typename T, template <typename> typename... Requirements>
concept satisfies_all =
    ((valid_requirement<Requirements<T>> && Requirements<T>::value) && ...);

template <typename T, template <typename> typename... Requirements>
concept satisfies_any =
    ((valid_requirement<Requirements<T>> && Requirements<T>::value) || ...);


template <typename Base, typename Derived>
concept derived_from = std::is_base_of_v<Base, Derived>;


template <typename T>
concept equality_comparable = std::default_initializable<T> && requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
};

template <typename T>
struct is_equality_comparable {
    static constexpr bool value = equality_comparable<T>;
};

template <typename T>
inline constexpr bool is_equality_comparable_v = is_equality_comparable<T>::value;


template <typename T, typename... ValidTypes>
struct is_valid_type : std::disjunction<std::is_same<T, ValidTypes>...> {};

// template <typename T>
// inline constexpr bool is_valid_type_v = false;

template <typename T, typename... ValidTypes>
inline constexpr bool is_valid_type_v = is_valid_type<T, ValidTypes...>::value;


// numerical type concepts
template <typename T>
concept u_integral = std::unsigned_integral<T>;

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

}  // namespace detail


// data descriptor trait
template <typename Descriptor>
concept data_descriptor_t = requires(Descriptor descriptor) {
    typename Descriptor::data_type;
    { not std::is_void_v<typename Descriptor::data_type> };
    {
        std::is_same_v<decltype(descriptor.data()), typename Descriptor::data_type&>
    };
    {
        std::is_same_v<decltype(descriptor.set_data(std::declval<typename Descriptor::data_type&>())), void>
    };
};

template <typename T>
struct is_data_descriptor {
    using type = T;
    static constexpr bool value = data_descriptor_t<T>;
};

template <typename T>
inline constexpr bool is_data_descriptor_v = is_data_descriptor<T>::value;

}  // namespace gl
