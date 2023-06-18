#ifndef CPP_GL_WEIGHTED_FLOW_EDGE
#define CPP_GL_WEIGHTED_FLOW_EDGE

#include <utility/types.hpp>



namespace gl {

template <
    index_t vertex_key_t = std::size_t,
    numerical_t weight_t = std::size_t,
    numerical_t flow_t = std::size_t,
    typename data_t = std::nullopt_t
>
struct weighted_flow_edge {
    weighted_flow_edge() = default;

    explicit weighted_flow_edge (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const weight_t& weight,
        const flow_t& flow,
        const flow_t& capacity,
        const data_t& data
    ) : 
        source(source), 
        destination(destination), 
        weight(weight),
        flow(flow),
        capacity(capacity),
        data(data) 
    {}

    explicit weighted_flow_edge (
        const weighted_flow_edge<vertex_key_t, weight_t, flow_t, data_t>& other
    ) : 
        source(other.source), 
        destination(other.destination),
        flow(other.flow),
        capacity(other.capacity),
        data(other.data)
    {}

    ~weighted_flow_edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    weight_t weight;
    flow_t flow;
    flow_t capacity;

    data_t data;
};



template <index_t vertex_key_t, numerical_t weight_t, numerical_t flow_t>
struct weighted_flow_edge <vertex_key_t, weight_t, flow_t, std::nullopt_t> {
    weighted_flow_edge() = default;

    weighted_flow_edge (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const weight_t& weight,
        const flow_t& flow,
        const flow_t& capacity
    ) : 
        source(source), 
        destination(destination),
        weight(weight),
        flow(flow),
        capacity(capacity)
    {}

    weighted_flow_edge (
        const weighted_flow_edge<vertex_key_t, weight_t, flow_t>& other
    ) : 
        source(other.source), 
        destination(other.destination),
        weight(other.weight),
        flow(other.flow),
        capacity(other.capacity)
    {}

    ~weighted_flow_edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    weight_t weight;
    flow_t flow;
    flow_t capacity;
};

} // namespace gl

#endif // CPP_GL_WEIGHTED_FLOW_EDGE