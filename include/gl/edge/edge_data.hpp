#pragma once

#include <stdexcept>

#include "gl/utility.hpp"



namespace gl::edge {

template <arithmetic_t T = bool>
struct weight_data {
public:
    using weight_type = T;

    weight_data() = default;
    ~weight_data() = default;

    weight_data(weight_type weight)       : _weight(weight)        {}
    weight_data(const weight_data& other) : _weight(other._weight) {}
    weight_data(weight_data&& other)      : _weight(other._weight) {}


    inline weight_type weight() const {
        return this->_weight;
    }

    virtual inline void set_weight(weight_type weight) {
        this->_weight = weight;
    }

    virtual bool operator==(const weight_data& other) {
        return this->_weight == other._weight;
    }

protected:
    weight_type _weight = 1;
};



template <arithmetic_t T = bool>
struct flow_data {
public:
    using flow_type = T;

    flow_data() = default;
    ~flow_data() = default;

    explicit flow_data(flow_type capacity) : _capacity(capacity) {}

    explicit flow_data(flow_type flow, flow_type capacity)
        : _flow(flow), _capacity(capacity)
    {}

    flow_data(const flow_data& other)
        : _flow(other._flow), _capacity(other._capacity)
    {}

    flow_data(flow_data&& other)
        : _flow(other._flow), _capacity(other._capacity)
    {}


    inline flow_type flow() const {
        return this->_flow;
    }

    inline flow_type capacity() const {
        return this->_capacity;
    }

    virtual void set_flow(flow_type flow) {
        if (flow > this->_capacity)
            throw std::invalid_argument(
                "overflow: new flow (" + std::to_string(flow)
                + ") > capacity (" + std::to_string(this->_capacity) + ")"
            );

        this->_flow = flow;
    }

    virtual inline void set_capacity(flow_type capacity) {
        if (capacity < this->_flow)
            throw std::invalid_argument(
                "overflow: new capacity (" + std::to_string(capacity)
                + ") < flow (" + std::to_string(this->_flow) + ")"
            );

        this->_capacity = capacity;
    }

    virtual bool operator==(const flow_data& other) {
        return this->_flow == other._flow && this->_capacity == other._capacity;
    }

protected:
    flow_type _flow = 0;
    flow_type _capacity = 1;
};

} // namespace gl::edge
