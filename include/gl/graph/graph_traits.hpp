#ifndef CPP_GL_GRAPH_TRAITS
#define CPP_GL_GRAPH_TRAITS

#include <optional>
#include <functional>

#include <gl/utility.hpp>
#include <gl/edge.hpp>
#include <gl/vertex.hpp>



namespace gl {

template <bool DIRECTED, vertex_descriptor_t vertex_t, graph_container_s container_s>
class igraph {
public:
    using vertex_key_type = vertex_t::key_type;
    using vertex_type = vertex_t;
    using edge_type = vertex_t::edge_type;
    using container_type = gl::container_traits_t<container_s, vertex_type>;

public:
    virtual std::size_t num_vertices() const = 0;
    virtual std::size_t num_edges() const = 0;
    virtual bool empty() const = 0;
    virtual bool has_vertex(const vertex_key_type& vertex_key) const = 0;

    virtual vertex_type& operator[](const std::size_t& index) const = 0;
    virtual vertex_type& at(const std::size_t& index) const = 0;
    virtual std::optional<std::reference_wrapper<vertex_type>> 
            get_vertex(const vertex_key_type& vertex_key) = 0;
    virtual const container_type& vertices() const = 0;

    virtual void add_vertex(const vertex_key_type& vertex_key) = 0;
    virtual void add_vertex(const vertex_type& vertex) = 0;
    virtual bool add_edge(const edge_type& edge) = 0;
};

} // namespace gl

#endif // CPP_GL_GRAPH_TRAITS