#ifndef CPP_GL_CONCEPTS
#define CPP_GL_CONCEPTS

#include <concepts>
#include <type_traits>



namespace gl {

// numerical type concepts
template <typename T>
concept index_t = std::unsigned_integral<T>;

template <typename T>
concept numerical_t = std::is_arithmetic_v<T>;



// data descriptor concept
template <typename T>
struct is_data_descriptor {
private:
    template <typename S> // SFINAE
    static constexpr auto test(int) -> decltype(S::data, std::true_type{});

    template <typename>
    static constexpr std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template <typename T>
inline constexpr bool is_data_descriptor_v = is_data_descriptor<T>::value;



// alias concept
template <typename T, typename Alias>
concept has_type_alias = requires {
    typename T::template alias_t<Alias>;
};


} // namespace gl

#endif // CPP_GL_CONCEPTS