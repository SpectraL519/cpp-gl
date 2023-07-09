#ifndef CPP_GL_EDGE_TYPE_TRAITS
#define CPP_GL_EDGE_TYPE_TRAITS

#include <type_traits>
#include <concepts>
#include <optional>

#include <gl/edge/edge_descriptor.hpp>
#include <gl/utility/types.hpp>



namespace gl {

namespace edge {

template <typename descriptor_t>
struct is_valid_descriptor : std::false_type {};

template <typename vertex_key_t, typename weight_t, typename flow_t, typename data_t>
struct is_valid_descriptor <edge_descriptor <vertex_key_t, weight_t, flow_t, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<descriptor_t>::value;

} // namespace edge

template <typename descriptor_t>
concept edge_descriptor_t = edge::is_valid_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_edge_descriptor_t = edge_descriptor_t<descriptor_t> && is_data_descriptor_v<descriptor_t>;



template <
    typename vertex_key_t, 
    typename weight_t = void, 
    typename flow_t = void, 
    typename data_t = void
>
struct edge_traits {
    using type = edge_descriptor<vertex_key_t, weight_t, flow_t, data_t>;
};

} // namespace gl

#endif // CPP_GL_EDGE_TYPE_TRAITS