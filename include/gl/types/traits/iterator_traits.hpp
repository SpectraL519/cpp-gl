#pragma once

#include <iterator>

namespace gl::types::traits {

template <std::ranges::range Range>
using iterator_type = typename Range::iterator;

template <std::ranges::range Range>
using const_iterator_type = typename Range::const_iterator;

// TODO: reverse iterator types (requires reverse_range concept)

} // namespace gl::types::traits
