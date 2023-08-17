#ifndef CPP_GL_VERTEX
#define CPP_GL_VERTEX

#include <memory>

#include <gl/utility.hpp>
#include <gl/edge/edge_descriptor.hpp>



namespace gl {

template <
    index_t key_t,
    key_type_edge_descriptor_t<key_t> edge_t,
    graph_container_t container_s,
    detail::satisfies_or_void<detail::equality_comparable_s> data_t
>
struct vertex_descriptor;



namespace vertex {

template <typename descriptor_t>
struct is_valid_descriptor : std::false_type {};

template <
    index_t key_t,
    key_type_edge_descriptor_t<key_t> edge_t,
    graph_container_t container_s,
    detail::satisfies_or_void<detail::equality_comparable_s> data_t
>
struct is_valid_descriptor <vertex_descriptor <key_t, edge_t, container_s, data_t>> : std::true_type {};

template <typename descriptor_t>
inline constexpr bool is_valid_descriptor_v = is_valid_descriptor<descriptor_t>::value;

} // namespace vertex

template <typename descriptor_t>
concept vertex_descriptor_t = vertex::is_valid_descriptor_v<descriptor_t>;

template <typename descriptor_t>
concept data_vertex_descriptor_t = vertex_descriptor_t<descriptor_t> && is_data_descriptor_v<descriptor_t>;





template <
    index_t key_t = std::size_t,
    key_type_edge_descriptor_t<key_t> edge_t = edge_descriptor<key_t>,
    graph_container_t adj_container_s = gl::vector,
    detail::satisfies_or_void<detail::equality_comparable_s> data_t = void
>
struct vertex_descriptor {
public:
    using key_type = key_t;
    using edge_type = edge_t;
    using edge_ptr = std::unique_ptr<edge_type>;
    using container_type = container_traits_t<adj_container_s, edge_ptr>;
    using data_type = data_t;

    const key_type key;


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

    vertex_descriptor(const vertex_descriptor<key_type, edge_type, adj_container_s, data_type>& other)
        : key(other.key), _adjacent(other._adjacent), _data(other._data)
    {}

    vertex_descriptor(vertex_descriptor<key_type, edge_type, adj_container_s, data_type>&& other)
        : key(other.key), _adjacent(other._adjacent), _data(other._data)
    {}

    ~vertex_descriptor() = default;


    friend bool operator==(const vertex_descriptor<key_type, edge_type, adj_container_s, data_type>& lhs,
                           const vertex_descriptor<key_type, edge_type, adj_container_s, data_type>& rhs) {
        return lhs.key == rhs.key &&
               lhs._adjacent == rhs._adjacent &&
               lhs._data == rhs._data;
    }


    [[nodiscard]] inline const container_type& adjacent() {
        return this->_adjacent;
    }

    [[nodiscard]] inline std::size_t in_deg() const {
        return this->_in_deg;
    }

    [[nodiscard]] inline std::size_t out_deg() const {
        return this->_adjacent.size();
    }


    [[nodiscard]] inline std::size_t degree() const {
        return this->in_deg() + this->out_deg();
    }

    void add_edge(key_type destination) {
        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination));
    }

    void add_edge(key_type destination, const data_type& data) {
        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination, data));
    }

    void add_edge(edge_type&& edge) {
        if (edge.source != this->key)
            return;

        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(std::move(edge)));
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


private:
    container_type _adjacent;
    std::size_t _in_deg = 0;

    data_type _data = data_type();

    std::function<void(container_type&, edge_ptr&&)> _container_insert =
        container_traits<adj_container_s, edge_ptr>::insert;

    template <bool DIRECTED, vertex_descriptor_t vertex_t, graph_container_t container_s>
    friend class graph; // friend graph class forward declaration
};



template <
    index_t key_t,
    key_type_edge_descriptor_t<key_t> edge_t,
    graph_container_t adj_container_s
>
struct vertex_descriptor <key_t, edge_t, adj_container_s, void> {
public:
    using key_type = key_t;
    using edge_type = edge_t;
    using edge_ptr = std::unique_ptr<edge_type>;
    using container_type = container_traits_t<adj_container_s, edge_ptr>;
    using data_type = void;

    const key_type key;


    explicit vertex_descriptor(const key_type& key) : key(key) {}

    explicit vertex_descriptor(const key_type& key, const container_type& adjacent_)
        : key(key), _adjacent(adjacent_)
    {}

    template <typename data_t = void>
    vertex_descriptor(const vertex_descriptor<key_t, edge_t, adj_container_s, data_t>& other)
        : key(other.key), _adjacent(other._adjacent)
    {}

    template <typename data_t = void>
    vertex_descriptor(vertex_descriptor<key_t, edge_t, adj_container_s, data_t>&& other)
        : key(other.key), _adjacent(other._adjacent)
    {}

    ~vertex_descriptor() = default;


    friend bool operator==(const vertex_descriptor<key_type, edge_type, adj_container_s, data_type>& lhs,
                           const vertex_descriptor<key_type, edge_type, adj_container_s, data_type>& rhs) {
        return lhs.key == rhs.key &&
               lhs._adjacent == rhs._adjacent;
    }


    [[nodiscard]] const container_type& adjacent() {
        return this->_adjacent;
    }

    [[nodiscard]] inline std::size_t in_deg() const {
        return this->_in_deg;
    }

    [[nodiscard]] inline std::size_t out_deg() const {
        return this->_adjacent.size();
    }

    [[nodiscard]] std::size_t degree() const {
        return this->in_deg() + this->out_deg();
    }

    void add_edge(key_type destination) {
        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(this->key, destination));
    }

    void add_edge (edge_type&& edge) {
        if (edge.source != this->key)
            return;

        this->_container_insert(this->_adjacent, std::make_unique<edge_type>(std::move(edge)));
    }


private:
    container_type _adjacent;
    std::size_t _in_deg = 0;

    std::function<void(container_type&, edge_ptr&&)> _container_insert =
        container_traits<adj_container_s, edge_ptr>::insert;

    template <bool DIRECTED, vertex_descriptor_t vertex_t, graph_container_t container_s>
    friend class graph;
};

} // namespace gl

#endif // CPP_GL_VERTEX
