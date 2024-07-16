# CPP-GL

<!-- TODO: workflow badges -->
<!-- TODO: master branch ruleset: require status checks to pass -->

<br />

## Overview
`CPP-GL` is a general purpose header-only template graph library for C++20 and newer standards.

<br />

## Table of contents
* [Tutorial](#tutorial)
    * [Including CPP-GL into a project](#including-cpp-gl-into-a-project)
        * [CMake integration](#cmake-integration)
        * [Downloading the library](#downloading-the-library)
    * ...
* Examples
* [Dev notes](#dev-notes)
    * [Building and testing](#building-and-testing)
    * [Formatting](#formatting)
* Compiler support
* [Licence]()

<br />
<br />

## Tutorial

### Including CPP-GL into a project

There are 2 main ways to include the CPP-GL library into a C++ project:

#### CMake integration

For CMake projects you can simply fetch the library in your `CMakeLists.txt` file:
```cmake
cmake_minimum_required(VERSION 3.12)

project(my_project LANGUAGES CXX)

# Include FetchContent module
include(FetchContent)

# Fetch CPP-GL library
FetchContent_Declare(
    cpp-gl
    GIT_REPOSITORY https://github.com/SpectraL519/cpp-gl.git
    GIT_TAG master # here you can specify the desired tag or branch
)

FetchContent_MakeAvailable(cpp-gl)

# Define the executable for the project
add_executable(my_project main.cpp)

set_target_properties(my_project PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
)

# Link against the cpp-gl library
target_link_libraries(my_project PRIVATE cpp-gl)
```

#### Downloading the library

If you do not use CMake you can dowload the desired [library release](https://github.com/SpectraL519/cpp-gl/releases), extract it in a desired directory and simply add the `<cpp-gl-dir>/include` to the list of include directories of your project.

<br />
<br />

## Dev notes

### Building and testing

TODO

### Formatting

> [!NOTE]
> The project uses `clang-format-18`. Installation instructions can be found on the [llvm apt](https://apt.llvm.org/) page.

To format the code run the following:
```shell
# Linux
./scripts/format/linux.sh
```
```shell
# Windows: powershell
./scripts/format/windows.ps1
```
To run a format check use the above-mentioned scripts with a `--check` flag.

<br />
<br />

## Licence

The `CPP-GL` project uses the [MIT Licence](https://opensource.org/license/mit)
