// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/edge_tags.hpp"
#include "gl/types/type_traits.hpp"

namespace gl {

namespace impl {

struct list_t;

struct matrix_t;

} // namespace impl

namespace type_traits {

template <typename T>
concept c_graph_impl_tag = c_one_of<T, impl::list_t, impl::matrix_t>;

} // namespace type_traits

} // namespace gl
