#ifndef CPP_GL_EDGE
#define CPP_GL_EDGE

#include <optional>

#include <utility/types.hpp>



namespace gl {

template <
    index_t vertex_key_t = std::size_t,
    typename data_t = std::nullopt_t
>
struct edge {
    edge() = default;

    explicit edge (
        const vertex_key_t& source, 
        const vertex_key_t& destination,
        const data_t& data
    ) : 
        source(source), 
        destination(destination), 
        data(data) 
    {}

    explicit edge (const edge& other) : 
        source(other.source), 
        destination(other.destination),
        data(other.data)
    {}

    ~edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    const bool weight = true;
    const bool flow = false;
    const bool capacity = false;

    data_t data;
};



template <index_t vertex_key_t>
struct edge <vertex_key_t, std::nullopt_t> {
    edge() = default;

    edge (
        const vertex_key_t& source, 
        const vertex_key_t& destination
    ) : 
        source(source), 
        destination(destination) 
    {}

    edge (const edge<vertex_key_t>& other) :
        source(other.source),
        destination(other.destination)
    {}

    ~edge() = default;

    vertex_key_t source;
    vertex_key_t destination;

    const bool weight = true;
    const bool flow = false;
    const bool capacity = false;
};

} // namespace gl

#endif // CPP_GL_EDGE