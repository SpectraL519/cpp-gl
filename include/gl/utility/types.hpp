#ifndef CPP_GL_TYPES
#define CPP_GL_TYPES

#include <functional>



namespace gl {

// numerical type concepts
template <typename T>
concept index_t = std::unsigned_integral<T>;

template <typename T>
concept arithmetic_t = std::is_arithmetic_v<T>;


// container types
struct vect_s     {};
struct deq_s      {};
struct list_s     {};
struct flist_s    {};
struct set_s      {}; // TODO: add Compare param
struct multiset_s {}; // TODO: add Compare param
// struct hash_set_s {}; // TODO: add Hash, EqualTo params

} // namespace gl

#endif // CPP_GL_TYPES