#pragma once

#include "type_traits.hpp"
#include "types.hpp"

#include <format>
#include <iterator>

#ifdef GL_CONFIG_IT_RANGE_DEFAULT_CACHE_MODE_EAGER
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::cache_mode::eager
#elif defined(GL_CONFIG_IT_RANGE_DEFAULT_CACHE_MODE_LAZY)
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::cache_mode::lazy
#elif defined(GL_CONFIG_IT_RANGE_DEFAULT_CACHE_MODE_NONE)
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::cache_mode::none
#endif

#ifndef _GL_IT_RANGE_DEFAULT_CACHE_MODE
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::cache_mode::lazy
#endif

namespace gl {

namespace types {

template <
    std::forward_iterator Iterator,
    type_traits::cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
class iterator_range {
public:
    using iterator = Iterator;
#if __cplusplus >= 202302L
    using const_iterator = std::const_iterator<Iterator>;
#endif
    using distance_type = std::ptrdiff_t;
    using value_type = std::remove_reference_t<typename iterator::value_type>;

    static constexpr type_traits::cache_mode cache_mode = CacheMode;

    iterator_range() = delete;

    explicit iterator_range(iterator begin, iterator end)
    requires(cache_mode == type_traits::cache_mode::eager)
    {
        this->_distance = std::ranges::distance(begin, end);
        this->_range = std::make_pair(begin, end);
    }

    explicit iterator_range(iterator begin, iterator end)
    requires(cache_mode == type_traits::cache_mode::lazy)
    {
        this->_range = std::make_pair(begin, end);
        this->_distance = _invalid_distance;
    }

    explicit iterator_range(iterator begin, iterator end)
    requires(cache_mode == type_traits::cache_mode::none)
    {
        this->_range = std::make_pair(begin, end);
    }

    iterator_range(const iterator_range&) = default;
    iterator_range(iterator_range&&) = default;

    iterator_range& operator=(const iterator_range&) = default;
    iterator_range& operator=(iterator_range&&) = default;

    ~iterator_range() = default;

    bool operator==(const iterator_range&) const = default;

    [[nodiscard]] inline iterator begin() const {
        return this->_range.first;
    }

    [[nodiscard]] inline iterator end() const {
        return this->_range.second;
    }

#if __cplusplus >= 202302L
    [[nodiscard]] inline auto cbegin() const {
        return std::make_const_iterator(this->_range.first);
    }

    [[nodiscard]] inline auto cend() const {
        return std::make_const_iterator(this->_range.second);
    }
#endif

    [[nodiscard]] inline distance_type distance() const
    requires(cache_mode == type_traits::cache_mode::eager)
    {
        return this->_distance;
    }

    [[nodiscard]] inline distance_type distance() const
    requires(cache_mode == type_traits::cache_mode::lazy)
    {
        if (this->_is_distance_uninitialized())
            this->_distance = std::ranges::distance(this->begin(), this->end());
        return this->_distance;
    }

    [[nodiscard]] inline distance_type distance() const
    requires(cache_mode == type_traits::cache_mode::none)
    {
        return std::ranges::distance(this->begin(), this->end());
    }

    [[nodiscard]] inline value_type& element_at(types::size_type position) {
        this->_validate_element_position(position);
        return *std::ranges::next(this->begin(), position);
    }

    [[nodiscard]] inline const value_type& element_at(types::size_type position) const {
        this->_validate_element_position(position);
        return *std::ranges::next(this->begin(), position);
    }

private:
    [[nodiscard]] inline bool _is_distance_uninitialized() const
    requires(cache_mode == type_traits::cache_mode::lazy)
    {
        return this->_distance == _invalid_distance;
    }

    void _validate_element_position(const types::size_type position) const {
        const auto current_distance = this->distance();
        if (position >= current_distance)
            throw std::out_of_range(
                std::format("Position index {} out of range [0, {}]", position, current_distance)
            );
    }

    homogeneous_pair<iterator> _range;

    [[no_unique_address]] mutable std::
        conditional_t<cache_mode == type_traits::cache_mode::none, std::monostate, distance_type>
            _distance;

    static constexpr distance_type _invalid_distance = -1;
    static constexpr distance_type _default_n = 1;
};

} // namespace types

template <
    std::forward_iterator Iterator,
    type_traits::cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
[[nodiscard]] inline types::iterator_range<Iterator, CacheMode> make_iterator_range(
    Iterator begin, Iterator end
) {
    return types::iterator_range<Iterator, CacheMode>{begin, end};
}

template <
    type_traits::c_range Range,
    type_traits::cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
[[nodiscard]] inline types::iterator_range<typename Range::iterator, CacheMode> make_iterator_range(
    Range& range
) {
    return types::iterator_range<typename Range::iterator, CacheMode>{
        std::ranges::begin(range), std::ranges::end(range)
    };
}

template <
    type_traits::c_range Range,
    type_traits::cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
[[nodiscard]] inline types::iterator_range<typename Range::const_iterator, CacheMode>
make_const_iterator_range(const Range& range) {
    return types::iterator_range<typename Range::const_iterator, CacheMode>{
        std::ranges::cbegin(range), std::ranges::cend(range)
    };
}

} // namespace gl
