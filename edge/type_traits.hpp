#ifndef CPP_GL_EDGE_TYPE_TRAITS
#define CPP_GL_EDGE_TYPE_TRAITS

#include <type_traits>
#include <concepts>
#include <optional>

#include <edge/types.hpp>
#include <edge/edge.hpp>
#include <edge/weighted_edge.hpp>
#include <edge/flow_edge.hpp>
#include <edge/weighted_flow_edge.hpp>
#include <utility/types.hpp>



namespace gl {

namespace edge {

template <typename S>
struct is_valid : std::false_type {};

template <typename data_t>
struct is_valid <default_s <data_t>> : std::true_type {};
template <> struct is_valid <default_s <>> : std::true_type {};

template <numerical_t weight_t, typename data_t>
struct is_valid <weighted_s <weight_t, data_t>> : std::true_type {};

template <numerical_t flow_t, typename data_t>
struct is_valid <flow_s <flow_t, data_t>> : std::true_type {};

template <numerical_t weight_t, numerical_t flow_t, typename data_t>
struct is_valid <weighted_flow_s <weight_t, flow_t, data_t>> : std::true_type {};

template <typename S>
inline constexpr bool is_valid_v = is_valid<S>::value;

} // namespace edge

template <typename S>
concept graph_edge_s = edge::is_valid_v<S>;



template <typename vertex_key_t, graph_edge_s S = edge::default_s<void>>
struct edge_traits {};

template <typename vertex_key_t, typename data_t>
struct edge_traits <vertex_key_t, edge::default_s <data_t>> {
    typedef edge_descriptor<vertex_key_t, data_t> type;
};

template <typename vertex_key_t>
struct edge_traits <vertex_key_t, edge::default_s <>> {
    typedef edge_descriptor<vertex_key_t> type;
};

template <typename vertex_key_t, numerical_t weight_t, typename data_t>
struct edge_traits <vertex_key_t, edge::weighted_s <weight_t, data_t>> {
    #if data_t != void
        typedef weighted_edge_descriptor<vertex_key_t, weight_t, data_t> type;
    #else
        typedef weighted_edge_descriptor<vertex_key_t, weight_t> type;
    #endif
};

template <typename vertex_key_t, numerical_t flow_t, typename data_t>
struct edge_traits <vertex_key_t, edge::flow_s <flow_t, data_t>> {
    #if data_t != void
        typedef flow_edge_descriptor<vertex_key_t, flow_t, data_t> type;
    #else
        typedef flow_edge_descriptor<vertex_key_t, flow_t> type;
    #endif
};

template <typename vertex_key_t, numerical_t weight_t, numerical_t flow_t, typename data_t>
struct edge_traits <vertex_key_t, edge::weighted_flow_s <weight_t, flow_t, data_t>> {
    #if data_t != void
        typedef weighted_flow_edge_descriptor<vertex_key_t, weight_t, flow_t, data_t> type;
    #else
        typedef weighted_flow_edge_descriptor<vertex_key_t, weight_t, flow_t> type;
    #endif
};





/*

// edge container types
template <typename descriptor_t>
struct is_edge_descriptor : std::false_type {};

template <
    typename vertex_key_t, 
    typename data_t
>
struct is_edge_descriptor <edge_descriptor <vertex_key_t, data_t>> : std::true_type {};

template <
    typename vertex_key_t, 
    numerical_t weight_t, 
    typename data_t
>
struct is_edge_descriptor <weighted_edge_descriptor <vertex_key_t, weight_t, data_t>> : std::true_type {};

template <
    typename vertex_key_t, 
    numerical_t flow_t, 
    typename data_t
>
struct is_edge_descriptor <flow_edge_descriptor <vertex_key_t, flow_t, data_t>> : std::true_type {};

template <
    typename vertex_key_t, 
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

*/

} // namespace gl

#endif // CPP_GL_EDGE_TYPE_TRAITS