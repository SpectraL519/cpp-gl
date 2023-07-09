#ifndef CPP_GL_CONCEPTS
#define CPP_GL_CONCEPTS

#include <type_traits>
#include <concepts>
#include <optional>

#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <gl/utility/types.hpp>



namespace gl {

// container traits
namespace container {

template <typename S> struct is_valid               : std::false_type {};
template <>           struct is_valid <vect_s>      : std::true_type  {};
template <>           struct is_valid <list_s>      : std::true_type  {};
template <>           struct is_valid <set_s>       : std::true_type  {};
template <>           struct is_valid <hash_set_s>  : std::true_type  {};
template <>           struct is_valid <map_s>       : std::true_type  {};
template <>           struct is_valid <hash_map_s>  : std::true_type  {};

template <typename S>
inline constexpr bool is_valid_v = is_valid<S>::value;


template <typename S> struct is_adjacency_valid              : std::false_type {};
template <>           struct is_adjacency_valid <vect_s>     : std::true_type  {};
template <>           struct is_adjacency_valid <list_s>     : std::true_type  {};
template <>           struct is_adjacency_valid <set_s>      : std::true_type  {};
template <>           struct is_adjacency_valid <hash_set_s> : std::true_type  {};

template <typename S>
inline constexpr bool is_adjacency_valid_v = is_adjacency_valid<S>::value;

} // namespace container

template <typename S>
concept graph_container_s = container::is_valid_v<S>;

template <typename S>
concept adjacency_container_s = container::is_adjacency_valid_v<S>;



template <graph_container_s S, typename key_t = std::size_t, typename value_t = void>
struct container_traits {};

template <typename key_t> 
struct container_traits <container::vect_s, key_t, void> {
    typedef std::vector<key_t> type;
};

template <typename key_t> 
struct container_traits <container::list_s, key_t, void> {
    typedef std::list<key_t> type;
};

template <typename key_t> 
struct container_traits <container::set_s, key_t, void> {
    typedef std::set<key_t> type;
};

template <typename key_t> 
struct container_traits <container::hash_set_s, key_t, void> {
    typedef std::unordered_set<key_t> type;
};

template <typename key_t, typename value_t> 
struct container_traits <container::map_s, key_t, value_t> {
    typedef std::map<key_t, value_t> type;
};

template <typename key_t, typename value_t> 
struct container_traits <container::hash_map_s, key_t, value_t> {
    typedef std::unordered_map<key_t, value_t> type;
};



// data descriptor trait
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



namespace detail {

// has static const bool value
template <typename T>
concept has_static_const_bool_value = requires {
    T::value;
    std::is_same_v<decltype(T::value), const bool>;
};

} // namespace detail

// satisfies concept or is void trait
template <typename T, template <typename> typename Concept>
concept satisfies_or_void = detail::has_static_const_bool_value<Concept<T>> && (Concept<T>::value || std::is_void_v<T>);

template <typename T, template <typename> typename... Concepts>
concept satisfies_all = ((detail::has_static_const_bool_value<Concepts<T>> && Concepts<T>::value) && ...);

} // namespace gl

#endif // CPP_GL_CONCEPTS