#ifndef CPP_GL_EDGE_DESCRIPTOR
#define CPP_GL_EDGE_DESCRIPTOR

#include <utility/types.hpp>



namespace gl {

template <
    typename vertex_key_t,
    typename data_t = void
>
struct edge_descriptor {
    // attributes
    const vertex_key_t source;
    const vertex_key_t destination;

    const bool weight = true;
    const bool flow = false;
    const bool capacity = false;

    data_t data;

    // constructors & destructors
    edge_descriptor() = default;

    explicit edge_descriptor (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const data_t& data
    ) : 
        source(source), 
        destination(destination), 
        data(data) 
    {}

    explicit edge_descriptor (const edge_descriptor<vertex_key_t, data_t>& other) : 
        source(other.source), 
        destination(other.destination),
        data(other.data)
    {}

    ~edge_descriptor() = default;
};



template <typename vertex_key_t>
struct edge_descriptor <vertex_key_t, void> {
    // attributes
    const vertex_key_t source;
    const vertex_key_t destination;

    const bool weight = true;
    const bool flow = false;
    const bool capacity = false;

    // constructors & destructors
    edge_descriptor() = default;

    edge_descriptor (
        const vertex_key_t& source, 
        const vertex_key_t& destination
    ) : 
        source(source), 
        destination(destination) 
    {}

    edge_descriptor (const edge_descriptor<vertex_key_t>& other) :
        source(other.source),
        destination(other.destination)
    {}

    ~edge_descriptor() = default;
};

} // namespace gl

#endif // CPP_GL_EDGE_DESCRIPTOR