#pragma once

#include "doctest.h"

#include <gl/api.hpp>

#define GL_SUBCASE_STRICT_ONLY(name, traits) \
    SUBCASE(name)                            \
    if constexpr (std::same_as<typename traits::api_policy_tag, gl::api::strict_t>)
