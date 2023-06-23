#ifndef CPP_GL_FLOW_EDGE
#define CPP_GL_FLOW_EDGE

#include <edge/edge_traits.hpp>
#include <utility/types.hpp>



namespace gl {

template <
    index_t vertex_key_t = std::size_t,
    numerical_t flow_t = std::size_t,
    typename data_t = std::nullopt_t
>
struct flow_edge {
    flow_edge() = default;

    explicit flow_edge (
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

    explicit flow_edge (const flow_edge<vertex_key_t, flow_t, data_t>& other) : 
        source(other.source), 
        destination(other.destination),
        flow(other.flow),
        capacity(other.capacity),
        data(other.data)
    {}

    ~flow_edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    const bool weight = true;
    flow_t flow;
    flow_t capacity;

    data_t data;
};



template <index_t vertex_key_t, numerical_t flow_t>
struct flow_edge <vertex_key_t, flow_t, std::nullopt_t> {
    flow_edge() = default;

    flow_edge (
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

    flow_edge (const flow_edge<vertex_key_t, flow_t>& other) : 
        source(other.source), 
        destination(other.destination),
        flow(other.flow),
        capacity(other.capacity)
    {}

    ~flow_edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    const bool weight = true;
    flow_t flow;
    flow_t capacity;
};

} // namespace gl

#endif // CPP_GL_FLOW_EDGE