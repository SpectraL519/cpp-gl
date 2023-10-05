#pragma once

#include <memory>
#include <functional>

#include "gl/utility.hpp"
#include "gl/edge/edge_descriptor.hpp"



namespace gl {

template <
    detail::u_integral key_t,
    key_type_matching_edge_descriptor_c<key_t> edge_t,
    graph_container_c adj_container_t,
    detail::satisfies_or_void<detail::is_equality_comparable> data_t
>
class vertex_descriptor;



namespace vertex {

template <typename descriptor_t>
struct is_valid_descriptor : std::false_type {};

template <
    detail::u_integral key_t,
    key_type_matching_edge_descriptor_c<key_t> edge_t,
    graph_container_c adj_container_t,
    detail::satisfies_or_void<detail::is_equality_comparable> data_t
>
struct is_valid_descriptor <vertex_descriptor <key_t, edge_t, adj_container_t, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<descriptor_t>::value;

} // namespace vertex

template <typename descriptor_t>
concept vertex_descriptor_c = vertex::is_valid_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_vertex_descriptor_c = vertex_descriptor_c<descriptor_t> && data_descriptor_t<descriptor_t>;



template <
    detail::u_integral key_t = std::size_t,
    key_type_matching_edge_descriptor_c<key_t> edge_t = edge_descriptor<key_t>,
    graph_container_c adj_container_t = gl::vector,
    detail::satisfies_or_void<detail::is_equality_comparable> data_t = void
>
class vertex_descriptor {
private:
    using _vertex_type = vertex_descriptor<key_t, edge_t, adj_container_t, data_t>;

public:
    using key_type = key_t;
    using edge_type = edge_t;
    using edge_ptr_type = std::unique_ptr<edge_type>;
    using container_type = gl::container_traits_t<adj_container_t, edge_ptr_type>;
    using data_type = data_t;


    explicit vertex_descriptor(const key_type& key) : key(key) {}

    explicit vertex_descriptor(const key_type& key, const data_type& data)
        : key(key), _data(data)
    {}

    explicit vertex_descriptor(const key_type& key, const container_type& adjacent, const data_type& data)
        : key(key), _adjacent(adjacent), _data(data)
    {}

    explicit vertex_descriptor(const key_type& key, const container_type& adjacent_)
        : key(key), _adjacent(adjacent_)
    {}

    vertex_descriptor(const _vertex_type& other)
        : key(other.key), _adjacent(other._adjacent), _data(other._data)
    {}

    vertex_descriptor(_vertex_type&& other)
        : key(other.key), _adjacent(other._adjacent), _data(other._data)
    {}

    ~vertex_descriptor() = default;


    friend bool operator==(const _vertex_type& lhs, const _vertex_type& rhs) {
        return lhs.key == rhs.key &&
               lhs._adjacent == rhs._adjacent &&
               lhs._data == rhs._data;
    }


    [[nodiscard]] inline const container_type& adjacent() {
        return this->_adjacent;
    }

    [[nodiscard]] inline std::size_t in_degree() const {
        return this->_in_deg;
    }

    [[nodiscard]] inline std::size_t out_degree() const {
        return this->_adjacent.size();
    }


    [[nodiscard]] inline std::size_t degree() const {
        return this->in_degree() + this->out_degree();
    }

    [[nodiscard]] inline const data_type& data() {
        return this->_data;
    }

    inline void set_data(data_type& data) {
        this->_data = data;
    }

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
        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination));
    }

    inline void _add_edge(key_type destination, const data_type& data) {
        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination, data));
    }

    void _add_edge(edge_type&& edge) {
        if (edge.source != this->key)
            return;

        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(std::move(edge)));
    }

    void _remove_edge(key_type destination) {
        _container_it edge_it = std::find_if(
            std::begin(this->_adjacent),
            std::end(this->_adjacent),
            [destination](const edge_ptr_type& edge) {
                return edge->destination == destination;
            }
        );

        this->_container_remove(this->_adjacent, edge_it);
    }


    using _container_traits = gl::container_traits<adj_container_t, edge_ptr_type>;
    using _container_it = _container_traits::iterator;

    std::function<void(container_type&, edge_ptr_type&&)> _container_insert = _container_traits::insert;
    std::function<void(container_type&, _container_it)> _container_remove = _container_traits::remove;


    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class graph;

    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class mutable_graph;
};



template <
    detail::u_integral key_t,
    key_type_matching_edge_descriptor_c<key_t> edge_t,
    graph_container_c adj_container_t
>
class vertex_descriptor <key_t, edge_t, adj_container_t, void> {
private:
    using _vertex_type = vertex_descriptor<key_t, edge_t, adj_container_t, void>;

public:
    using key_type = key_t;
    using edge_type = edge_t;
    using edge_ptr_type = std::unique_ptr<edge_type>;
    using container_type = gl::container_traits_t<adj_container_t, edge_ptr_type>;
    using data_type = void;


    explicit vertex_descriptor(const key_type& key) : key(key) {}

    explicit vertex_descriptor(const key_type& key, const container_type& adjacent_)
        : key(key), _adjacent(adjacent_)
    {}

    template <typename data_t = void>
    vertex_descriptor(const _vertex_type& other)
        : key(other.key), _adjacent(other._adjacent)
    {}

    template <typename data_t = void>
    vertex_descriptor(_vertex_type&& other)
        : key(other.key), _adjacent(other._adjacent)
    {}

    ~vertex_descriptor() = default;


    friend bool operator==(const _vertex_type& lhs, const _vertex_type& rhs) {
        return lhs.key == rhs.key &&
               lhs._adjacent == rhs._adjacent;
    }


    [[nodiscard]] const container_type& adjacent() {
        return this->_adjacent;
    }

    [[nodiscard]] inline std::size_t in_degree() const {
        return this->_in_deg;
    }

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
        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination));
    }

    void _add_edge (edge_type&& edge) {
        if (edge.source != this->key)
            return;

        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(std::move(edge)));
    }

    void _remove_edge(key_type destination) {
        _container_it edge_it = std::find_if(
            std::begin(this->_adjacent),
            std::end(this->_adjacent),
            [destination](const edge_ptr_type& edge) {
                return edge->destination == destination;
            }
        );

        this->_container_remove(this->_adjacent, edge_it);
    }


    using _container_traits = gl::container_traits<adj_container_t, edge_ptr_type>;
    using _container_it = _container_traits::iterator;

    std::function<void(container_type&, edge_ptr_type&&)> _container_insert = _container_traits::insert;
    std::function<void(container_type&, _container_it)> _container_remove = _container_traits::remove;


    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class graph;

    template <bool directed_v, vertex_descriptor_c vertex_t, graph_container_c container_t>
    friend class mutable_graph;
};

} // namespace gl
