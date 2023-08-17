#pragma once

#include <memory>
#include <functional>
#include <optional>

#include <gl/utility.hpp>
#include <gl/edge.hpp>
#include <gl/vertex.hpp>



namespace gl {

template <
    bool DIRECTED = true,
    vertex_descriptor_t vertex_t = gl::vertex_descriptor<>,
    graph_container_t container_s = gl::vector
>
class igraph {
public:
    using vertex_type = vertex_t;
    using vertex_ptr = std::unique_ptr<vertex_type>;
    using vertex_key_type = vertex_type::key_type;
    using edge_type = vertex_type::edge_type;
    using container_type = gl::container_traits_t<container_s, vertex_ptr>;

public:
    virtual vertex_key_type num_vertices() const = 0;
    virtual std::size_t num_edges() const = 0;
    virtual std::size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual bool has_vertex(vertex_key_type key) const = 0;

    virtual const vertex_ptr& at(std::size_t idx) = 0;
    virtual const vertex_ptr& get_vertex(vertex_key_type key) = 0;
    virtual const container_type& vertices() = 0;

    virtual void add_vertex() = 0;
    virtual void add_vertices(vertex_key_type num_new_vertices) = 0;
    virtual void add_edge(vertex_key_type source_key, vertex_key_type destination_key) = 0;
    virtual void add_edge(edge_type&& edge) = 0;
};

} // namespace gl
