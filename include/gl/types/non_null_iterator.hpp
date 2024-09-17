#pragma once

#include "gl/attributes/force_inline.hpp"
#include "type_traits.hpp"

#include <algorithm>

namespace gl {

namespace types {

template <std::forward_iterator Iterator>
requires(type_traits::c_strong_ptr<typename Iterator::value_type>)
class non_null_iterator {
public:
    using iterator_type = Iterator;
    using value_type = typename std::iterator_traits<iterator_type>::value_type;
    using reference = typename std::iterator_traits<iterator_type>::reference;
    using pointer = typename std::iterator_traits<iterator_type>::pointer;
    using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
    using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;

    non_null_iterator() = default;

    non_null_iterator(iterator_type current, iterator_type end)
    requires(not std::bidirectional_iterator<iterator_type>)
    : _begin(), _current(current), _end(end) {
        if (this->_current != this->_end and not *this->_current)
            this->_skip_null_elements_forward();
    }

    non_null_iterator(iterator_type begin, iterator_type current, iterator_type end)
    : _begin(begin), _current(current), _end(end) {
        if (this->_current != this->_end and not *this->_current)
            this->_skip_null_elements_forward();
    }

    non_null_iterator(const non_null_iterator&) = default;
    non_null_iterator(non_null_iterator&&) = default;

    non_null_iterator& operator=(const non_null_iterator&) = default;
    non_null_iterator& operator=(non_null_iterator&&) = default;

    [[nodiscard]] gl_attr_force_inline reference operator*() const {
        return *this->_current;
    }

    [[nodiscard]] gl_attr_force_inline pointer operator->() const {
        return &(*this->_current);
    }

    inline non_null_iterator& operator++() {
        this->_skip_null_elements_forward();
        return *this;
    }

    inline non_null_iterator operator++(int) {
        non_null_iterator tmp = *this;
        this->_skip_null_elements_forward();
        return tmp;
    }

    inline non_null_iterator& operator--()
    requires(std::bidirectional_iterator<iterator_type>)
    {
        this->_skip_null_elements_backward();
        return *this;
    }

    inline non_null_iterator operator--(int)
    requires(std::bidirectional_iterator<iterator_type>)
    {
        non_null_iterator tmp = *this;
        this->_skip_null_elements_backward();
        return tmp;
    }

    [[nodiscard]] gl_attr_force_inline bool operator==(const non_null_iterator& other) const {
        return this->_current == other._current;
    }

    [[nodiscard]] gl_attr_force_inline bool operator!=(const non_null_iterator& other) const {
        return this->_current != other._current;
    }

private:
    static constexpr bool _is_null(const reference& ptr) {
        return ptr == nullptr;
    }

    inline void _skip_null_elements_forward() {
        if (this->_current == this->_end)
            return;

        this->_current = std::find_if_not(++this->_current, this->_end, _is_null);
    }

    void _skip_null_elements_backward()
    requires(std::bidirectional_iterator<iterator_type>)
    {
        if (this->_current == this->_begin)
            return;

        this->_current =
            std::find_if_not(
                ++std::make_reverse_iterator(this->_current),
                std::make_reverse_iterator(this->_begin),
                _is_null
            )
                .base();

        /*
        reverse_iterator.base() points to an element one past the position
        the iterator points to in a forward sequence so to make sure the new
        current iterator is valid it needs to be adjusted by one backward
        */
        if (this->_current != this->_begin)
            --this->_current;
    }

    [[no_unique_address]] std::
        conditional_t<std::bidirectional_iterator<iterator_type>, iterator_type, std::monostate>
            _begin;

    iterator_type _current;
    iterator_type _end;
};

} // namespace types

template <type_traits::c_range Range>
[[nodiscard]] auto non_null_begin(Range& range)
requires(not std::bidirectional_iterator<decltype(std::ranges::begin(range))>)
{
    return types::non_null_iterator(std::ranges::begin(range), std::ranges::end(range));
}

template <type_traits::c_range Range>
[[nodiscard]] auto non_null_end(Range& range)
requires(not std::bidirectional_iterator<decltype(std::ranges::end(range))>)
{
    return types::non_null_iterator(std::ranges::end(range), std::ranges::end(range));
}

template <type_traits::c_const_range Range>
[[nodiscard]] auto non_null_cbegin(Range& range)
requires(not std::bidirectional_iterator<decltype(std::ranges::cbegin(range))>)
{
    return types::non_null_iterator(std::ranges::cbegin(range), std::ranges::cend(range));
}

template <type_traits::c_const_range Range>
[[nodiscard]] auto non_null_cend(Range& range)
requires(not std::bidirectional_iterator<decltype(std::ranges::cend(range))>)
{
    return types::non_null_iterator(std::ranges::cend(range), std::ranges::cend(range));
}

template <type_traits::c_range Range>
[[nodiscard]] auto non_null_begin(Range& range)
requires(std::bidirectional_iterator<decltype(std::ranges::begin(range))>)
{
    return types::non_null_iterator(
        std::ranges::begin(range), std::ranges::begin(range), std::ranges::end(range)
    );
}

template <type_traits::c_range Range>
[[nodiscard]] auto non_null_end(Range& range)
requires(std::bidirectional_iterator<decltype(std::ranges::end(range))>)
{
    return types::non_null_iterator(
        std::ranges::begin(range), std::ranges::end(range), std::ranges::end(range)
    );
}

template <type_traits::c_const_range Range>
[[nodiscard]] auto non_null_cbegin(Range& range)
requires(std::bidirectional_iterator<decltype(std::ranges::cbegin(range))>)
{
    return types::non_null_iterator(
        std::ranges::cbegin(range), std::ranges::cbegin(range), std::ranges::cend(range)
    );
}

template <type_traits::c_const_range Range>
[[nodiscard]] auto non_null_cend(Range& range)
requires(std::bidirectional_iterator<decltype(std::ranges::cend(range))>)
{
    return types::non_null_iterator(
        std::ranges::cbegin(range), std::ranges::cend(range), std::ranges::cend(range)
    );
}

} // namespace gl
