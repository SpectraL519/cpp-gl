#pragma once

#include "vertex_descriptor.hpp"
#include "detail/concepts.hpp"
#include "detail/default_types.hpp"

namespace gl {

template <
    detail::c_instantiation_of<vertex_descriptor> VertexType,
    detail::c_properties Properties = detail::empty_properties>
class edge_descriptor {
public:
    using vertex_type = VertexType;
    using properties_type = Properties;

    edge_descriptor() = delete;

private:
    // u, v (shared_ptr/weak_ptr/const unique_ptr&)
};

} // namespace gl