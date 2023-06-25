#ifndef CPP_GL_WEIGHTED_EDGE
#define CPP_GL_WEIGHTED_EDGE

#include <edge/edge_traits.hpp>
#include <utility/type_traits.hpp>



namespace gl {

template <
    index_t vertex_key_t = std::size_t,
    numerical_t weight_t = std::size_t,
    typename data_t = std::nullopt_t
>
struct weighted_edge {
    weighted_edge() = default;

    explicit weighted_edge (
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

    explicit weighted_edge (const weighted_edge<vertex_key_t>& other) : 
        source(other.source), 
        destination(other.destination),
        weight(other.weight),
        data(other.data)
    {}

    ~weighted_edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    weight_t weight;
    const bool flow = false;
    const bool capacity = false;

    data_t data;
};



template <index_t vertex_key_t, numerical_t weight_t>
struct weighted_edge <vertex_key_t, weight_t, std::nullopt_t> {
    weighted_edge() = default;

    weighted_edge (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const weight_t& weight
    ) : 
        source(source), 
        destination(destination),
        weight(weight)
    {}

    weighted_edge (const weighted_edge<vertex_key_t>& other) : 
        source(other.source), 
        destination(other.destination),
        weight(other.weight)
    {}

    ~weighted_edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    weight_t weight;
    const bool flow = false;
    const bool capacity = false;
};

} // namespace gl

#endif // CPP_GL_WEIGHTED_EDGE