# CPP-GL

General purpose header-only template graph library for C++20 and newer standards.

[![g++](https://github.com/SpectraL519/cpp-gl/actions/workflows/gpp.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/g++)
[![clang++](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang++)
[![format](https://github.com/SpectraL519/cpp-gl/actions/workflows/format.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/format)

<br />

## Overview

* The goal of the project was to create a higlhy customizable, intuitive and easy to work with graph library for the modern C++ standards.
* The `CPP-GL` library's implementation relies solely on the C++ standard library (it does not require installing any additional tools) and is designed to be compatible with the C++ standard tools, e.g. range-based loops, std algorithms, the [ranges library](https://en.cppreference.com/w/cpp/ranges), stream operations, etc.
* The library relies heavily on [concepts](https://en.cppreference.com/w/cpp/language/constraints) to acheive abstraction instead of interfaces and abstract classes to minimize the overhead associated with virtual tables and dynamic dispatch.

> [!NOTE]
> `v1.0` of the library has been developed as the Bachelor of Engineering Thesis at the *Wrocław University of Science and Technology*
>
> Faculty: *W04N - Faculty of Information and Communication Technology*
>
> Field of study: *Algorithmic Computer Science*

<br />

## Table of contents

* [Installing the library](#installing-the-library)
* [CMake integration](#cmake-integration)
* [Documentation](#documentation)
* Examples
* [Dev notes](#dev-notes)
* [Compiler support](#compiler-support)
* [Licence](#licence)

<br />

## Installing the library

The use the `CPP-GL` library in your project you can download the source code from the [releases](https://github.com/SpectraL519/cpp-gl/releases) page and add the `<cpp-gl-root>/include` path to the include directories of your project.

<br />

## CMake integration

The `CPP-GL` library can also be included in a `CMake` project by adding the following to the `CMakeLists.txt` file of your project:

```cmake
cmake_minimum_required(VERSION 3.12)

project(my_project LANGUAGES CXX)

# Include FetchContent module
include(FetchContent)

# Fetch the CPP-GL library
FetchContent_Declare(
    cpp-gl
    GIT_REPOSITORY https://github.com/SpectraL519/cpp-gl.git
    GIT_TAG <tag> # here you can specify the desired tag or branch
)

FetchContent_MakeAvailable(cpp-gl)

# Define the executable for the project
add_executable(my_project main.cpp)

set_target_properties(my_project PROPERTIES
    CXX_STANDARD 20 # or newer
    CXX_STANDARD_REQUIRED YES
)

# Link against the cpp-gl library
target_link_libraries(my_project PRIVATE cpp-gl)
```

<br />

## Documentation

> [!NOTE]
> The `CPP-GL` library does not use any dedicated documentation tools in the initial release, however it is planned to add such a tool in a future release

### Quick start

The core of the library is the template [graph class](/docs/graph.md) which holds the graph's [vertices](/docs/vertex.md) and [edges](/docs/edge.md) and defines methods allowing for the manipulation of the graph's structure.

```c++
#include <gl/graph.hpp>

#include <iostream>
#include <format>

int main() {
    // initialize the graph with 5 vertices
    gl::graph<> graph(5);

    // add some edges
    graph.add_edge(0, 1);
    graph.add_edge(1, 4);
    graph.add_edge(4, 2);
    graph.add_edge(4, 3);

    // print the size of the graph
    std::cout << std::format("number of vertices: {}\n", graph.n_vertices())
              << std::format("number of edges: {}\n", graph.n_unique_edges());
}
```

The [graph class](/docs/graph.md) can be easily customized to suit your needs by defining the directional and implementation types of the graph as well as the [vertex](/docs/graph_elements.md) and [edge](/docs/graph_elements.md) properties types.

While the `gl::graph` class is the key element of the library, it's not the only one. The `CPP-GL` library provides a wide range of functionalities designed to handle various graph-related operations efficiently. For detailed explanations and usage examples of these features, please refer to the dedicated documentation pages linked below.

* [The graph class - library's core](/docs/graph.md)
* [The vertex and edge classes - representation of the graph's elements](/docs/graph_elements.md)
* [IO operations](/docs/io.md)
* [Graph topology generators](/docs/topologies.md)
* [Algorithms](/docs/algoithms.md)

<br />

## Dev notes

The instructions and requirements of working on the `CPP-GL` project can be found [here](/docs/dev_notes.md).

<br />

## Compiler support

| Compiler | Min version |
| :-: | :-: |
| GNU G++ | 13 |
| Clang | 17 |

> [!NOTE]
> Although currently the project has been properly verified using only the G++ and Clang compilers it should work fine with other compilers with C++20 support like MSVC.

<br />

## Licence

The `CPP-GL` project uses the [MIT Licence](https://mit-license.org/) which can be found in the [LICENCE](/LICENSE) file
