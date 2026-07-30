<p align="center">
  <a href="https://github.com/SpectraL519/cpp-gl">
    <img src="docs/img/cpp-gl-hex.png" alt="CPP-GL" width="360" />
  </a>
</p>

<div align="center">

[![G++](https://github.com/SpectraL519/cpp-gl/actions/workflows/gpp.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/gpp.yaml)
[![Clang++](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang.yaml)

</div>

<div align="center">

[![Code Formatting](https://github.com/SpectraL519/cpp-gl/actions/workflows/format.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/format.yaml)
[![Benchmarks Smoke Test](https://github.com/SpectraL519/cpp-gl/actions/workflows/benchmarks.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/benchmarks.yaml)
[![changelog](https://img.shields.io/badge/changelog-blue.svg?logo=github)](https://github.com/SpectraL519/cpp-gl/releases)

</div>

<br />

## Documentation

The complete, versioned documentation, including detailed tutorials, quick start guides, and architectural overviews, is hosted online:

**[Explore the CPP-GL Documentation](https://spectral519.github.io/cpp-gl/latest/)**

<br/>

## Overview

CPP-GL is a highly customizable, intuitive, and concept-driven graph and hypergraph library designed for modern C++ standards.

Designed strictly around modern C++ paradigms, the library heavily leverages templates and concepts to deliver an API that is exceptionally fast, generic, and type-safe. It relies solely on the C++ standard library, meaning it requires no external dependencies and integrates perfectly with modern C++ tools such as range-based loops, the `<ranges>` library, standard algorithms, stream operations, and more.

<br/>

## Module Architecture: GL vs. HGL

To accommodate different mathematical models without compromising API clarity or performance, the library is strictly partitioned into two primary modules:

- **GL (Graph Library):** The core module dedicated to standard graphs. It handles both directed and undirected topologies where edges represent connections between exactly two vertices. It provides a comprehensive suite of utilities including structural generators, graph modifiers, and a broad spectrum of classical graph algorithms.
- **HGL (Hypergraph Library):** A specialized module dedicated to hypergraphs, where a single hyperedge represents a higher-order connection between an arbitrary number of vertices. It features generalized traversal algorithms and incidence-based memory models tailored for complex, multi-way relationships.

<br/>

## Core Features

Because both modules share the same fundamental design philosophy, they offer a unified, symmetrical feature set tailored to their respective mathematical domains:

- **Shared Zero-Cost Infrastructure:** By relying on C++20 concepts instead of virtual interfaces and dynamic dispatch, CPP-GL eliminates vtable overhead while enforcing strict compile-time contracts. Crucially, the **HGL module is built strictly on top of the GL module**, directly reusing its core infrastructure (ID types, traits, memory structures) to ensure maximum code reuse, while allowing the GL module to remain completely standalone for standard graph use cases.
- **Unified Topology APIs:** The `gl::graph` and `hgl::hypergraph` classes offer consistent interfaces that completely abstract away the underlying memory models. Users can seamlessly swap between different backend representations without rewriting traversal logic, property accesses, or algorithm calls.
- **Flexible Memory Layouts:** The library provides a comprehensive suite of memory-efficient representations. Users can choose between dynamic container-based layouts (Lists and Matrices) and highly optimized, cache-friendly contiguous array structures (Flat Lists and Flat Matrices). The HGL module further extends this by allowing users to specify the orientation of the memory models to optimize generalized incidence queries.
- **Customizable Element Properties:** Vertices, edges, and hyperedges can carry arbitrary, user-defined payloads. Whether assigning standard metric weights and colors or attaching complex, application-specific data structures, the library's traits system ensures that property injection and access remain strictly type-safe and performant.
- **Extensible Algorithmic Engines:** The library features a strictly decoupled algorithm design. Powerful, generic search templates serve as the underlying engines, allowing custom algorithms to be built on the fly by simply injecting zero-cost callbacks. Built upon these generic engines is a rich, out-of-the-box library of concrete algorithms—covering everything from classic pathfinding to advanced, multi-way reachability semantics for directed hypergraphs.
- **Robust I/O Facilities:** Native serialization and formatting tools make it simple to pipe topologies directly to and from standard C++ streams and files. Both the GL and HGL modules share a unified set of stream options for visual output, while relying on dedicated, easily parsable flat-text formats (GSF and HGSF) to ensure fast and predictable data serialization.

<br/>

## Compiler Support

| Compiler | Min version |
| :-: | :-: |
| GNU G++ | 14 |
| Clang | 18 |

> [!NOTE]
>
> Although currently the project has been properly verified using only the G++ and Clang compilers, it should work fine with other compilers with C++23 support like MSVC.

<br/>

## Installation & Integration

CPP-GL is a header-only template library. It can be integrated into a project either by directly including the headers or via CMake.

### Option A: CMake `FetchContent` (Recommended)

The easiest way to integrate CPP-GL is to fetch it directly from the GitHub repository during the CMake configuration phase:

```cmake
cmake_minimum_required(VERSION 3.14)
project(my_project LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    cpp-gl
    GIT_REPOSITORY [https://github.com/SpectraL519/cpp-gl.git](https://github.com/SpectraL519/cpp-gl.git)
    GIT_TAG <tag> # Specify the desired version tag, branch, or specific commit.
)

FetchContent_MakeAvailable(cpp-gl)

add_executable(my_project main.cpp)

set_target_properties(my_project PROPERTIES
    CXX_STANDARD 23 # The CPP-GL library requires C++23.
    CXX_STANDARD_REQUIRED YES
)

target_link_libraries(my_project PRIVATE cpp-gl)
```

### Option B: Including the Headers Directly

If CMake is not being used, simply download the desired version of the library from the [Releases Page](https://github.com/SpectraL519/cpp-gl/releases) and add the `include` directory of the library to the project via the `-I<cpp-gl-dir>/include` flag.

<br/>

## Benchmarks

Performance and zero-cost abstraction are primary goals of this library. A comprehensive evaluation suite built on top of **Google Benchmark** provides a structured way to measure, filter, and report the library's performance across various graph algorithms and memory layouts.

For detailed performance metrics, configuration instructions (including BGL integration), and CLI usage, please refer to the **[Benchmarks Guide](benchmarks/README.md)**.

<br/>

## Developer Notes

To build the testing and documentation suites locally, follow the guides below.

### Running Tests

The library features a comprehensive test suite powered by **Doctest** and orchestrated via CMake. The test executables for both the `gl` and `hgl` modules can be built and run as follows:

```bash
# Configure the project with tests enabled
cmake -B build -DBUILD_TESTS=ON

# Build the test executables (replace <n> with the core count)
cmake --build build/ -j<n>

# Execute the tests
./build/tests/gl
./build/tests/hgl
```

### Code Formatting

The project enforces strict code formatting using `clang-format-18`. A custom Python script is provided to streamline the formatting process across the repository.

To format the entire repository:

```bash
python scripts/format.py -exe clang-format-18
```

To format only the files modified in the last commit:

```bash
python scripts/format.py -m -exe clang-format-18
```

> [!NOTE]
>
> If `clang-format-18` is your system's default, you can omit the `-exe` flag.

### Building the Documentation

The documentation build process utilizes the following toolchain:

- **[Doxygen](https://www.doxygen.nl/)**: Extracts the initial C++ API structure and documentation into XML format.
- **Concept Parser**: A [custom Python script](/docs/scripts/gen_concept_docs.py) processes the XML to generate dedicated concept documentation pages.
- **[MkDoxy (Custom Fork)](https://github.com/SpectraL519/MkDoxy)**: Injects the Doxygen XML data directly into the MkDocs build lifecycle.
- **[MkDocs](https://www.mkdocs.org/) & [Material Theme](https://squidfunk.github.io/mkdocs-material/)**: Renders the final, searchable static HTML website.
- **[mike](https://github.com/jimporter/mike)**: Manages versioning, allowing multiple versions of the documentation to coexist and be deployed simultaneously.

To build and serve the documentation locally:

```bash
make serve-docs # automatically builds and serves the documentation page using the `dev` tag
```

To clean the documentation build directories:
```bash
make clean-docs
```

> [!TIP]
>
> If your local `gh-branches` is out of sync with the `origin/gh-pages` branch, you can run the following command to force sync it to match the state of the origin branch:
> ```bash
> make sync-gh-pages
> ```

> [!NOTE]
>
> The documentation build scripts rely on **[uv](https://docs.astral.sh/uv/)** to simplify Python dependency management within the project. Ensure `uv` is installed on the system before attempting to build the documentation.

<br/>

## License

The `CPP-GL` project uses the [MIT License](https://mit-license.org/) which can be found in the [LICENSE](LICENSE.md) file.
