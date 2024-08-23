#pragma once

#include "type_traits.hpp"
#include "gl/attributes/force_inline.hpp"

#include <algorithm>

namespace gl {

namespace types {

template <std::forward_iterator Iterator>
requires(type_traits::c_strong_smart_ptr<typename Iterator::value_type>)
class non_null_iterator {
public:
    using iterator_type = Iterator;
    using value_type = typename std::iterator_traits<iterator_type>::value_type;
    using reference = typename std::iterator_traits<iterator_type>::reference;
    using pointer = typename std::iterator_traits<iterator_type>::pointer;
    using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
    using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;

    non_null_iterator() = default;

    non_null_iterator(iterator_type current, iterator_type end) : _current(current), _end(end) {
        if (this->_current != this->_end and not *this->_current)
            this->_skip_null_elements();
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
        this->_skip_null_elements();
        return *this;
    }

    inline non_null_iterator operator++(int) {
        non_null_iterator tmp = *this;
        this->_skip_null_elements();
        return tmp;
    }

    [[nodiscard]] gl_attr_force_inline bool operator==(const non_null_iterator& other) const {
        return this->_current == other._current;
    }

    [[nodiscard]] gl_attr_force_inline bool operator!=(const non_null_iterator& other) const {
        return this->_current != other._current;
    }

private:
    void _skip_null_elements() {
        constexpr auto is_null = [](const reference& ptr) { return ptr == nullptr; };

        if (this->_current == this->_end)
            return;

        this->_current = std::find_if_not(++this->_current, this->_end, is_null);
    }

    iterator_type _current;
    iterator_type _end;
};

} // namespace types

template <type_traits::c_range Range>
[[nodiscard]] gl_attr_force_inline auto non_null_begin(Range& range) {
    return types::non_null_iterator(std::ranges::begin(range), std::ranges::end(range));
}

template <type_traits::c_range Range>
[[nodiscard]] gl_attr_force_inline auto non_null_end(Range& range) {
    return types::non_null_iterator(std::ranges::end(range), std::ranges::end(range));
}

template <type_traits::c_const_range Range>
[[nodiscard]] gl_attr_force_inline auto non_null_cbegin(Range& range) {
    return types::non_null_iterator(std::ranges::cbegin(range), std::ranges::cend(range));
}

template <type_traits::c_const_range Range>
[[nodiscard]] gl_attr_force_inline auto non_null_cend(Range& range) {
    return types::non_null_iterator(std::ranges::cend(range), std::ranges::cend(range));
}

} // namespace gl
