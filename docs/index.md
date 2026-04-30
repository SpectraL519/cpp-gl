<style>
  .md-typeset h1 { display: none !important; }
</style>

<div align="center" markdown="1">
  <img src="img/cpp-gl-hex.png" alt="CPP-GL" width="360" />
</div>

<br />

<div align="center" markdown="1">

[![G++](https://github.com/SpectraL519/cpp-gl/actions/workflows/gpp.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/gpp.yaml)
[![Clang++](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang.yaml)


</div>

<div align="center" markdown="1">

[![Code Formatting](https://github.com/SpectraL519/cpp-gl/actions/workflows/format.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/format.yaml)
[![Benchmarks Smoke Test](https://github.com/SpectraL519/cpp-gl/actions/workflows/benchmarks.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/benchmarks.yaml)
[![changelog](https://img.shields.io/badge/changelog-blue.svg?logo=github)](https://github.com/SpectraL519/cpp-gl/releases)

</div>

<br />

## Overview

CPP-GL is a highly customizable, intuitive, and concept-driven graph and hypergraph library designed for modern C++ standards.

Designed strictly around modern C++ paradigms, the library heavily leverages templates and concepts to deliver an API that is exceptionally fast, generic, and type-safe. It relies solely on the C++ standard library, meaning it requires no external dependencies and integrates perfectly with modern C++ tools such as range-based loops, the `<ranges>` library, standard algorithms, stream operations and more.

---

## Module Architecture: GL vs. HGL

To accommodate different mathematical models without compromising API clarity or performance, the library is strictly partitioned into two primary modules:

- **GL (Graph Library):** The core module dedicated to standard graphs. It handles both directed and undirected topologies where edges represent connections between exactly two vertices. It provides a comprehensive suite of utilities including structural generators, graph modifiers, and a broad spectrum of classical graph algorithms.
- **HGL (Hypergraph Library):** A specialized module dedicated to hypergraphs, where a single hyperedge represents a higher-order connection between an arbitrary number of vertices. It features generalized traversal algorithms and incidence-based memory models tailored for complex, multi-way relationships.

---

## Core Features

Because both modules share the same fundamental design philosophy, they offer a unified, symmetrical feature set tailored to their respective mathematical domains:

- **Shared Zero-Cost Infrastructure:** By relying on C++20 concepts instead of virtual interfaces and dynamic dispatch, CPP-GL eliminates vtable overhead while enforcing strict compile-time contracts. Crucially, the **HGL module is built strictly on top of the GL module**, directly reusing its core infrastructure (ID types, traits, memory structures) to ensure maximum code reuse, while allowing the GL module to remain completely standalone for standard graph use cases.
- **Unified Topology APIs:** The `gl::graph` and `hgl::hypergraph` classes offer consistent interfaces that completely abstract away the underlying memory models. Users can seamlessly swap between different backend representations without rewriting their traversal logic, property accesses, or algorithm calls.
- **Flexible Memory Layouts:** The library provides a comprehensive suite of memory-efficient representations. Users can choose between dynamic container-based layouts (Lists and Matrices) and highly optimized, cache-friendly contiguous array structures (Flat Lists and Flat Matrices). The HGL module further extends this by allowing users to specify the orientation of the memory models to optimize generalized incidence queries.
- **Customizable Element Properties:** Vertices, edges, and hyperedges can carry arbitrary, user-defined payloads. Whether you are assigning standard metric weights and colors or attaching complex, application-specific data structures, the library's traits system ensures that property injection and access remain strictly type-safe and performant.
- **Dual-Layered Algorithmic Architecture:** CPP-GL cleanly separates traversal mechanics from specific problem-solving logic. At the foundation, highly generic search engines (like BFS and DFS) empower users to construct completely custom algorithms via injected callbacks. Layered seamlessly above this is a comprehensive suite of ready-to-use concrete algorithms, ranging from standard graph pathfinding to specialized hypergraph reachability (such as B-Searches and F-Searches).
- **Robust I/O Facilities:** Native serialization and formatting tools make it simple to pipe topologies directly to and from standard C++ streams and files. Both the GL and HGL modules share a unified set of stream options for visual output, while relying on dedicated, easily parsable flat-text formats (GSF and HGSF) to ensure fast and predictable data serialization.

---

## Compiler support

| Compiler | Min version |
| :-: | :-: |
| GNU G++ | 14 |
| Clang | 18 |

> [!NOTE] C++23 Support
>
> Although currently the project has been properly verified using only the G++ and Clang compilers it should work fine with other compilers with C++23 support like MSVC.

---

## Installation & Integration

CPP-GL is a header-only template library. You can integrate it into your project either by directly including the headers or via CMake.

### Option A: CMake `FetchContent` (Recommended)

The easiest way to integrate CPP-GL is to fetch it directly from the GitHub repository during your CMake configuration phase:

```cmake
cmake_minimum_required(VERSION 3.14)
project(my_project LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    cpp-gl
    GIT_REPOSITORY https://github.com/SpectraL519/cpp-gl.git
    GIT_TAG <tag> # (1)!
)

FetchContent_MakeAvailable(cpp-gl)

add_executable(my_project main.cpp)

set_target_properties(my_project PROPERTIES
    CXX_STANDARD 23 # (2)!
    CXX_STANDARD_REQUIRED YES
)

target_link_libraries(my_project PRIVATE cpp-gl)
```

1. Specify the desired version tag, branch, or specific commit
2. The CPP-GL library requires C++23

### Option B: Including the Headers Directly

If you do not wish to use CMake, you can simply download the desired version of the library from the [Releases Page](https://github.com/SpectraL519/cpp-gl/releases) and add the `include` directory of the library to your project via the `-I<cpp-gl-dir>/include` flag.

---

## Next Steps

Ready to write some code? Choose a module below to view its Quick Start guide and dive into the tutorials:

* [**Get Started with GL (Standard Graphs)**](gl/quick_start.md) - Master the core concepts, build custom topologies, and explore the robust suite of generic traversal engines and classical algorithms.
* [**Get Started with HGL (Hypergraphs)**](hgl/quick_start.md) - Master generalized incidence, configure complex memory layouts, and evaluate higher-order reachability across hypergraph networks.
