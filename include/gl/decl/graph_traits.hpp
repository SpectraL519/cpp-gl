// Copyright (c) 2024-2026 Jakub Musiał
// This file is part of the CPP-GL project (https://github.com/SpectraL519/cpp-gl).
// Licensed under the MIT License. See the LICENSE file in the project root for full license information.

#pragma once

#include "gl/decl/impl_tags.hpp"
#include "gl/directional_tags.hpp"
#include "gl/traits.hpp"

namespace gl {

template <
    traits::c_graph_directional_tag DirectionalTag,
    traits::c_properties VertexProperties,
    traits::c_properties EdgeProperties,
    traits::c_graph_impl_tag ImplTag,
    traits::c_id_type IdType>
struct graph_traits;

} // namespace gl
