<p align="center">
  <a href="https://github.com/SpectraL519/cpp-gl">
    <img src="docs/img/cpp-gl-hex.png" alt="CPP-GL" width="360" />
  </a>
</p>

<br />

<div align="center" markdown="1">

[![g++](https://github.com/SpectraL519/cpp-gl/actions/workflows/gpp.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/g++)
[![clang++](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang++)
[![format](https://github.com/SpectraL519/cpp-gl/actions/workflows/format.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/format)
[![benchmarks](https://github.com/SpectraL519/cpp-gl/actions/workflows/benchmarks.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/benchmarks.yaml)

</div>

<br />

## Overview

- The goal of the project was to create a higlhy customizable, intuitive and easy to work with graph library for the modern C++ standards.
- The `CPP-GL` library's implementation relies solely on the C++ standard library (it does not require installing any additional tools) and is designed to be compatible with the C++ standard tools, e.g. range-based loops, std algorithms, the [ranges library](https://en.cppreference.com/w/cpp/ranges), stream operations, etc.
- The library relies heavily on [concepts](https://en.cppreference.com/w/cpp/language/constraints) to acheive abstraction instead of interfaces and abstract classes to minimize the overhead associated with virtual tables and dynamic dispatch.

> [!NOTE]
> `v1.0.*` and `v1.0-prerelease-*` versions of the library have been developed as the Bachelor of Engineering project at the *Wrocław University of Science and Technology*
>
> Faculty: *W04N - Faculty of Information and Communication Technology*
>
> Field of study: *Algorithmic Computer Science*

<br />

## Table of content

- [Overview](#overview)
- [Table of content](#table-of-content)
- [Installing the library](#installing-the-library)
- [CMake integration](#cmake-integration)
- [Documentation - TODO](#documentation---todo)
- [Dev notes - TODO](#dev-notes---todo)
- [Compiler support](#compiler-support)
- [Licence](#licence)

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
    CXX_STANDARD 23 # or newer
    CXX_STANDARD_REQUIRED YES
)

# Link against the cpp-gl library
target_link_libraries(my_project PRIVATE cpp-gl)
```

<br />

## Documentation - TODO

<br />

## Dev notes - TODO

<br />

## Compiler support

| Compiler | Min version |
| :-: | :-: |
| GNU G++ | 14 |
| Clang | 18 |

> [!NOTE]
> Although currently the project has been properly verified using only the G++ and Clang compilers it should work fine with other compilers with C++23 support like MSVC.

<br />

## Licence

The `CPP-GL` project uses the [MIT Licence](https://mit-license.org/) which can be found in the [LICENCE](/LICENSE) file
