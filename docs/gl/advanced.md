# Advanced Features

The CPP-GL library is designed to be highly configurable, allowing developers to tune the library's internal behavior to meet strict performance and architectural requirements.

This page outlines the advanced configuration toggles and features available in the library.

---

## API Validation Policy (Strict vs. Relaxed)

By default, every CPP-GL graph operates under the [**gl::api::strict_t**](../cpp-gl/structgl_1_1api_1_1strict__t.md) policy. This means that every time you query a neighbor, retrieve an edge property, or remove a vertex, the library safely checks if the provided ID is within the bounds of the active topology. If it isn't, the library throws a `std::invalid_argument` exception.

While this guarantees safety and predictable failure, these branches and bounds-checks incur a small overhead. In highly complex algorithms traversing millions of edges, these checks can compound.

### The Relaxed Contract

To achieve absolute zero-cost abstraction, you can instantiate your graph with the [**gl::api::relaxed_t**](../cpp-gl/structgl_1_1api_1_1relaxed__t.md) tag:

```cpp
using fast_traits = gl::list_graph_traits<
    gl::directed_t, gl::empty_properties, gl::empty_properties, gl::api::relaxed_t
>;
gl::graph<fast_traits> fast_graph;
```

When using `relaxed_t`, the library completely compiles away all internal throw statements and bounds checks.

> [!WARNING] The Danger of Relaxed Mode
>
> By opting into relaxed_t, you enter a strict contract with the compiler: You guarantee that your inputs are always valid.
>
> If you pass an invalid vertex ID or a dangling edge descriptor to a relaxed graph, the library will not catch it. It will blindly read or erase from unmapped memory, resulting in Undefined Behavior (UB), data corruption, or a segmentation fault.

### The at() Exception

Mirroring standard library conventions (like `std::vector::operator[]` vs `std::vector::at()`), the `graph.at(vertex_id)` method is the sole exception to the API policy. It will always perform a strict bounds-check and throw an exception, even if the graph is instantiated with `relaxed_t`.

### When to use `relaxed_t`

You should switch to `relaxed_t` only when:

1. You are running heavily localized algorithms (like custom BFS/DFS/A* loops) where you are exclusively querying IDs that the graph itself just provided to you.

2. You have profiled your application and identified that branch-prediction failures or validation overhead in the graph queries is a demonstrable bottleneck.

3. Your graph topology is fully constructed and immutable during the algorithm's execution (guaranteeing that no descriptors are invalidated mid-traversal).

---

## Forced Function Inlining

By default, C++ compilers use their own internal heuristics to decide whether a function marked with the `inline` keyword should actually be inlined. While modern compilers are generally excellent at this, relying purely on default cost models can sometimes leave small, heavily utilized accessors or structural traversal utilities un-inlined.

To guarantee zero-overhead abstraction in the most performance-critical code paths, CPP-GL provides an opt-in macro to forcibly inline a carefully selected set of core library methods.

### Enabling Forced Inlining

You can enable this feature by defining the `GL_CONFIG_FORCE_INLINE` macro. For consistent behavior across translation units, it is highly recommended to define this globally through your build system rather than in individual source files.

**Via CMake (Recommended):**

```cmake
target_compile_definitions(my_application PRIVATE GL_CONFIG_FORCE_INLINE)
```

**Via Code:**

If you must define it in code, ensure it is defined before including any CPP-GL headers.

```cpp
#define GL_CONFIG_FORCE_INLINE
#include <gl/graph.hpp>
```

### How it Works

When `GL_CONFIG_FORCE_INLINE` is active, the library applies compiler-specific attributes, such as `[[clang::always_inline]]` for Clang or `[[gnu::always_inline]]` for GCC, to critical internal functions. This bypasses the compiler's standard inlining heuristics. If the macro is left undefined, the library safely falls back to using the standard C++ inline keyword.

> [!WARNING] Performance Trade-offs
>
> Forcing the compiler to inline functions is a double-edged sword. While it eliminates function call overhead and can expose more optimization opportunities (like constant propagation), it can also lead to significant code bloat.
>
> Excessively large binaries can cause instruction cache (i-cache) misses, which may ultimately degrade performance rather than improve it. Always profile and benchmark your application with and without `GL_CONFIG_FORCE_INLINE` to ensure it provides a tangible benefit for your specific workload.
