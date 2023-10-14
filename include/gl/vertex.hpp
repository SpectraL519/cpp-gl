#pragma once

#include "gl/edge/edge_descriptor.hpp"
#include "gl/utility.hpp"

#include <functional>
#include <memory>


namespace gl {

template <
    detail::u_integral Key, key_type_matching_edge_descriptor_c<Key> EdgeDescriptor,
    graph_container_c Container, detail::satisfies_or_void<detail::is_equality_comparable> Data>
class vertex_descriptor;


namespace vertex {

template <typename Descriptor>
struct is_valid_descriptor : std::false_type {};

template <
    detail::u_integral Key, key_type_matching_edge_descriptor_c<Key> EdgeDescriptor,
    graph_container_c Container, detail::satisfies_or_void<detail::is_equality_comparable> Data>
struct is_valid_descriptor<vertex_descriptor<Key, EdgeDescriptor, Container, Data>>
    : std::true_type {};

template <typename Descriptor>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<Descriptor>::value;

}  // namespace vertex

template <typename Descriptor>
concept vertex_descriptor_c = vertex::is_valid_descriptor_v<Descriptor>;

template <typename Descriptor>
concept data_vertex_descriptor_c =
    vertex_descriptor_c<Descriptor> && data_descriptor_t<Descriptor>;


template <
    detail::u_integral Key = std::size_t,
    key_type_matching_edge_descriptor_c<Key> EdgeDescriptor = edge_descriptor<Key>,
    graph_container_c Container = gl::vector,
    detail::satisfies_or_void<detail::is_equality_comparable> Data = void>
class vertex_descriptor {
public:
    using type = vertex_descriptor<Key, EdgeDescriptor, Container, Data>;
    using key_type = Key;
    using edge_type = EdgeDescriptor;
    using edge_ptr_type = std::unique_ptr<edge_type>;
    using container_type = gl::container_traits_t<Container, edge_ptr_type>;
    using data_type = Data;


    explicit vertex_descriptor(const key_type& key) : key(key) {}

    explicit vertex_descriptor(const key_type& key, const data_type& data)
        : key(key), _data(data) {}

    explicit vertex_descriptor(
        const key_type& key, const container_type& adjacent, const data_type& data
    )
        : key(key), _adjacent(adjacent), _data(data) {}

    explicit vertex_descriptor(const key_type& key, const container_type& adjacent_)
        : key(key), _adjacent(adjacent_) {}

    vertex_descriptor(const type& other)
        : key(other.key), _adjacent(other._adjacent), _data(other._data) {}

    vertex_descriptor(type&& other)
        : key(other.key), _adjacent(other._adjacent), _data(other._data) {}

    ~vertex_descriptor() = default;


    friend bool operator== (const type& lhs, const type& rhs) {
        return lhs.key == rhs.key && lhs._adjacent == rhs._adjacent &&
               lhs._data == rhs._data;
    }


    [[nodiscard]] inline const container_type& adjacent() {
        return this->_adjacent;
    }

    [[nodiscard]] inline std::size_t in_degree() const { return this->_in_deg; }

    [[nodiscard]] inline std::size_t out_degree() const {
        return this->_adjacent.size();
    }


    [[nodiscard]] inline std::size_t degree() const {
        return this->in_degree() + this->out_degree();
    }

    [[nodiscard]] inline const data_type& data() { return this->_data; }

    inline void set_data(data_type& data) { this->_data = data; }

    template <typename... T>
    inline void set_data(const T&... args) {
        this->_data = data_type(args...);
    }


    const key_type key;

private:
    container_type _adjacent;
    std::size_t _in_deg = 0;

    data_type _data = data_type();


    inline void _add_edge(key_type destination) {
        this->_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination));
    }

    inline void _add_edge(key_type destination, const data_type& data) {
        this->_insert(
            this->_adjacent, std::make_unique<edge_type>(this->key, destination, data)
        );
    }

    void _add_edge(edge_type&& edge) {
        if (edge.source != this->key)
            return;

        this->_insert(this->_adjacent, std::make_unique<edge_type>(std::move(edge)));
    }

    void _remove_edge(key_type destination) {
        _container_it edge_it = std::find_if(
            std::begin(this->_adjacent), std::end(this->_adjacent),
            [destination](const edge_ptr_type& edge) {
                return edge->destination == destination;
            }
        );

        this->_remove(this->_adjacent, edge_it);
    }


    using _container_traits = gl::container_traits<Container, edge_ptr_type>;
    using _container_it = _container_traits::iterator;

    std::function<void(container_type&, edge_ptr_type&&)> _insert =
        _container_traits::insert;
    std::function<void(container_type&, _container_it)> _remove =
        _container_traits::remove;


    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class graph;

    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class mutable_graph;
};


template <detail::u_integral Key, key_type_matching_edge_descriptor_c<Key> EdgeDescriptor, graph_container_c Container>
class vertex_descriptor<Key, EdgeDescriptor, Container, void> {
public:
    using type = vertex_descriptor<Key, EdgeDescriptor, Container, void>;
    using key_type = Key;
    using edge_type = EdgeDescriptor;
    using edge_ptr_type = std::unique_ptr<edge_type>;
    using container_type = gl::container_traits_t<Container, edge_ptr_type>;
    using data_type = void;


    explicit vertex_descriptor(const key_type& key) : key(key) {}

    explicit vertex_descriptor(const key_type& key, const container_type& adjacent_)
        : key(key), _adjacent(adjacent_) {}

    vertex_descriptor(const type& other)
        : key(other.key), _adjacent(other._adjacent) {}

    vertex_descriptor(type&& other)
        : key(other.key), _adjacent(other._adjacent) {}

    ~vertex_descriptor() = default;


    friend bool operator== (const type& lhs, const type& rhs) {
        return lhs.key == rhs.key && lhs._adjacent == rhs._adjacent;
    }


    [[nodiscard]] const container_type& adjacent() { return this->_adjacent; }

    [[nodiscard]] inline std::size_t in_degree() const { return this->_in_deg; }

    [[nodiscard]] inline std::size_t out_degree() const {
        return this->_adjacent.size();
    }

    [[nodiscard]] std::size_t degree() const {
        return this->in_degree() + this->out_degree();
    }


    const key_type key;

private:
    container_type _adjacent;
    std::size_t _in_deg = 0;


    inline void _add_edge(key_type destination) {
        this->_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination));
    }

    void _add_edge(edge_type&& edge) {
        if (edge.source != this->key)
            return;

        this->_insert(this->_adjacent, std::make_unique<edge_type>(std::move(edge)));
    }

    void _remove_edge(key_type destination) {
        _container_it edge_it = std::find_if(
            std::begin(this->_adjacent), std::end(this->_adjacent),
            [destination](const edge_ptr_type& edge) {
                return edge->destination == destination;
            }
        );

        this->_remove(this->_adjacent, edge_it);
    }


    using _container_traits = gl::container_traits<Container, edge_ptr_type>;
    using _container_it = _container_traits::iterator;

    std::function<void(container_type&, edge_ptr_type&&)> _insert =
        _container_traits::insert;
    std::function<void(container_type&, _container_it)> _remove =
        _container_traits::remove;


    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class graph;

    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class mutable_graph;
};

}  // namespace gl
