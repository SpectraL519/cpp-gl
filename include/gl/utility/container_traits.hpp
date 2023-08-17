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
struct vector             {};
struct deque              {};
struct linked_list        {};
struct doubly_linked_list {};
struct set                {}; // TODO: add Compare param
struct multiset           {}; // TODO: add Compare param

namespace detail {

template <typename T> struct is_valid_container                     : std::false_type {};
template <>           struct is_valid_container<vector>             : std::true_type  {};
template <>           struct is_valid_container<deque>              : std::true_type  {};
template <>           struct is_valid_container<linked_list>        : std::true_type  {};
template <>           struct is_valid_container<doubly_linked_list> : std::true_type  {};
template <>           struct is_valid_container<set>                : std::true_type  {};
template <>           struct is_valid_container<multiset>           : std::true_type  {};

template <typename T>
inline constexpr bool is_valid_container_v = is_valid_container<T>::value;

} // namespace container

template <typename T>
concept graph_container_t = detail::is_valid_container_v<T>;



template <graph_container_t C, typename key_t = std::size_t>
struct container_traits {
    typedef void type;
    typedef void iterator;
    typedef void const_iterator;

    static void insert(C& container, key_t&& key);
    static key_t& at(C& container, std::size_t index);
};

template <typename key_t>
struct container_traits <vector, key_t> {
    typedef std::vector<key_t> type;
    typedef std::vector<key_t>::iterator iterator;
    typedef std::vector<key_t>::const_iterator const_iterator;

    static inline void insert(std::vector<key_t>& container, key_t&& key) {
        container.push_back(std::move(key));
    }

    static inline key_t& at(std::vector<key_t>& container, std::size_t index) {
        return container.at(index);
    }
};

template <typename key_t>
struct container_traits <deque, key_t> {
    typedef std::deque<key_t> type;
    typedef std::deque<key_t>::iterator iterator;
    typedef std::deque<key_t>::const_iterator const_iterator;

    static inline void insert(std::deque<key_t>& container, key_t&& key) {
        container.push_back(std::move(key));
    }

    static inline key_t& at(std::deque<key_t>& container, std::size_t index) {
        return container.at(index);
    }
};

template <typename key_t>
struct container_traits <linked_list, key_t> {
    typedef std::forward_list<key_t> type;
    typedef std::forward_list<key_t>::iterator iterator;
    typedef std::forward_list<key_t>::const_iterator const_iterator;

    static inline void insert(std::forward_list<key_t>& container, key_t&& key) {
        container.insert_after(container.cend(), std::move(key));
    }

    static key_t& at(std::forward_list<key_t>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" + container.size() + "]"
            );

        return *std::next(container.begin(), index);
    }
};

template <typename key_t>
struct container_traits <doubly_linked_list, key_t> {
    typedef std::list<key_t> type;
    typedef std::list<key_t>::iterator iterator;
    typedef std::list<key_t>::const_iterator const_iterator;

    static inline void insert(std::list<key_t>& container, key_t&& key) {
        container.push_back(std::move(key));
    }

    static key_t& at(std::list<key_t>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" + container.size() + "]"
            );

        return *std::next(container.begin(), index);
    }
};

template <typename key_t>
struct container_traits <set, key_t> {
    typedef std::set<key_t> type;
    typedef std::set<key_t>::iterator iterator;
    typedef std::set<key_t>::const_iterator const_iterator;

    static inline void insert(std::set<key_t>& container, key_t&& key) {
        container.insert(std::move(key));
    }

    static key_t& at(std::set<key_t>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" + container.size() + "]"
            );

        return *std::next(container.begin(), index);
    }
};

template <typename key_t>
struct container_traits <multiset, key_t> {
    typedef std::multiset<key_t> type;
    typedef std::multiset<key_t>::iterator iterator;
    typedef std::multiset<key_t>::const_iterator const_iterator;

    static inline void insert(std::multiset<key_t>& container, key_t&& key) {
        container.insert(std::move(key));
    }

    static key_t& at(std::multiset<key_t>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" + container.size() + "]"
            );

        return *std::next(container.begin(), index);
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
