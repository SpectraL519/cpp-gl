#ifndef CPP_GL_CONCEPTS
#define CPP_GL_CONCEPTS

#include <concepts>
#include <type_traits>

#include <utility/types.hpp>



namespace gl {

// data descriptor concept
template <typename T>
struct data_struct {
private:
    template <typename S>
    static constexpr auto test(int) -> decltype(S::data, std::true_type{});

    template <typename>
    static constexpr std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template <typename T>
concept is_data_struct = data_struct<T>::value;



// alias concept
template <typename T, typename Alias>
concept has_type_alias = requires {
    typename T::template alias_t<Alias>;
};


} // namespace gl

#endif // CPP_GL_CONCEPTS