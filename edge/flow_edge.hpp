#ifndef CPP_GL_FLOW_EDGE_DESCRIPTOR
#define CPP_GL_FLOW_EDGE_DESCRIPTOR

#include <utility/types.hpp>



namespace gl {

template <
    typename vertex_key_t,
    arithmetic_t flow_t = std::size_t,
    typename data_t = void
>
struct flow_edge_descriptor {
    using vertex_key_type = vertex_key_t;
    using data_type = data_t;
    using weight_type = void;
    using flow_type = flow_t;

    // attributes
    const vertex_key_t source;
    const vertex_key_t destination;

    const bool weight = true;
    flow_t flow;
    flow_t capacity;

    data_t data;

    // constructors & destructors
    flow_edge_descriptor() = default;

    explicit flow_edge_descriptor (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const flow_t& flow,
        const flow_t& capacity,
        const data_t& data
    ) : 
        source(source), 
        destination(destination), 
        flow(flow),
        capacity(capacity),
        data(data) 
    {}

    explicit flow_edge_descriptor (
        const flow_edge_descriptor<vertex_key_t, flow_t, data_t>& other
    ) : 
        source(other.source), 
        destination(other.destination),
        flow(other.flow),
        capacity(other.capacity),
        data(other.data)
    {}

    ~flow_edge_descriptor() = default;
};



template <typename vertex_key_t, arithmetic_t flow_t>
struct flow_edge_descriptor <vertex_key_t, flow_t, void> {
    using vertex_key_type = vertex_key_t;
    using data_type = void;
    using weight_type = void;
    using flow_type = flow_t;

    // attributes
    vertex_key_t source;
    vertex_key_t destination;

    const bool weight = true;
    flow_t flow;
    flow_t capacity;

    // constructors & destructors
    flow_edge_descriptor() = default;

    flow_edge_descriptor (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const flow_t& flow,
        const flow_t& capacity
    ) : 
        source(source), 
        destination(destination),
        flow(flow),
        capacity(capacity)
    {}

    flow_edge_descriptor (const flow_edge_descriptor<vertex_key_t, flow_t>& other) : 
        source(other.source), 
        destination(other.destination),
        flow(other.flow),
        capacity(other.capacity)
    {}

    ~flow_edge_descriptor() = default;
};

} // namespace gl

#endif // CPP_GL_FLOW_EDGE_DESCRIPTOR