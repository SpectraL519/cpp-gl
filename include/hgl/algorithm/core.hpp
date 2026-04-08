// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/algorithm/core.hpp"
#include "gl/algorithm/traits.hpp"
#include "gl/algorithm/util.hpp"
#include "hgl/traits.hpp"
#include "hgl/types.hpp"

namespace hgl::algorithm {

// -- core ---

using gl::algorithm::decision;
using gl::algorithm::result_discriminator;

using gl::algorithm::empty_callback;
using gl::algorithm::non_void_return_type;
using gl::algorithm::return_type;

using gl::algorithm::no_root;
using gl::algorithm::no_root_t;
using gl::algorithm::no_root_v;

// --- util ---

using gl::algorithm::is_reachable;

} // namespace hgl::algorithm

// TODO! Validate `const Callback&` vs `Callback&&` in alg templates
