// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "hgl/repr/layout_tags.hpp"

#include <concepts>

namespace hgl::impl {

template <traits::c_hypergraph_asymmetric_layout_tag LT>
using major_element_t = typename LT::major_element;

template <traits::c_hypergraph_asymmetric_layout_tag LT>
using minor_element_t = typename LT::minor_element;

} // namespace hgl::impl
