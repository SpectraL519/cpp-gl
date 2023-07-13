#ifndef CPP_GL_VERTEX_DESCRIPTOR
#define CPP_GL_VERTEX_DESCRIPTOR

#include <optional>

#include <gl/utility.hpp>
#include <gl/edge.hpp>



namespace gl {

template <
    edge_descriptor_t edge_t = edge_descriptor<std::size_t>,
    graph_container_s container_s = gl::vect_s,
    typename data_t = void
>
struct vertex_descriptor {
public:
    using key_type = edge_t::vertex_key_type;
    using edge_type = edge_t;
    using container_type = container_traits_t<container_s, edge_type>;

private:
    container_type _adjacent;
    std::function<void(container_type&, const edge_type&)> _container_insert = 
        container_traits<container_s, edge_type>::insert;

public:
    const key_type key;
    data_t data;

    // constructors & destructors
    vertex_descriptor (const key_type key, const data_t& data) 
        : key(key), data(data)
    {}

    explicit vertex_descriptor (const key_type key, const container_type& adjacent, const data_t& data) 
        : key(key), _adjacent(adjacent), data(data)
    {}

    explicit vertex_descriptor (
        const key_type key, 
        const vertex_descriptor<edge_t, container_s, data_t>& other
    ) 
        : _adjacent(other._adjacent), data(other.data)
    {}

    ~vertex_descriptor() = default;

    // member functions
    [[nodiscard]] const container_type& adjacent () const {
        return const_cast<container_type&>(this->_adjacent);
    }

    [[nodiscard]] std::size_t degree () const {
        return this->_adjacent.size();
    }

    bool add_edge (const edge_type& edge) {
        if (edge.source != this->key)
            return false;
        
        this->_container_insert(this->_adjacent, edge);
        return true;
    }
};



template <
    edge_descriptor_t edge_t,
    graph_container_s container_s
>
struct vertex_descriptor <edge_t, container_s, void> {
public:
    using key_type = edge_t::vertex_key_type;
    using edge_type = edge_t;
    using container_type = container_traits_t<container_s, edge_type>;

private:
    container_type _adjacent;
    std::function<void(container_type&, const edge_type&)> _container_insert = 
        container_traits<container_s, edge_type>::insert;

public:
    const key_type key;

    // constructors & destructors
    vertex_descriptor (const key_type key) : key(key) {}

    vertex_descriptor (const key_type key, const container_type& adjacent_) 
        : key(key), _adjacent(adjacent_)
    {}

    template <typename data_t = void>
    vertex_descriptor ( 
        const vertex_descriptor<edge_t, container_s, data_t>& other
    ) 
        : key(other.key), _adjacent(other._adjacent)
    {}

    ~vertex_descriptor() = default;

    // member functions
    [[nodiscard]] const container_type& adjacent () const {
        return const_cast<container_type&>(this->_adjacent);
    }

    [[nodiscard]] std::size_t degree () const {
        return this->_adjacent.size();
    }

    bool add_edge (const edge_type& edge) {
        if (edge.source != this->key)
            return false;
        
        this->_container_insert(this->_adjacent, edge);
        return true;
    }
};

} // namespace gl


#endif // CPP_GL_VERTEX_DESCRIPTOR