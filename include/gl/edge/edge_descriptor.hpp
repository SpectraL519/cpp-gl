#ifndef CPP_GL_EDGE_DESCRIPTOR
#define CPP_GL_EDGE_DESCRIPTOR

#include <type_traits>

#include <gl/utility.hpp>



namespace gl {

template <
    typename vertex_key_t,
    satisfies_or_void<std::is_arithmetic> weight_t = void,
    satisfies_or_void<std::is_arithmetic> flow_t = void,
    typename data_t = void
>
struct edge_descriptor {
public:
    // type definitions
    using vertex_key_type = vertex_key_t;
    using data_type = data_t;
    using weight_type = std::conditional_t<std::is_void_v<weight_t>, bool, weight_t>;
    using flow_type = std::conditional_t<std::is_void_v<flow_t>, bool, flow_t>;

    // attributes
    const vertex_key_type source;
    const vertex_key_type destination;

private:
    // attributes
    weight_type _weight = true;
    flow_type _flow = false;
    flow_type _capacity = false;

    data_type _data = data_type();

public:
    // constructors & destructors
    edge_descriptor() = default;

    explicit edge_descriptor (
        const vertex_key_type& source, const vertex_key_type& destination,
        const data_type data = data_type()
    ) : 
        source(source), destination(destination), 
        _data(data) 
    {}

    template <typename W = weight_t> requires (!std::is_void_v<W>)
    explicit edge_descriptor (
        const vertex_key_type& source, const vertex_key_type& destination,
        const weight_type& weight,
        const data_type data = data_type()
    ) : 
        source(source), destination(destination), 
        _weight(weight),
        _data(data) 
    {}

    template <typename F = flow_t> requires (!std::is_void_v<F>)
    explicit edge_descriptor (
        const vertex_key_type& source, const vertex_key_type& destination,
        const flow_type& flow, const flow_type& capacity, 
        const data_type data = data_type()
    ) :
        source(source), destination(destination), 
        _flow(flow), _capacity(capacity),
        _data(data) 
    {}

    template <typename W = weight_t, typename F = flow_t> 
    requires (!std::is_void_v<W> && !std::is_void_v<F>)
    explicit edge_descriptor (
        const vertex_key_type& source, const vertex_key_type& destination,
        const weight_type& weight, const flow_type& flow, const flow_type& capacity, 
        const data_type data = data_type()
    ) :
        source(source), destination(destination), 
        _weight(weight), _flow(flow), _capacity(capacity),
        _data(data) 
    {}

    edge_descriptor (const edge_descriptor<vertex_key_t, weight_t, flow_t, data_t>& other) 
        : source(other.source), destination(other.destination),
          _weight(other._weight), _flow(other._flow), _capacity(other._capacity),
          _data(other._data)
    {}

    edge_descriptor (edge_descriptor<vertex_key_t, weight_t, flow_t, data_t>&& other) 
        : source(other.source), destination(other.destination),
          _weight(other._weight), _flow(other._flow), _capacity(other._capacity),
          _data(other._data)
    {}

    ~edge_descriptor() = default;

    // getters & setters
    [[nodiscard]] inline weight_type weight () const {
        return this->_weight;
    }

    template <typename W = weight_t> requires (!std::is_void_v<W>)
    inline void set_weight (weight_type weight) {
        this->_weight = weight;
    }

    [[nodiscard]] inline flow_type flow () const {
        return this->_flow;
    }

    template <typename F = flow_t> requires (!std::is_void_v<F>)
    inline void set_flow (flow_type flow_) {
        this->_flow = flow;
    }

    [[nodiscard]] inline flow_type capacity () const {
        return this->_capacity;
    }

    template <typename F = flow_t> requires (!std::is_void_v<F>)
    inline void set_capacity (flow_type capacity_) {
        this->_capacity = capacity;
    }

    template <typename D = data_t> requires (!std::is_void_v<D>)
    [[nodiscard]] inline data_type& data () const {
        return this->_data;
    }

    template <typename D = data_t> requires (!std::is_void_v<D>)
    inline void set_data (data_type data) {
        this->_data = data;
    }

