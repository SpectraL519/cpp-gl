#ifndef CPP_GL_WEIGHTED_FLOW_EDGE_DESCRIPTOR
#define CPP_GL_WEIGHTED_FLOW_EDGE_DESCRIPTOR

#include <utility/types.hpp>



namespace gl {

template <
    typename vertex_key_t,
    arithmetic_t weight_t = std::size_t,
    arithmetic_t flow_t = std::size_t,
    typename data_t = void
>
struct weighted_flow_edge_descriptor {
    using vertex_key_type = vertex_key_t;
    using data_type = data_t;
    using weight_type = weight_t;
    using flow_type = flow_t;

    // attributes
    const vertex_key_t source;
    const vertex_key_t destination;

    weight_t weight;
    flow_t flow;
    flow_t capacity;

    data_t data;

    // constructors & destructors
    weighted_flow_edge_descriptor() = default;

    explicit weighted_flow_edge_descriptor (
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

    explicit weighted_flow_edge_descriptor (
        const weighted_flow_edge_descriptor<vertex_key_t, weight_t, flow_t, data_t>& other
    ) : 
        source(other.source), 
        destination(other.destination),
        flow(other.flow),
        capacity(other.capacity),
        data(other.data)
    {}

    ~weighted_flow_edge_descriptor() = default;
};



template <typename vertex_key_t, arithmetic_t weight_t, arithmetic_t flow_t>
struct weighted_flow_edge_descriptor <vertex_key_t, weight_t, flow_t, void> {
    using vertex_key_type = vertex_key_t;
    using data_type = void;
    using weight_type = weight_t;
    using flow_type = flow_t;
    
    // attributes
    const vertex_key_t source;
    const vertex_key_t destination;

    weight_t weight;
    flow_t flow;
    flow_t capacity;

    // constructors & destructors
    weighted_flow_edge_descriptor() = default;

    weighted_flow_edge_descriptor (
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

    weighted_flow_edge_descriptor (
        const weighted_flow_edge_descriptor<vertex_key_t, weight_t, flow_t>& other
    ) : 
        source(other.source), 
        destination(other.destination),
        weight(other.weight),
        flow(other.flow),
        capacity(other.capacity)
    {}

    ~weighted_flow_edge_descriptor() = default;
};

} // namespace gl

#endif // CPP_GL_WEIGHTED_FLOW_EDGE_DESCRIPTOR