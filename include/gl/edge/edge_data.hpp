#ifndef CPP_GL_EDGE_DATA
#define CPP_GL_EDGE_DATA

#include <gl/utility.hpp>



namespace gl::edge {

template<arithmetic_t weight_t = bool>
struct weight_data {
public:
    using weight_type = weight_t;

protected:
    weight_type _weight = 1;

public:
    weight_data() = default;
    ~weight_data() = default;

    weight_data(const weight_type& weight) : _weight(weight)        {}
    weight_data(const weight_data& other)  : _weight(other._weight) {}
    weight_data(weight_data&& other)       : _weight(other._weight) {}

    inline weight_type& weight() const;
    virtual inline void set_weight(const weight_type& weight);

    virtual bool operator==(const weight_data& other);
};



template<arithmetic_t flow_t = bool>
struct flow_data {
public:
    using flow_type = flow_t;

protected:
    flow_type _flow = 0;
    flow_type _capacity = 1;

public:
    flow_data() = default;
    ~flow_data() = default;

    explicit flow_data(const flow_type& capacity) : _capacity(capacity) {}

    explicit flow_data(const flow_type& flow, const flow_type& capacity)
        : _flow(flow), _capacity(capacity)
    {}

    flow_data(const flow_data& other)
        : _flow(other._flow), _capacity(other._capacity)
    {}

    flow_data(flow_data&& other)
        : _flow(other._flow), _capacity(other._capacity)
    {}

    inline flow_type& flow() const;
    inline flow_type& capacity() const;

    virtual inline void set_flow(const flow_type& weight);
    virtual inline void set_capacity(const flow_type& capacity);

    virtual bool operator==(const flow_data& other);
};

} // namespace gl::edge



// weight_data member functions
template<gl::arithmetic_t weight_t>
inline gl::edge::weight_data<weight_t>::weight_type& gl::edge::weight_data<weight_t>::weight() const {
    return const_cast<weight_type&>(this->_weight);
}

template<gl::arithmetic_t weight_t>
inline void gl::edge::weight_data<weight_t>::set_weight(const weight_type& weight) {
    this->_weight = weight;
}

template<gl::arithmetic_t weight_t>
bool gl::edge::weight_data<weight_t>::operator==(const weight_data& other) {
    return this->_weight == other._weight;
}



// flow_data member_functions
template<gl::arithmetic_t flow_t>
inline gl::edge::flow_data<flow_t>::flow_type& gl::edge::flow_data<flow_t>::flow() const {
    return const_cast<flow_type&>(this->_flow);
}

template<gl::arithmetic_t flow_t>
inline gl::edge::flow_data<flow_t>::flow_type& gl::edge::flow_data<flow_t>::capacity() const {
    return const_cast<flow_type&>(this->_capacity);
}

template<gl::arithmetic_t flow_t>
inline void gl::edge::flow_data<flow_t>::set_flow(const flow_type& flow) {
    this->_flow = flow;
}

template<gl::arithmetic_t flow_t>
inline void gl::edge::flow_data<flow_t>::set_capacity(const flow_type& capacity) {
    this->_capacity = capacity;
}

template<gl::arithmetic_t flow_t>
bool gl::edge::flow_data<flow_t>::operator==(const flow_data& other) {
    return this->_flow == other._flow && this->_capacity == other._capacity;
}

#endif // CPP_GL_EDGE_DATA
