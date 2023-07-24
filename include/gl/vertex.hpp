#ifndef CPP_GL_VERTEX
#define CPP_GL_VERTEX

#include <optional>

#include <gl/utility.hpp>
#include <gl/edge.hpp>



namespace gl {

// vertex struct forward declaration
template <
    typename key_t,
    key_type_edge_descriptor_t<key_t> edge_t,
    graph_container_s container_s,
    typename data_t
>
struct vertex_descriptor;

// valid vertex descriptor concepts
namespace vertex {

template <typename descriptor_t>
struct is_valid_descriptor : std::false_type {};

template <
    typename key_t,
    key_type_edge_descriptor_t<key_t> edge_t,
    graph_container_s container_s,
    typename data_t
>
struct is_valid_descriptor <vertex_descriptor <key_t, edge_t, container_s, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<descriptor_t>::value;

} // namespace vertex

template <typename descriptor_t>
concept vertex_descriptor_t = vertex::is_valid_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_vertex_descriptor_t = vertex_descriptor_t<descriptor_t> && is_data_descriptor_v<descriptor_t>;




// vertex struct definition
template <
    typename key_t = std::size_t,
    key_type_edge_descriptor_t<key_t> edge_t = edge_descriptor<key_t>,
    graph_container_s adj_container_s = gl::vect_s,
    typename data_t = void
>
struct vertex_descriptor
{
public:
    using key_type = key_t;
    using edge_type = edge_t;
    using container_type = container_traits_t<adj_container_s, edge_type>;
    using data_type = data_t;

private:
    container_type _adjacent;
    std::size_t _in_deg = 0;

    std::function<void(container_type&, const edge_type&)> _container_insert = 
        container_traits<adj_container_s, edge_type>::insert;

    template <bool DIRECTED, vertex_descriptor_t vertex_t, graph_container_s container_s>
    friend class graph; // friend graph class forward declaration

    data_type _data = data_type();

public:
    const key_type key;

    // constructors & destructors
    vertex_descriptor (const key_type key) : key(key) {}

    explicit vertex_descriptor (const key_type key, const data_type& data) 
        : key(key), _data(data)
    {}

    explicit vertex_descriptor (const key_type key, const container_type& adjacent, const data_type& data) 
        : key(key), _adjacent(adjacent), _data(data)
    {}

    explicit vertex_descriptor (const key_type key, const container_type& adjacent_) 
        : key(key), _adjacent(adjacent_)
    {}

    vertex_descriptor (const vertex_descriptor<edge_t, adj_container_s, data_t>& other) 
        : key(other.key), _adjacent(other._adjacent), _data(other.data)
    {}

    ~vertex_descriptor() = default;

    // member functions
    [[nodiscard]] inline const container_type& adjacent () const {
        return const_cast<container_type&>(this->_adjacent);
    }

    [[nodiscard]] inline std::size_t in_deg () const {
        return this->_in_deg;
    }

    [[nodiscard]] inline std::size_t out_deg () const {
        return this->_adjacent.size();
    }


    [[nodiscard]] inline std::size_t degree () const {
        return this->in_deg() + this->out_deg();
    }

    bool add_edge (const edge_type& edge) {
        if (edge.source != this->key)
            return false;
        
        this->_container_insert(this->_adjacent, edge);
        return true;
    }

    template <typename Data = data_t> requires (!std::is_void_v<Data>)
    [[nodiscard]] inline data_type& data () const {
        return this->_data;
    }

    template <typename Data = data_t> requires (!std::is_void_v<Data>)
    inline void set_data (data_type& data) {
        this->_data = data;
    }
};



template <
    typename key_t,
    key_type_edge_descriptor_t<key_t> edge_t,
    graph_container_s adj_container_s
>
struct vertex_descriptor <key_t, edge_t, adj_container_s, void> {
public:
    using key_type = key_t;
    using edge_type = edge_t;
    using container_type = container_traits_t<adj_container_s, edge_type>;
    using data_type = void;

private:
    container_type _adjacent;
    std::size_t _in_deg = 0;

    std::function<void(container_type&, const edge_type&)> _container_insert = 
        container_traits<adj_container_s, edge_type>::insert;

    template <bool DIRECTED, vertex_descriptor_t vertex_t, graph_container_s container_s>
    friend class graph; // friend graph class forward declaration

public:
    const key_type key;

    // constructors & destructors
    vertex_descriptor (const key_type key) : key(key) {}

    explicit vertex_descriptor (const key_type key, const container_type& adjacent_) 
        : key(key), _adjacent(adjacent_)
    {}

    template <typename data_t = void>
    vertex_descriptor (const vertex_descriptor<edge_t, adj_container_s, data_t>& other) 
        : key(other.key), _adjacent(other._adjacent)
    {}

    ~vertex_descriptor() = default;

    // member functions
    [[nodiscard]] const container_type& adjacent () const {
        return const_cast<container_type&>(this->_adjacent);
    }

    [[nodiscard]] inline std::size_t in_deg () const {
        return this->_in_deg;
    }

    [[nodiscard]] inline std::size_t out_deg () const {
        return this->_adjacent.size();
    }

    [[nodiscard]] std::size_t degree () const {
        return this->in_deg() + this->out_deg();
    }

    bool add_edge (const edge_type& edge) {
        if (edge.source != this->key)
            return false;
        
        this->_container_insert(this->_adjacent, edge);
        return true;
    }
};

} // namespace gl

#endif // CPP_GL_VERTEX