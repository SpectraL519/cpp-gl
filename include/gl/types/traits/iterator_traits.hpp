#pragma once

#include "concepts.hpp"

#include <iterator>

namespace gl::type_traits {

template <type_traits::c_range Range>
using iterator_type = typename Range::iterator;

template <type_traits::c_const_range Range>
using const_iterator_type = typename Range::const_iterator;

// TODO: reverse iterator types (requires reverse_range concept)

} // namespace gl::type_traits
