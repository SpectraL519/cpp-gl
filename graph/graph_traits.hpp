#ifndef CPP_GL_GRAPH_TRAITS
#define CPP_GL_GRAPH_TRAITS

#include <edge/type_traits.hpp>
#include <vertex/type_traits.hpp>
#include <vertex/vertex_descriptor.hpp>



namespace gl {

template <
    vertex_descriptor_t vertex_t,
    typename vertex_name_t = void,
    graph_container_s container_s = gl::container::vect_s
>
class IGraph {
public:
    using container_type = container_traits<container_s, typename vertex_t::key_type, vertex_name_t>::type;
};

} // namespace gl

#endif // CPP_GL_GRAPH_TRAITS