#pragma once

#include "types.hpp"

#include <iterator>
#include <ranges>

namespace gl::types {

/*
TODO:
* constructor(container/range)
* assignment operators
* advance/retreat_begin/end
* comparison operators
*/

template <std::forward_iterator Iterator>
class iterator_range {
public:
    using iterator_type = Iterator;
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_reference_t<typename iterator_type::value_type>;

    iterator_range(iterator_type begin, iterator_type end) : _range{begin, end} {}

    template <std::ranges::range Range>
    iterator_range(Range& range) : _range{std::ranges::begin(range), std::ranges::end(range)} {}

    iterator_range(const iterator_range&) = default;
    iterator_range(iterator_range&&) = default;

    iterator_range& operator=(const iterator_range&) = default;
    iterator_range& operator=(iterator_range&&) = default;

    ~iterator_range() = default;

    bool operator==(const iterator_range&) const = default;

    [[nodiscard]] inline iterator_type begin() const {
        return this->_range.first;
    }

    [[nodiscard]] inline iterator_type end() const {
        return this->_range.second;
    }

    [[nodiscard]] inline difference_type distance() const {
        return std::ranges::distance(this->begin(), this->end());
    }

    [[nodiscard]] inline value_type& get(types::size_type n) const {
        return *std::next(this->begin(), n);
    }

private:
    homogeneous_pair<iterator_type> _range;
};

} // namespace gl::types
