#ifndef CPP_GL_EDGE_DESCRIPTOR
#define CPP_GL_EDGE_DESCRIPTOR

#include <gl/utility.hpp>
#include <gl/edge/edge_data.hpp>



namespace gl {

// edge struct forward declaration
template <
    typename vertex_key_t,
    detail::satisfies_or_void<detail::equality_comparable_s> data_t
>
struct edge_descriptor;



// valid edge descriptor concepts
namespace edge {

template <typename descriptor_t>
struct is_valid_descriptor : std::false_type {};

template <typename vertex_key_t, typename data_t>
struct is_valid_descriptor <edge_descriptor <vertex_key_t, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<descriptor_t>::value;

} // namespace edge

template <typename descriptor_t>
concept edge_descriptor_t = edge::is_valid_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_edge_descriptor_t = edge_descriptor_t<descriptor_t> && is_data_descriptor_v<descriptor_t>;

template <typename descriptor_t, typename vertex_key_t>
concept key_type_edge_descriptor_t = edge_descriptor_t<descriptor_t> && 
                                     std::is_same_v<typename descriptor_t::vertex_key_type, vertex_key_t>;



// edge struct definition
template <
    typename vertex_key_t = std::size_t,
    detail::satisfies_or_void<detail::equality_comparable_s> data_t = void
>
struct edge_descriptor {
public:
    // type definitions
    using vertex_key_type = vertex_key_t;
    using data_type = data_t;

    // attributes
    const vertex_key_type source;
    const vertex_key_type destination;

private:
    data_type _data = data_type();

public:
    // constructors & destructors
    edge_descriptor() = default;
    ~edge_descriptor() = default;

    explicit edge_descriptor(const vertex_key_type& source, const vertex_key_type& destination)
        : source(source), destination(destination)
    {}

    explicit edge_descriptor(
        const vertex_key_type& source, const vertex_key_type& destination,
        const data_type& data
    ) : 
        source(source), destination(destination), 
        _data(data) 
    {}

    edge_descriptor(const edge_descriptor<vertex_key_type, data_type>& other) 
        : source(other.source), destination(other.destination),
          _data(other._data)
    {}

    edge_descriptor(edge_descriptor<vertex_key_type, data_type>&& other) 
        : source(other.source), destination(other.destination),
          _data(other._data)
    {}

    edge_descriptor(const edge_descriptor<vertex_key_type, void>& other) 
        : source(other.source), destination(other.destination)
    {}

    edge_descriptor(edge_descriptor<vertex_key_type, void>&& other) 
        : source(other.source), destination(other.destination)
    {}

    // operators
    friend bool operator==(const edge_descriptor<vertex_key_type, data_type>& lhs,
                           const edge_descriptor<vertex_key_type, data_type>& rhs) {
        return lhs.source == rhs.source &&
               lhs.destination == rhs.destination &&
               lhs._data == rhs._data;
    }

    // member functions
    [[nodiscard]] inline data_type& data() const {
        return const_cast<data_type&>(this->_data);
    }

    inline void set_data(const data_type& data) {
        this->_data = data;
    }

    [[nodiscard]] inline edge_descriptor<vertex_key_type, data_type> reverse() {
        return edge_descriptor<vertex_key_type, data_type>(this->destination, this->source, this->_data);
    }

    [[nodiscard]] inline edge_descriptor<vertex_key_type, data_type> reverse(const data_type& reverse_data) {
        return edge_descriptor<vertex_key_type, data_type>(this->destination, this->source, reverse_data);
    }
};



// void data edge struct definition
template <typename vertex_key_t>
struct edge_descriptor<vertex_key_t, void> {
public:
    // type definitions
    using vertex_key_type = vertex_key_t;
    using data_type = void;

    // attributes
    const vertex_key_type source;
    const vertex_key_type destination;

public:
    // constructors & destructors
    edge_descriptor() = default;
    ~edge_descriptor() = default;

    explicit edge_descriptor(const vertex_key_type& source, const vertex_key_type& destination)
        : source(source), destination(destination)
    {}

    edge_descriptor(const edge_descriptor<vertex_key_type, void>& other) 
        : source(other.source), destination(other.destination)
    {}

    edge_descriptor(edge_descriptor<vertex_key_type, void>&& other) 
        : source(other.source), destination(other.destination)
    {}

    // operators
    friend bool operator==(const edge_descriptor<vertex_key_type, data_type>& lhs,
                           const edge_descriptor<vertex_key_type, data_type>& rhs) {
        return lhs.source == rhs.source &&
               lhs.destination == rhs.destination;
    }

    // member functions
    [[nodiscard]] inline edge_descriptor<vertex_key_type, void> reverse() {
        return edge_descriptor<vertex_key_type, void>(this->destination, this->source);
    }
};

} // namespace gl

#endif // CPP_GL_EDGE_DESCRIPTOR