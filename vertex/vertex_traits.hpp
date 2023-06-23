#ifndef CPP_GL_VERTEX_TRAITS
#define CPP_GL_VERTEX_TRAITS

#include <optional>
#include <concepts>

#include <utility/types.hpp>
#include <vertex/adjacent_container.hpp>
#include <vertex/vertex_descriptor.hpp>



namespace gl {

template <typename descriptor_t>
struct vertex_descriptor_trait : std::false_type {};

template <
    index_t key_t,
    edge_descriptor_t edge_t, 
    adjacent_container_t container_t, 
    typename data_t
>
struct vertex_descriptor_trait <vertex_descriptor <key_t, edge_t, container_t, data_t>> : std::true_type {};



template <typename descriptor_t>
concept vertex_descriptor_t = vertex_descriptor_trait<descriptor_t>::value;

template <typename descriptor_t>
concept data_vertex_descriptor_t = vertex_descriptor_trait<descriptor_t>::value && is_data_struct<descriptor_t>;

} // namespace gl

#endif // CPP_GL_VERTEX_TRAITS