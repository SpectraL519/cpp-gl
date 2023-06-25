#ifndef CPP_GL_EDGE_TRAITS
#define CPP_GL_EDGE_TRAITS

#include <optional>
#include <concepts>

#include <edge/edge.hpp>
#include <edge/weighted_edge.hpp>
#include <edge/flow_edge.hpp>
#include <edge/weighted_flow_edge.hpp>
#include <utility/type_traits.hpp>



namespace gl {

// edge container types
template <typename descriptor_t>
struct is_edge_descriptor : std::false_type {};

template <
    index_t vertex_key_t, 
    typename data_t
>
struct is_edge_descriptor <edge_descriptor <vertex_key_t, data_t>> : std::true_type {};

template <
    index_t vertex_key_t, 
    numerical_t weight_t, 
    typename data_t
>
struct is_edge_descriptor <weighted_edge_descriptor <vertex_key_t, weight_t, data_t>> : std::true_type {};

template <
    index_t vertex_key_t, 
    numerical_t flow_t, 
    typename data_t
>
struct is_edge_descriptor <flow_edge_descriptor <vertex_key_t, flow_t, data_t>> : std::true_type {};

template <
    index_t vertex_key_t, 
    numerical_t weight_t, 
    numerical_t flow_t, 
    typename data_t
>
struct is_edge_descriptor <weighted_flow_edge_descriptor <vertex_key_t, weight_t, flow_t, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_edge_descriptor_v = is_edge_descriptor<descriptor_t>::value;



template <typename descriptor_t>
concept edge_descriptor_t = is_edge_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_edge_descriptor_t = is_edge_descriptor_v<descriptor_t> && is_data_descriptor_v<descriptor_t>;

} // namespace gl

#endif // CPP_GL_EDGE_TRAITS