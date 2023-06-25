#ifndef CPP_GL_GRAPH_TRAITS
#define CPP_GL_GRAPH_TRAITS

#include <edge/edge_traits.hpp>
#include <vertex/vertex_traits.hpp>



namespace gl {

template <vertex_descriptor_t vertex_t>
class IGraph {
    using vertex_key_t = vertex_t::key_t;
};

} // namespace gl

#endif // CPP_GL_GRAPH_TRAITS