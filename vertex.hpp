#ifndef CPP_GL_VERTEX
#define CPP_GL_VERTEX

#include <utility/types.hpp>
#include <utility/concepts.hpp>



namespace gl {

template <
    index_t key_t = std::size_t,
    edge_descriptor_t edge_t = edge<key_t>,
    adjacent_container_t container_t = vect<edge_t>,
    typename data_t = std::nullopt_t
>
struct vertex_descriptor {
    vertex_descriptor() = default;

    explicit vertex_descriptor (
        const container_t& ajdacent,
        const data_t& data
    ) :
        ajdacent(adjacent),
        data(data)
    {}

    explicit vertex_descriptor (
        const vertex_descriptor<key_t, edge_t, container_t, data_t>& other
    ) :
        adjacent(other.adjacent),
        data(other.data)
    {}

    ~vertex_descriptor() = default;

    container_t adjacent;
    data_t data;
};



template <
    index_t key_t,
    edge_descriptor_t edge_t,
    adjacent_container_t container_t
>
struct vertex_descriptor {
    vertex_descriptor() = default;

    explicit vertex_descriptor (const container_t& ajdacent) :
        ajdacent(adjacent)
    {}

    explicit vertex_descriptor (
        const vertex_descriptor<key_t, edge_t, container_t>& other
    ) : 
        adjacent(other.adjacent)
    {}

    ~vertex_descriptor() = default;

    container_t adjacent;
};
    
} // namespace gl


#endif // CPP_GL_VERTEX