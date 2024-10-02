// Copyright (c) 2024 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/types/type_traits.hpp"
#include "impl_tags.hpp"

namespace gl {

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag,
    type_traits::c_properties VertexProperties,
    type_traits::c_properties EdgeProperties,
    type_traits::c_graph_impl_tag ImplTag>
struct graph_traits;

} // namespace gl
