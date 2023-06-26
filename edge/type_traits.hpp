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
struct is_valid_descriptor : std::false_type {};

template <typename data_t>
struct is_valid_descriptor <default_s <data_t>> : std::true_type {};
template <> struct is_valid_descriptor <default_s <>> : std::true_type {};

template <numerical_t weight_t, typename data_t>
struct is_valid_descriptor <weighted_s <weight_t, data_t>> : std::true_type {};

template <numerical_t flow_t, typename data_t>
struct is_valid_descriptor <flow_s <flow_t, data_t>> : std::true_type {};

template <numerical_t weight_t, numerical_t flow_t, typename data_t>
struct is_valid_descriptor <weighted_flow_s <weight_t, flow_t, data_t>> : std::true_type {};

template <typename S>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<S>::value;

} // namespace edge

template <typename S>
concept graph_edge_s = edge::is_valid_descriptor_v<S>;



template <typename vertex_key_t, graph_edge_s S = edge::default_s<void>>
struct edge_traits {};

template <typename vertex_key_t, typename data_t>
struct edge_traits <vertex_key_t, edge::default_s <data_t>> {
    using type = typename std::conditional<
        std::is_void_v<data_t>, 
        edge_descriptor<vertex_key_t>,
        edge_descriptor<vertex_key_t, data_t>
    >::type;
};

template <typename vertex_key_t, numerical_t weight_t, typename data_t>
struct edge_traits <vertex_key_t, edge::weighted_s <weight_t, data_t>> {
    using type = typename std::conditional<
        std::is_void_v<data_t>, 
        weighted_edge_descriptor<vertex_key_t, weight_t>,
        weighted_edge_descriptor<vertex_key_t, weight_t, data_t>
    >::type;
};

template <typename vertex_key_t, numerical_t flow_t, typename data_t>
struct edge_traits <vertex_key_t, edge::flow_s <flow_t, data_t>> {
    using type = typename std::conditional<
        std::is_void_v<data_t>, 
        flow_edge_descriptor<vertex_key_t, flow_t>,
        flow_edge_descriptor<vertex_key_t, flow_t, data_t>
    >::type;
};

template <typename vertex_key_t, numerical_t weight_t, numerical_t flow_t, typename data_t>
struct edge_traits <vertex_key_t, edge::weighted_flow_s <weight_t, flow_t, data_t>> {
    using type = typename std::conditional<
        std::is_void_v<data_t>, 
        weighted_flow_edge_descriptor<vertex_key_t, weight_t, flow_t>,
        weighted_flow_edge_descriptor<vertex_key_t, weight_t, flow_t, data_t>
    >::type;
};

} // namespace gl

#endif // CPP_GL_EDGE_TYPE_TRAITS