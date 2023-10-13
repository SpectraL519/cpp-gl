#pragma once

#include <memory>
#include <functional>
#include <optional>

#include "gl/utility/types.hpp"
#include "gl/utility/type_traits.hpp"
#include "gl/vertex.hpp"



namespace gl::detail {

template <
    directed_specifier_type Directed,
    vertex_descriptor_c VertexDescriptor,
    graph_container_c Container
>
class abstract_graph_base {
public:
    using vertex_type = VertexDescriptor;
    using vertex_ptr_type = std::unique_ptr<vertex_type>;
    using vertex_key_type = vertex_type::key_type;

    using edge_type = vertex_type::edge_type;

    using container_type = gl::container_traits_t<Container, vertex_ptr_type>;
    using container_specifier =
        typename gl::container_traits<Container, vertex_ptr_type>::container_specifier;

    static constexpr bool is_directed() { return Directed; }


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

    virtual ~abstract_graph_base() = default;
};

} // namespace gl::DETAIL
