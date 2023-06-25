#ifndef CPP_GL_WEIGHTED_EDGE_DESCRIPTOR
#define CPP_GL_WEIGHTED_EDGE_DESCRIPTOR

#include <edge/edge_traits.hpp>
#include <utility/type_traits.hpp>



namespace gl {

template <
    index_t vertex_key_t = std::size_t,
    numerical_t weight_t = std::size_t,
    typename data_t = std::nullopt_t
>
struct weighted_edge_descriptor {
    // attributes
    const vertex_key_t source;
    const vertex_key_t destination;

    weight_t weight;
    const bool flow = false;
    const bool capacity = false;

    data_t data;

    // constructors & destructors
    weighted_edge_descriptor() = default;

    explicit weighted_edge_descriptor (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const weight_t& weight,
        const data_t& data
    ) : 
        source(source), 
        destination(destination), 
        weight(weight),
        data(data) 
    {}

    explicit weighted_edge_descriptor (
        const weighted_edge_descriptor<vertex_key_t, weight_t, data_t>& other
    ) : 
        source(other.source), 
        destination(other.destination),
        weight(other.weight),
        data(other.data)
    {}

    ~weighted_edge_descriptor() = default;
};



template <index_t vertex_key_t, numerical_t weight_t>
struct weighted_edge_descriptor <vertex_key_t, weight_t, std::nullopt_t> {
    // attributes
    const vertex_key_t source;
    const vertex_key_t destination;

    weight_t weight;
    const bool flow = false;
    const bool capacity = false;

    // constructors & destructors
    weighted_edge_descriptor() = default;

    weighted_edge_descriptor (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const weight_t& weight
    ) : 
        source(source), 
        destination(destination),
        weight(weight)
    {}

    weighted_edge_descriptor (const weighted_edge_descriptor<vertex_key_t, weight_t>& other) : 
        source(other.source), 
        destination(other.destination),
        weight(other.weight)
    {}

    ~weighted_edge_descriptor() = default;
};

} // namespace gl

#endif // CPP_GL_WEIGHTED_EDGE_DESCRIPTOR