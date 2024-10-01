// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/attributes/force_inline.hpp"
#include "type_traits.hpp"

#include <algorithm>

namespace gl {

namespace types {

template <std::forward_iterator Iterator>
requires(type_traits::c_strong_ptr<typename Iterator::value_type>)
class dereferencing_iterator {
private:
    using iterator_value_type = typename std::iterator_traits<Iterator>::value_type;

public:
    using iterator_type = Iterator;
    using value_type = type_traits::ptr_element_type_t<iterator_value_type>;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
    using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;

    dereferencing_iterator() = default;

    dereferencing_iterator(iterator_type it) : _it(it) {}

    dereferencing_iterator(const dereferencing_iterator&) = default;
    dereferencing_iterator(dereferencing_iterator&&) = default;

    dereferencing_iterator& operator=(const dereferencing_iterator&) = default;
    dereferencing_iterator& operator=(dereferencing_iterator&&) = default;

    [[nodiscard]] gl_attr_force_inline reference operator*() const {
        return **this->_it;
    }

    [[nodiscard]] gl_attr_force_inline pointer operator->() const {
        if constexpr (type_traits::c_strong_smart_ptr<iterator_value_type>)
            return this->_it->get();
        else
            return *this->_it;
    }

    inline dereferencing_iterator& operator++() {
        this->_it++;
        return *this;
    }

    inline dereferencing_iterator operator++(int) {
        dereferencing_iterator tmp = *this;
        this->_it++;
        return tmp;
    }

    inline dereferencing_iterator& operator--()
    requires(std::bidirectional_iterator<iterator_type>)
    {
        this->_it--;
        return *this;
    }

    inline dereferencing_iterator operator--(int)
    requires(std::bidirectional_iterator<iterator_type>)
    {
        dereferencing_iterator tmp = *this;
        this->_it--;
        return tmp;
    }

    [[nodiscard]] gl_attr_force_inline bool operator==(const dereferencing_iterator& other) const {
        return this->_it == other._it;
    }

    [[nodiscard]] gl_attr_force_inline bool operator!=(const dereferencing_iterator& other) const {
        return this->_it != other._it;
    }

    [[nodiscard]] gl_attr_force_inline iterator_type base() const {
        return this->_it;
    }

private:
    iterator_type _it;
};

} // namespace types

template <type_traits::c_range Range>
[[nodiscard]] gl_attr_force_inline auto deref_begin(Range& range) {
    return types::dereferencing_iterator(std::ranges::begin(range));
}

template <type_traits::c_range Range>
[[nodiscard]] gl_attr_force_inline auto deref_end(Range& range) {
    return types::dereferencing_iterator(std::ranges::end(range));
}

template <type_traits::c_const_range Range>
[[nodiscard]] gl_attr_force_inline auto deref_cbegin(Range& range) {
    return types::dereferencing_iterator(std::ranges::cbegin(range));
}

template <type_traits::c_const_range Range>
[[nodiscard]] gl_attr_force_inline auto deref_cend(Range& range) {
    return types::dereferencing_iterator(std::ranges::cend(range));
}

} // namespace gl
