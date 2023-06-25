#ifndef CPP_GL_VERTEX_DESCRIPTOR
#define CPP_GL_VERTEX_DESCRIPTOR

#include <edge/edge_traits.hpp>
#include <edge/edge.hpp>
#include <vertex/vertex_traits.hpp>
#include <utility/types.hpp>
#include <utility/type_traits.hpp>



namespace gl {

template <
    index_t key_t = std::size_t,
    edge_descriptor_t edge_t = edge_descriptor<key_t>,
    adjacent_container_t container_t = vect<edge_t>,
    typename data_t = std::nullopt_t
>
struct vertex_descriptor {
private:
    container_t _adjacent;

public:
    data_t data;

    // constructors & destructors
    vertex_descriptor() = default;

    explicit vertex_descriptor (
        const container_t& adjacent_,
        const data_t& data
    ) :
        _adjacent(adjacent_),
        data(data)
    {}

    explicit vertex_descriptor (
        const vertex_descriptor<key_t, edge_t, container_t, data_t>& other
    ) :
        _adjacent(other._adjacent),
        data(other.data)
    {}

    ~vertex_descriptor() = default;

    // member functions
    container_t& adjacent () const {
        return const_cast<container_t&>(this->_adjacent);
    }
};



template <
    index_t key_t,
    edge_descriptor_t edge_t,
    adjacent_container_t container_t
>
struct vertex_descriptor <key_t, edge_t, container_t, std::nullopt_t> {
private:
    container_t _adjacent;

public:
    // constructors & destructors
    vertex_descriptor() = default;

    explicit vertex_descriptor (const container_t& adjacent_) 
        : _adjacent(adjacent_)
    {}

    explicit vertex_descriptor (
        const vertex_descriptor<key_t, edge_t, container_t>& other
    ) : 
        _adjacent(other._adjacent)
    {}

    ~vertex_descriptor() = default;

    // member functions
    container_t& adjacent () const {
        return const_cast<container_t&>(this->_adjacent);
    }
};
    
} // namespace gl


#endif // CPP_GL_VERTEX_DESCRIPTOR