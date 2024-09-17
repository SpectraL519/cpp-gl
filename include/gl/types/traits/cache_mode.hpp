#pragma once

#include "concepts.hpp"

namespace gl::type_traits {

enum class cache_mode_value { none, lazy, eager };

template <cache_mode_value CacheModeValue>
struct cache_mode {
    static constexpr cache_mode_value value = CacheModeValue;
};

using no_cache = cache_mode<cache_mode_value::none>;
using lazy_cache = cache_mode<cache_mode_value::lazy>;
using eager_cache = cache_mode<cache_mode_value::eager>;

template <typename T>
concept c_cache_mode = c_one_of<T, no_cache, lazy_cache, eager_cache>;

} // namespace gl::type_traits
