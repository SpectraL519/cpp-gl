#ifndef CPP_GL_UTILITY_TYPE_TRAITS
#define CPP_GL_UTILITY_TYPE_TRAITS

#include <type_traits>

#include <gl/utility/type_traits.hpp>



namespace gl {

// data descriptor trait
template<typename descriptor_t>
concept data_descriptor_t = requires(descriptor_t descriptor) {
    typename descriptor_t::data_type;
    { !std::is_void_v<typename descriptor_t::data_type> };
    { std::is_same_v<decltype(descriptor.data()), typename descriptor_t::data_type&> };
    { std::is_same_v<decltype(descriptor.set_data(std::declval<typename descriptor_t::data_type&>())), void> };
};

template<typename T>
struct is_data_descriptor {
    using type = T;
    static constexpr bool value = data_descriptor_t<T>;
};

template<typename T>
inline constexpr bool is_data_descriptor_v = is_data_descriptor<T>::value;



// general traits
namespace detail {

// has static const bool value
template<typename T>
concept has_static_const_bool_value = requires {
    T::value;
    std::is_same_v<decltype(T::value), const bool>;
};

template<typename T, template<typename> typename Concept>
concept satisfies_or_void = has_static_const_bool_value<Concept<T>> && (Concept<T>::value || std::is_void_v<T>);

template<typename T, template<typename> typename... Concepts>
concept satisfies_all = ((has_static_const_bool_value<Concepts<T>> && Concepts<T>::value) && ...);

template<typename T, template<typename> typename... Concepts>
concept satisfies_any = ((has_static_const_bool_value<Concepts<T>> && Concepts<T>::value) || ...);

template<typename Base, typename Derived>
concept derived_from = std::is_base_of_v<Base, Derived>;

template<typename data_t>
concept equality_comparable =
    std::default_initializable<data_t> &&
    requires(data_t a, data_t b) {
        { a == b } -> std::convertible_to<bool>;
    };

template<typename data_t>
struct equality_comparable_s {
    static constexpr bool value = equality_comparable<data_t>;
};

} // namespace detail

// numerical type concepts
template<typename T>
concept index_t = std::unsigned_integral<T>;

template<typename T>
concept arithmetic_t = std::is_arithmetic_v<T>;

} // namespace gl

#endif // CPP_GL_UTILITY_TYPE_TRAITS
