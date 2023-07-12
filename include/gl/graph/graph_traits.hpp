#ifndef CPP_GL_GRAPH_TRAITS
#define CPP_GL_GRAPH_TRAITS

#include <initializer_list>

#include <gl/utility.hpp>
#include <gl/edge.hpp>
#include <gl/vertex.hpp>



namespace gl {

template <
    bool DIRECTED = true,
    vertex_descriptor_t vertex_t = gl::vertex_descriptor<>, 
    graph_container_s container_s = gl::vect_s
>
class igraph {
public:
    using vertex_key_type = vertex_t::key_type;
    using vertex_type = vertex_t;
    using edge_type = vertex_t::edge_type;
    using container_type = gl::container_traits_t<container_s, vertex_type>;
    using adj_container_type = vertex_t::container_type;

public:
    virtual std::size_t num_vertices () const = 0;
    virtual std::size_t num_edges () const = 0;
    virtual bool empty () const = 0;

    virtual void add_vertex (const vertex_key_type& vertex_key) = 0; // TODO data vertices
    virtual void add_edge (const edge_type& edge) = 0;

    // template <typename... T> // how to make a virtual func with this behaviour?
    // void add_edge (const vertex_key_type& source, const vertex_key_type& destination, T... args) {
    //     this->add_edge(edge_type(source, destination, args...));
    // }
};

} // namespace gl

#endif // CPP_GL_GRAPH_TRAITS