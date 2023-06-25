#ifndef CPP_GL_EDGE_TYPES
#define CPP_GL_EDGE_TYPES

#include <utility/types.hpp>



namespace gl::edge {

// struct default_s {};
// struct weighted_s {};
// struct flow_s {};
// struct weighted_flow_s {};

// /*
template <typename data_t = void> struct default_s        {};
template <>                       struct default_s <void> {};

template <numerical_t weight_t, 
          typename data_t = void> struct weighted_s                  {};
template <numerical_t weight_t>   struct weighted_s <weight_t, void> {};

template <numerical_t flow_t, 
          typename data_t = void> struct flow_s                {};
template <numerical_t flow_t>     struct flow_s <flow_t, void> {};

template <numerical_t weight_t, 
          numerical_t flow_t, 
          typename data_t = void> struct weighted_flow_s                          {};
template <numerical_t weight_t, 
          numerical_t flow_t>     struct weighted_flow_s <weight_t, flow_t, void> {};
// */

} // namespace gl::edge

#endif // CPP_GL_EDGE_TYPES