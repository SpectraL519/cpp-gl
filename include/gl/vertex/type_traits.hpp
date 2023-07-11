#ifndef CPP_GL_VERTEX_TYPE_TRAITS
#define CPP_GL_VERTEX_TYPE_TRAITS

#include <type_traits>
#include <concepts>

#include <gl/utility/types.hpp>
#include <gl/edge/type_traits.hpp>
#include <gl/vertex/vertex_descriptor.hpp>



namespace gl {

namespace vertex {

template <typename descriptor_t>
struct is_valid_descriptor : std::false_type {};

template <
    edge_descriptor_t edge_t,
    graph_container_s container_s,
    typename data_t
>
struct is_valid_descriptor <vertex_descriptor <edge_t, container_s, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<descriptor_t>::value;

} // namespace vertex

template <typename descriptor_t>
concept vertex_descriptor_t = vertex::is_valid_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_vertex_descriptor_t = vertex_descriptor_t<descriptor_t> && is_data_descriptor_v<descriptor_t>;

} // namespace gl

#endif // CPP_GL_VERTEX_TYPE_TRAITS