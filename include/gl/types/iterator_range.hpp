#pragma once

#include "types.hpp"

#include <iterator>

namespace gl::types {

template <std::forward_iterator Iterator>
class iterator_range {
public:
    using iterator_type = Iterator;

    // TODO: figure out the types (use reference or not ?)

    iterator_range(iterator_type begin, iterator_type end) : _range(std::make_pair(begin, end)) {}

    [[nodiscard]] inline iterator_type begin() const {
        return this->_range.first;
    }

    [[nodiscard]] inline iterator_type end() const {
        return this->_range.second;
    }

private:
    homogeneous_pair<iterator_type> _range;
};

} // namespace gl::types
