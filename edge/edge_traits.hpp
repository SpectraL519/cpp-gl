#ifndef CPP_GL_EDGE_TRAITS
#define CPP_GL_EDGE_TRAITS

#include <optional>
#include <concepts>

#include <edge/edge.hpp>
#include <edge/weighted_edge.hpp>
#include <edge/flow_edge.hpp>
#include <edge/weighted_flow_edge.hpp>
#include <utility/concepts.hpp>



namespace gl {

// edge container types
template <typename descriptor_t>
struct edge_descriptor_trait : std::false_type {};

template <
    index_t vertex_key_t, 
    typename data_t
>
struct edge_descriptor_trait <edge <vertex_key_t, data_t>> : std::true_type {};

template <
    index_t vertex_key_t, 
    numerical_t weight_t, 
    typename data_t
>
struct edge_descriptor_trait <weighted_edge <vertex_key_t, weight_t, data_t>> : std::true_type {};

template <
    index_t vertex_key_t, 
    numerical_t flow_t, 
    typename data_t
>
struct edge_descriptor_trait <flow_edge <vertex_key_t, flow_t, data_t>> : std::true_type {};

template <
    index_t vertex_key_t, 
    numerical_t weight_t, 
    numerical_t flow_t, 
    typename data_t
>
struct edge_descriptor_trait <weighted_flow_edge <vertex_key_t, weight_t, flow_t, data_t>> : std::true_type {};



template <typename descriptor_t>
concept edge_descriptor_t = edge_descriptor_trait<descriptor_t>::value;

template <typename descriptor_t>
concept data_edge_descriptor_t = edge_descriptor_t<descriptor_t> && is_data_struct<descriptor_t>;

} // namespace gl

#endif // CPP_GL_EDGE_TRAITS