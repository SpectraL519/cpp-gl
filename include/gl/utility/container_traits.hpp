#pragma once

#include "gl/utility/type_traits.hpp"
#include "gl/utility/types.hpp"

#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <vector>


namespace gl {

template <typename T>
concept graph_container_c =
    detail::is_valid_type_v<T, vector, deque, linked_list, doubly_linked_list, set, multiset>;


template <graph_container_c Container, typename Key = std::size_t>
struct container_traits {
    typedef void type;
    typedef void iterator;
    typedef void const_iterator;
    typedef void container_specifier;

    static void insert(Container& container, Key&& key);

    static void remove(Container& container, typename Container::iterator pos);
    template <typename UnaryPredicate>
    static void remove_if(Container& container, UnaryPredicate predicate);

    static const Key& at(Container& container, std::size_t index);
};


template <typename Key>
struct container_traits<vector, Key> {
    typedef std::vector<Key> type;
    typedef std::vector<Key>::iterator iterator;
    typedef std::vector<Key>::const_iterator const_iterator;
    typedef vector container_specifier;

    static inline void insert(std::vector<Key>& container, Key&& key) {
        container.push_back(std::move(key));
    }

    static inline void remove(std::vector<Key>& container, iterator pos) {
        container.erase(pos);
    }

    template <typename UnaryPredicate>
    static void remove_if(std::vector<Key>& container, UnaryPredicate predicate) {
        container.erase(
            std::remove_if(container.begin(), container.end(), predicate),
            container.end()
        );
    }

    static inline Key& at(std::vector<Key>& container, std::size_t index) {
        return container.at(index);
    }
};


template <typename Key>
struct container_traits<deque, Key> {
    typedef std::deque<Key> type;
    typedef std::deque<Key>::iterator iterator;
    typedef std::deque<Key>::const_iterator const_iterator;
    typedef deque container_specifier;

    static inline void insert(std::deque<Key>& container, Key&& key) {
        container.push_back(std::move(key));
    }

    static inline void remove(std::deque<Key>& container, iterator pos) {
        container.erase(pos);
    }

    template <typename UnaryPredicate>
    static void remove_if(std::deque<Key>& container, UnaryPredicate predicate) {
        container.erase(
            std::remove_if(container.begin(), container.end(), predicate),
            container.end()
        );
    }

    static inline Key& at(std::deque<Key>& container, std::size_t index) {
        return container.at(index);
    }
};


template <typename Key>
struct container_traits<linked_list, Key> {
    typedef std::forward_list<Key> type;
    typedef std::forward_list<Key>::iterator iterator;
    typedef std::forward_list<Key>::const_iterator const_iterator;
    typedef linked_list container_specifier;

    static inline void insert(std::forward_list<Key>& container, Key&& key) {
        container.insert_after(container.cend(), std::move(key));
    }

    static inline void remove(std::forward_list<Key>& container, iterator pos) {
        iterator prev = container.before_begin();
        while (prev != container.end() && std::next(prev) != pos)
            prev++;
        container.erase_after(prev);
    }

    template <typename UnaryPredicate>
    static void
        remove_if(std::forward_list<Key>& container, UnaryPredicate predicate) {
        container.remove_if(predicate);
    }

    static const Key& at(std::forward_list<Key>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" +
                std::to_string(container.size()) + "]"
            );

        return *std::next(container.begin(), index);
    }
};


template <typename Key>
struct container_traits<doubly_linked_list, Key> {
    typedef std::list<Key> type;
    typedef std::list<Key>::iterator iterator;
    typedef std::list<Key>::const_iterator const_iterator;
    typedef doubly_linked_list container_specifier;

    static inline void insert(std::list<Key>& container, Key&& key) {
        container.push_back(std::move(key));
    }

    static inline void remove(std::list<Key>& container, iterator pos) {
        container.erase(pos);
    }

    template <typename UnaryPredicate>
    static void remove_if(std::list<Key>& container, UnaryPredicate predicate) {
        container.erase(
            std::remove_if(container.begin(), container.end(), predicate),
            container.end()
        );
    }

    static const Key& at(std::list<Key>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" +
                std::to_string(container.size()) + "]"
            );

        return *std::next(container.begin(), index);
    }
};


template <typename Key>
struct container_traits<set, Key> {
    typedef std::set<Key> type;
    typedef std::set<Key>::iterator iterator;
    typedef std::set<Key>::const_iterator const_iterator;
    typedef set container_specifier;

    static inline void insert(std::set<Key>& container, Key&& key) {
        container.insert(std::move(key));
    }

    static inline void remove(std::set<Key>& container, iterator pos) {
        container.erase(pos);
    }

    template <typename UnaryPredicate>
    static void remove_if(std::set<Key>& container, UnaryPredicate predicate) {
        container.erase(
            std::remove_if(container.begin(), container.end(), predicate),
            container.end()
        );
    }

    static const Key& at(std::set<Key>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" +
                std::to_string(container.size()) + "]"
            );

        return *std::next(container.begin(), index);
    }
};


template <typename Key>
struct container_traits<multiset, Key> {
    typedef std::multiset<Key> type;
    typedef std::multiset<Key>::iterator iterator;
    typedef std::multiset<Key>::const_iterator const_iterator;
    typedef multiset container_specifier;

    static inline void insert(std::multiset<Key>& container, Key&& key) {
        container.insert(std::move(key));
    }

    static inline void remove(std::multiset<Key>& container, iterator pos) {
        container.erase(pos);
    }

    template <typename UnaryPredicate>
    static void remove_if(std::multiset<Key>& container, UnaryPredicate predicate) {
        container.erase(
            std::remove_if(container.begin(), container.end(), predicate),
            container.end()
        );
    }

    static const Key& at(std::multiset<Key>& container, std::size_t index) {
        if (index > container.size())
            throw std::out_of_range(
                "index [" + std::to_string(index) + "] >= contaiener.size() [" +
                std::to_string(container.size()) + "]"
            );

        return *std::next(container.begin(), index);
    }
};

template <typename container_t, typename Key>
using container_traits_t = typename container_traits<container_t, Key>::type;


template <typename Container>
concept joinable_container = requires(Container c) {
    { std::ranges::begin(c) } -> std::same_as<typename Container::iterator>;
    { std::ranges::end(c) } -> std::same_as<typename Container::iterator>;
};

template <typename Container>
struct is_joinable_container {
    static constexpr bool value = joinable_container<Container>;
};

template <typename Container>
inline constexpr bool is_joinable_container_v =
    is_joinable_container<Container>::value;

}  // namespace gl
