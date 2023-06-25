#ifndef CPP_GL_VERTEX_TRAITS
#define CPP_GL_VERTEX_TRAITS

#include <optional>
#include <concepts>

#include <utility/type_traits.hpp>
#include <vertex/adjacent_container.hpp>
#include <vertex/vertex_descriptor.hpp>



namespace gl {

template <typename descriptor_t>
struct is_vertex_descriptor : std::false_type {};

template <
    index_t key_t,
    edge_descriptor_t edge_t, 
    adjacent_container_t container_t, 
    typename data_t
>
struct is_vertex_descriptor <vertex_descriptor <key_t, edge_t, container_t, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_vertex_descriptor_v = is_vertex_descriptor<descriptor_t>::value;



template <typename descriptor_t>
concept vertex_descriptor_t = is_vertex_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_vertex_descriptor_t = is_vertex_descriptor_v<descriptor_t> && is_data_descriptor_v<descriptor_t>;

} // namespace gl

#endif // CPP_GL_VERTEX_TRAITS