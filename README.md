# CPP-GL

General purpose header-only template graph library for C++20 and newer standards.

[![g++](https://github.com/SpectraL519/cpp-gl/actions/workflows/gpp.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/g++)
[![clang++](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/clang++)
[![format](https://github.com/SpectraL519/cpp-gl/actions/workflows/format.yaml/badge.svg)](https://github.com/SpectraL519/cpp-gl/actions/workflows/format)

<br />

## Overview

* The goal of the project was to create a higlhy customizable, intuitive and easy to work with graph library for the modern C++ standards.
* The `CPP-GL` library's implementation relies solely on the C++ standard library (it does not require installing any additional tools) and is designed to be compatible with the C++ standard tools, e.g. range-based loops, the `std::ranges` library, iostream operations, etc.
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
* Documentation
* Examples
* Dev notes
* [Compiler support](#compiler-support)
* Licence

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