    edge_descriptor<vertex_key_t, weight_t, flow_t, data_t> reverse () {
        edge_descriptor<vertex_key_t, weight_t, flow_t, data_t> reverse_edge(this->destination, this->source, this->_data);
        reverse_edge._weight = this->_weight;
        reverse_edge._flow = this->_flow;
        reverse_edge._capacity = this->_capacity;
        return std::move(reverse_edge);
    }
};



template <typename vertex_key_t, typename weight_t, typename flow_t>
struct edge_descriptor <vertex_key_t, weight_t, flow_t, void> {
public:
    // type definitions
    using vertex_key_type = vertex_key_t;
    using data_type = void;
    using weight_type = std::conditional_t<std::is_void_v<weight_t>, bool, weight_t>;
    using flow_type = std::conditional_t<std::is_void_v<flow_t>, bool, flow_t>;

    // attributes
    const vertex_key_type source;
    const vertex_key_type destination;

private:
    // attributes
    weight_type _weight = true;
    flow_type _flow = false;
    flow_type _capacity = false;

// constructors & destructors
public:
    edge_descriptor() = default;

    explicit edge_descriptor (const vertex_key_type& source, const vertex_key_type& destination)
        : source(source), destination(destination)
    {}

    template <typename W = weight_t> requires (!std::is_void_v<W>)
    explicit edge_descriptor (
        const vertex_key_type& source, const vertex_key_type& destination,
        const weight_type& weight
    ) : 
        source(source), destination(destination), 
        _weight(weight)
    {}

    template <typename F = flow_t> requires (!std::is_void_v<F>)
    explicit edge_descriptor (
        const vertex_key_type& source, const vertex_key_type& destination,
        const flow_type& flow, const flow_type& capacity
    ) :
        source(source), destination(destination), 
        _flow(flow), _capacity(capacity)
    {}

    template <typename W = weight_t, typename F = flow_t> 
    requires (!std::is_void_v<W> && !std::is_void_v<F>)
    explicit edge_descriptor (
        const vertex_key_type& source, const vertex_key_type& destination,
        const weight_type& weight, const flow_type& flow, const flow_type& capacity
    ) :
        source(source), destination(destination), 
        _weight(weight), _flow(flow), _capacity(capacity)
    {}

    template <typename data_t = void>
    edge_descriptor (const edge_descriptor<vertex_key_t, weight_t, flow_t, data_t>& other) 
        : source(other.source), destination(other.destination),
          _weight(other._weight), _flow(other._flow), _capacity(other._capacity)
    {}

    template <typename data_t = void>
    edge_descriptor (edge_descriptor<vertex_key_t, weight_t, flow_t, data_t>&& other) 
        : source(other.source), destination(other.destination),
          _weight(other._weight), _flow(other._flow), _capacity(other._capacity)
    {}

    ~edge_descriptor() = default;

    // getters & setters
    [[nodiscard]] inline weight_type weight () const {
        return this->_weight;
    }

    template <typename W = weight_t> requires (!std::is_void_v<W>)
    inline void set_weight (weight_type weight) {
        this->_weight = weight;
    }

    [[nodiscard]] inline flow_type flow () const {
        return this->_flow;
    }

    template <typename F = flow_t> requires (!std::is_void_v<F>)
    inline void set_flow (flow_type flow_) {
        this->_flow = flow;
    }

    [[nodiscard]] inline flow_type capacity () const {
        return this->_capacity;
    }

    template <typename F = flow_t> requires (!std::is_void_v<F>)
    inline void set_capacity (flow_type capacity_) {
        this->_capacity = capacity;
    }

    edge_descriptor<vertex_key_t, weight_t, flow_t, void> reverse () {
        edge_descriptor<vertex_key_t, weight_t, flow_t, void> reverse_edge(this->destination, this->source);
        reverse_edge._weight = this->_weight;
        reverse_edge._flow = this->_flow;
        reverse_edge._capacity = this->_capacity;
        return std::move(reverse_edge);
    }
};

} // namespace gl

#endif // CPP_GL_EDGE_DESCRIPTOR