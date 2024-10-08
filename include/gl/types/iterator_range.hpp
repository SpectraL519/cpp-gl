// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "type_traits.hpp"
#include "types.hpp"

#include <format>
#include <iterator>

#if defined(GL_CONFIG_IT_RANGE_DEFAULT_CACHE_MODE_EAGER)
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::eager_cache
#elif defined(GL_CONFIG_IT_RANGE_DEFAULT_CACHE_MODE_LAZY)
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::lazy_cache
#elif defined(GL_CONFIG_IT_RANGE_DEFAULT_CACHE_MODE_NONE)
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::no_cache
#else
#define _GL_IT_RANGE_DEFAULT_CACHE_MODE gl::type_traits::lazy_cache
#endif

#ifdef GL_CONFIG_IT_RANGE_NOT_FINAL
#define _GL_IT_RANGE_NOT_FINAL
#else
#undef _GL_IT_RANGE_NOT_FINAL
#endif

namespace gl {

namespace types {

/*
A begin, end iterator holder class used as a view to the underlying collection.
Designed to be compatible with the range-based loops and std algorithms.
*/

template <
    std::forward_iterator Iterator,
    type_traits::c_cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
class iterator_range
#ifndef _GL_IT_RANGE_NOT_FINAL
    final
#endif
{
public:
    using iterator = Iterator;
#if __cplusplus >= 202302L
    using const_iterator = std::const_iterator<Iterator>;
#endif
    using distance_type = std::ptrdiff_t;
    using value_type = std::remove_reference_t<typename iterator::value_type>;

    using cache_mode = CacheMode;

    iterator_range() = delete;

    explicit iterator_range(iterator begin, iterator end)
    requires(cache_mode::value == type_traits::cache_mode_value::eager)
    {
        this->_distance = std::ranges::distance(begin, end);
        this->_range = std::make_pair(begin, end);
    }

    explicit iterator_range(iterator begin, iterator end)
    requires(cache_mode::value == type_traits::cache_mode_value::lazy)
    {
        this->_range = std::make_pair(begin, end);
        this->_distance = _invalid_distance;
    }

    explicit iterator_range(iterator begin, iterator end)
    requires(cache_mode::value == type_traits::cache_mode_value::none)
    {
        this->_range = std::make_pair(begin, end);
    }

    iterator_range(const iterator_range&) = default;
    iterator_range(iterator_range&&) = default;

    iterator_range& operator=(const iterator_range&) = default;
    iterator_range& operator=(iterator_range&&) = default;

#ifndef _GL_IT_RANGE_NOT_FINAL
    ~iterator_range() = default;
#else
    virtual ~iterator_range() = default;
#endif

    bool operator==(const iterator_range&) const = default;

    [[nodiscard]] gl_attr_force_inline iterator begin() const {
        return this->_range.first;
    }

    [[nodiscard]] gl_attr_force_inline iterator end() const {
        return this->_range.second;
    }

#if __cplusplus >= 202302L
    [[nodiscard]] gl_attr_force_inline auto cbegin() const {
        return std::make_const_iterator(this->_range.first);
    }

    [[nodiscard]] gl_attr_force_inline auto cend() const {
        return std::make_const_iterator(this->_range.second);
    }
#endif

    [[nodiscard]] gl_attr_force_inline distance_type distance() const
    requires(cache_mode::value == type_traits::cache_mode_value::eager)
    {
        return this->_distance;
    }

    [[nodiscard]] inline distance_type distance() const
    requires(cache_mode::value == type_traits::cache_mode_value::lazy)
    {
        if (this->_is_distance_uninitialized())
            this->_distance = std::ranges::distance(this->begin(), this->end());
        return this->_distance;
    }

    [[nodiscard]] gl_attr_force_inline distance_type distance() const
    requires(cache_mode::value == type_traits::cache_mode_value::none)
    {
        return std::ranges::distance(this->begin(), this->end());
    }

    [[nodiscard]] inline value_type& element_at(types::size_type position)
    requires(not type_traits::c_const_iterator<iterator>)
    {
        this->_validate_element_position(position);
        return *std::ranges::next(this->begin(), position);
    }

    [[nodiscard]] inline const value_type& element_at(types::size_type position) const {
        this->_validate_element_position(position);
        return *std::ranges::next(this->begin(), position);
    }

    [[nodiscard]] gl_attr_force_inline value_type& operator[](types::size_type position)
    requires(not type_traits::c_const_iterator<iterator>)
    {
        return this->element_at(position);
    }

    // clang-format off
    // gl_attr_force_inline misplacement

    [[nodiscard]] gl_attr_force_inline const value_type& operator[](types::size_type position) const {
        return this->element_at(position);
    }

    // clang-format on

private:
    [[nodiscard]] gl_attr_force_inline bool _is_distance_uninitialized() const
    requires(cache_mode::value == type_traits::cache_mode_value::lazy)
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

    [[no_unique_address]] mutable std::conditional_t<
        cache_mode::value == type_traits::cache_mode_value::none,
        std::monostate,
        distance_type> _distance;

    static constexpr distance_type _invalid_distance = -1;
    static constexpr distance_type _default_n = 1;
};

} // namespace types

template <
    std::forward_iterator Iterator,
    type_traits::c_cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
[[nodiscard]] gl_attr_force_inline types::iterator_range<Iterator, CacheMode> make_iterator_range(
    Iterator begin, Iterator end
) {
    return types::iterator_range<Iterator, CacheMode>{begin, end};
}

template <
    type_traits::c_range Range,
    type_traits::c_cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
[[nodiscard]] gl_attr_force_inline types::iterator_range<typename Range::iterator, CacheMode>
make_iterator_range(Range& range) {
    return types::iterator_range<typename Range::iterator, CacheMode>{
        std::ranges::begin(range), std::ranges::end(range)
    };
}

template <
    type_traits::c_range Range,
    type_traits::c_cache_mode CacheMode = _GL_IT_RANGE_DEFAULT_CACHE_MODE>
[[nodiscard]] gl_attr_force_inline types::iterator_range<typename Range::const_iterator, CacheMode>
make_const_iterator_range(const Range& range) {
    return types::iterator_range<typename Range::const_iterator, CacheMode>{
        std::ranges::cbegin(range), std::ranges::cend(range)
    };
}

} // namespace gl
