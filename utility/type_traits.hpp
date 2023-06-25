#ifndef CPP_GL_CONCEPTS
#define CPP_GL_CONCEPTS

#include <type_traits>
#include <concepts>
#include <optional>

#include <utility/types.hpp>



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



template <graph_container_s S, typename key_t = void, typename value_t = std::size_t>
struct container_traits {};

template <typename value_t> 
struct container_traits <container::vect_s, void, value_t> {
    typedef std::vector<value_t> type;
};

template <typename value_t> 
struct container_traits <container::list_s, void, value_t> {
    typedef std::list<value_t> type;
};

template <typename value_t> 
struct container_traits <container::set_s, void, value_t> {
    typedef std::set<value_t> type;
};

template <typename value_t> 
struct container_traits <container::hash_set_s, void, value_t> {
    typedef std::unordered_set<value_t> type;
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



// type alias trait
template <typename T, typename Alias>
concept has_type_alias = requires {
    typename T::template alias_t<Alias>;
};

} // namespace gl

#endif // CPP_GL_CONCEPTS