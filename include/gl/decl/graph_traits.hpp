#pragma once

#include "gl/types/type_traits.hpp"
#include "impl_tags.hpp"

namespace gl {

template <
    type_traits::c_edge_directional_tag EdgeDirectionalTag,
    type_traits::c_properties VertexProperties,
    type_traits::c_properties EdgeProperties,
    type_traits::c_graph_impl_tag ImplTag,
    type_traits::c_cache_mode VDegCacheMode>
struct graph_traits;

} // namespace gl
