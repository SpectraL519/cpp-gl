#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "gl/vertex.hpp"
#include "gl/utility/type_traits.hpp"



namespace gl {

using directed_specifier_type = bool;
constexpr bool directed = true;
constexpr bool undirected = false;



template <
    directed_specifier_type directed_v = directed,
    vertex_descriptor_c vertex_t = gl::vertex_descriptor<>,
    graph_container_c container_t = gl::vector
>
class igraph {
public:
    using vertex_type = vertex_t;
    using vertex_ptr_type = std::unique_ptr<vertex_type>;
    using vertex_key_type = vertex_type::key_type;

    using edge_type = vertex_type::edge_type;

    using container_type = gl::container_traits_t<container_t, vertex_ptr_type>;
    using container_specifier = typename gl::container_traits<container_t, vertex_ptr_type>::container_specifier;

    static constexpr bool is_directed() { return directed_v; }


    virtual vertex_key_type num_vertices() const = 0;
    virtual std::size_t num_edges() const = 0;
    virtual std::size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual bool has_vertex(vertex_key_type key) const = 0;

    virtual const vertex_ptr_type& at(std::size_t idx) = 0;
    // TODO: virtual const std::optional<vertex_ptr_type>& get_vertex(vertex_key_type key) = 0;
    virtual const vertex_ptr_type& get_vertex(vertex_key_type key) = 0;
    virtual const container_type& vertices() = 0;

    virtual void add_vertex() = 0;
    virtual void add_vertices(vertex_key_type num_new_vertices) = 0;
    virtual void add_edge(vertex_key_type source_key, vertex_key_type destination_key) = 0;
    virtual void add_edge(edge_type&& edge) = 0;

    // TODO: add_vertex(data), add_edge(data)
};

template <typename graph_t>
concept gl_graph_c = std::is_base_of_v<
    igraph<graph_t::is_directed(), typename graph_t::vertex_type, typename graph_t::container_specifier>,
    graph_t>;



template <gl_graph_c graph_t>
constexpr bool is_directed() {
    return graph_t::is_directed();
}

} // namespace gl
