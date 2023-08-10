#ifndef CPP_GL_GRAPH_TRAITS
#define CPP_GL_GRAPH_TRAITS

#include <optional>
#include <functional>

#include <gl/utility.hpp>
#include <gl/edge.hpp>
#include <gl/vertex.hpp>



namespace gl {

template<
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

public:
    virtual std::size_t num_vertices() const = 0;
    virtual std::size_t num_edges() const = 0;
    virtual std::size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual bool has_vertex(const vertex_key_type& key) const = 0;

    virtual vertex_type& operator[](const std::size_t& idx) const = 0;
    virtual vertex_type& at(const std::size_t& idx) const = 0;
    virtual const container_type& vertices() const = 0;
    // TODO: has_vertex(data) get_vertex(data)

    virtual void add_vertex() = 0;
    virtual void add_vertices(const vertex_key_type& num_new_vertices) = 0;
    virtual void add_edge(const edge_type& edge) = 0;
    virtual void add_edges(const vertex_type::container_type& edges) = 0;

    // TODO: add_vertex(data), add_vertices(vector<data>)
    // TODO: remove methods for vertices and edges
};

} // namespace gl

#endif // CPP_GL_GRAPH_TRAITS
