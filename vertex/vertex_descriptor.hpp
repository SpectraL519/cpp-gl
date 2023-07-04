#ifndef CPP_GL_VERTEX_DESCRIPTOR
#define CPP_GL_VERTEX_DESCRIPTOR

#include <optional>

#include <edge/type_traits.hpp>
#include <edge/edge.hpp>
#include <utility/types.hpp>
#include <utility/type_traits.hpp>



namespace gl {

template <
    index_t key_t = std::size_t,
    graph_edge_s edge_s = gl::edge::default_s<>,
    adjacency_container_s container_s = gl::container::vect_s,
    typename data_t = void
>
struct vertex_descriptor {
public:
    using key_type = key_t;
    using edge_type = edge_traits<key_t, edge_s>::type;
    using container_type = container_traits<container_s, edge_type>::type;

private:
    container_type _adjacent;

public:
    data_t data;

    // constructors & destructors
    vertex_descriptor() = default;

    explicit vertex_descriptor (
        const container_type& adjacent_,
        const data_t& data
    ) :
        _adjacent(adjacent_),
        data(data)
    {}

    explicit vertex_descriptor (
        const vertex_descriptor<key_t, edge_s, container_s, data_t>& other
    ) :
        _adjacent(other._adjacent),
        data(other.data)
    {}

    ~vertex_descriptor() = default;

    // member functions
    [[nodiscard]] container_type& adjacent () const noexcept {
        return const_cast<container_type&>(this->_adjacent);
    }

    [[nodiscard]] std::size_t degree () const noexcept {
        return this->_adjacent.size();
    }
};



template <
    index_t key_t,
    graph_edge_s edge_s,
    adjacency_container_s container_s
>
struct vertex_descriptor <key_t, edge_s, container_s, void> {
public:
    using key_type = key_t;
    using edge_type = edge_traits<key_t, edge_s>::type;
    using container_type = container_traits<container_s, edge_type>::type;

private:
    container_type _adjacent;

public:
    // constructors & destructors
    vertex_descriptor() = default;

    vertex_descriptor (const container_type& adjacent_) 
        : _adjacent(adjacent_)
    {}

    vertex_descriptor (
        const vertex_descriptor<key_t, edge_s, container_s, void>& other
    ) : 
        _adjacent(other._adjacent)
    {}

    ~vertex_descriptor() = default;

    // member functions
    [[nodiscard]] container_type& adjacent () const {
        return const_cast<container_type&>(this->_adjacent);
    }

    [[nodiscard]] std::size_t degree () const noexcept {
        return this->_adjacent.size();
    }
};

} // namespace gl


#endif // CPP_GL_VERTEX_DESCRIPTOR