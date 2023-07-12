#ifndef CPP_GL_CONCEPTS
#define CPP_GL_CONCEPTS

#include <type_traits>
#include <concepts>
#include <optional>

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>

#include <gl/utility/types.hpp>



namespace gl {

// container traits
namespace container {

template <typename S> struct is_valid              : std::false_type {};
template <>           struct is_valid <vect_s>     : std::true_type  {};
template <>           struct is_valid <deq_s>      : std::true_type  {};
template <>           struct is_valid <list_s>     : std::true_type  {};
template <>           struct is_valid <flist_s>    : std::true_type  {};
template <>           struct is_valid <set_s>      : std::true_type  {};
template <>           struct is_valid <multiset_s> : std::true_type  {};
// template <>           struct is_valid <hash_set_s> : std::true_type  {};

template <typename S>
inline constexpr bool is_valid_v = is_valid<S>::value;

} // namespace container

template <typename S>
concept graph_container_s = container::is_valid_v<S>;



template <graph_container_s S, typename key_t = std::size_t>
struct container_traits {
    typedef void type;
    static void insert (S container, const key_t& key) {}
    // TODO: remove method
};

template <typename key_t> 
struct container_traits <vect_s, key_t> {
    typedef std::vector<key_t> type;
    static void insert (std::vector<key_t>& container, const key_t& key) {
        container.emplace_back(key);
    }
};

template <typename key_t> 
struct container_traits <deq_s, key_t> {
    typedef std::deque<key_t> type;
    static void insert (std::deque<key_t>& container, const key_t& key) {
        container.emplace_back(key);
    }
};

template <typename key_t> 
struct container_traits <list_s, key_t> {
    typedef std::list<key_t> type;
    static void insert (std::list<key_t>& container, const key_t& key) {
        container.emplace_back(key);
    }
};

template <typename key_t> 
struct container_traits <flist_s, key_t> {
    typedef std::forward_list<key_t> type;
    static void insert (std::forward_list<key_t>& container, const key_t& key) {
        container.emplace_after(container.cend(), key);
    }
};

template <typename key_t> 
struct container_traits <set_s, key_t> {
    typedef std::set<key_t> type;
    static void insert (std::set<key_t>& container, const key_t& key) {
        container.emplace(key);
    }
};

template <typename key_t>
struct container_traits <multiset_s, key_t> {
    typedef std::multiset<key_t> type;
    static void insert (std::multiset<key_t>& container, const key_t& key) {
        container.emplace(key);
    }
};

// template <typename key_t> 
// struct container_traits <hash_set_s, key_t> {
//     typedef std::unordered_set<key_t> type;
//     static void insert (std::unordered_set<key_t>& container, const key_t& key) {
//         container.emplace(key);
//     }
// };

template <typename container_s, typename key_t>
using container_traits_t = typename container_traits<container_s, key_t>::type;



// data descriptor trait
template <typename T>
concept data_descriptor_t = requires (T descriptor) {
    typename T::data_type;
    { !std::is_void_v<typename T::data_type> };
    { descriptor.data() } -> std::same_as<typename T::data_type>;
};

template <typename T>
struct is_data_descriptor {
    using type = T;
    static constexpr bool value = data_descriptor_t<T>;
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