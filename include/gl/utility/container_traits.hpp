#ifndef CPP_GL_UTILITY_CONTAINER_TRAITS
#define CPP_GL_UTILITY_CONTAINER_TRAITS

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>

#include <gl/utility/type_traits.hpp>



namespace gl {

// container types
struct vect_s     {};
struct deq_s      {};
struct list_s     {};
struct flist_s    {};
struct set_s      {}; // TODO: add Compare param
struct multiset_s {}; // TODO: add Compare param

namespace detail {

template <typename S> struct is_valid_container             : std::false_type {};
template <>           struct is_valid_container<vect_s>     : std::true_type  {};
template <>           struct is_valid_container<deq_s>      : std::true_type  {};
template <>           struct is_valid_container<list_s>     : std::true_type  {};
template <>           struct is_valid_container<flist_s>    : std::true_type  {};
template <>           struct is_valid_container<set_s>      : std::true_type  {};
template <>           struct is_valid_container<multiset_s> : std::true_type  {};

template <typename S>
inline constexpr bool is_valid_container_v = is_valid_container<S>::value;

} // namespace container

template <typename S>
concept graph_container_s = detail::is_valid_container_v<S>;



template <graph_container_s S, typename key_t = std::size_t>
struct container_traits {
    typedef void type;
    typedef void iterator;
    typedef void const_iterator;

    static inline void insert(S container, key_t&& key);
};

template <typename key_t>
struct container_traits <vect_s, key_t> {
    typedef std::vector<key_t> type;
    typedef std::vector<key_t>::iterator iterator;
    typedef std::vector<key_t>::const_iterator const_iterator;

    static inline void insert(std::vector<key_t>& container, key_t&& key) {
        container.push_back(std::move(key));
    }
};

template <typename key_t>
struct container_traits <deq_s, key_t> {
    typedef std::deque<key_t> type;
    typedef std::deque<key_t>::iterator iterator;
    typedef std::deque<key_t>::const_iterator const_iterator;

    static inline void insert(std::deque<key_t>& container, key_t&& key) {
        container.push_back(std::move(key));
    }
};

template <typename key_t>
struct container_traits <list_s, key_t> {
    typedef std::list<key_t> type;
    typedef std::list<key_t>::iterator iterator;
    typedef std::list<key_t>::const_iterator const_iterator;

    static inline void insert(std::list<key_t>& container, key_t&& key) {
        container.push_back(std::move(key));
    }
};

template <typename key_t>
struct container_traits <flist_s, key_t> {
    typedef std::forward_list<key_t> type;
    typedef std::forward_list<key_t>::iterator iterator;
    typedef std::forward_list<key_t>::const_iterator const_iterator;

    static inline void insert(std::forward_list<key_t>& container, key_t&& key) {
        container.insert_after(container.cend(), key);
    }
};

template <typename key_t>
struct container_traits <set_s, key_t> {
    typedef std::set<key_t> type;
    typedef std::set<key_t>::iterator iterator;
    typedef std::set<key_t>::const_iterator const_iterator;

    static inline void insert(std::set<key_t>& container, key_t&& key) {
        container.insert(std::move(key));
    }
};

template <typename key_t>
struct container_traits <multiset_s, key_t> {
    typedef std::multiset<key_t> type;
    typedef std::multiset<key_t>::iterator iterator;
    typedef std::multiset<key_t>::const_iterator const_iterator;

    static inline void insert(std::multiset<key_t>& container, key_t&& key) {
        container.insert(std::move(key));
    }
};

template <typename container_s, typename key_t>
using container_traits_t = typename container_traits<container_s, key_t>::type;



template <typename container_t>
concept joinable_container = requires(container_t c) {
    { std::ranges::begin(c) } -> std::same_as<typename container_t::iterator>;
    { std::ranges::end(c) }   -> std::same_as<typename container_t::iterator>;
};

template <typename container_t>
struct is_joinable_container {
    static constexpr bool value = joinable_container<container_t>;
};

template <typename container_t>
inline constexpr bool is_joinable_container_v = is_joinable_container<container_t>::value;

} // namespace gl

#endif // CPP_GL_UTILITY_CONTAINER_TRAITS
