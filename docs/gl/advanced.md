# Advanced Features

The CPP-GL library is designed to be highly configurable, allowing developers to tune the library's internal behavior to meet strict performance and architectural requirements.

This page outlines the advanced configuration toggles and features available in the library.

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
