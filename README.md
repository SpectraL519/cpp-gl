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

<!--

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

> [!NOTE]
> The project uses `Doctest` and `FakeIt` frameworks for unit testing, however they are already installed in the `tests/external` directory so there is no need to download them manually.

Build the testing executable:
    ```shell
    cmake -B build
    cd build && make
    ```

> [!TIP]
> To speed up the build process you can run: `make -j <number-of-jobs>`

This will build the test executable `run` in the `<project-root>/build/tests` directory.

> [!TIP]
> Building on Windows -  use the `-G "Unix Makefiles"` option when running CMake to build a GNU Make project instead of a default Visual Studio project.

Run the tests:

* All tests:

    ```shell
    cd build
    ./tests/run
    ```

* A single test suite:

    ```shell
    cd build
    ./tests/run -ts="<test-suite-name>"
    ```

> [!NOTE]
> Test suites in the project have the same names as the files they're in except for the `test_config` file which defines two test suites: `test_doctest_config` and `test_fakeit_config`.
test_fakeit_config

<br />

### Formatting

> [!NOTE]
> The project uses `clang-format-18`. The `llvm-18` toolchain installation instructions can be found on the [llvm apt](https://apt.llvm.org/) page. After installing the toolchain run `sudo apt install clang-format-18`.

You can format the code manually using `clang-format-18` or you can use the prepared python script:

To format the code run the following:
```shell
python scripts/format.py (--check)
```

The script allows formatting all files modified since the last pushed commit with the `-m` or `--modified-files` flag.

The remaining options are: search paths, file patterns, exclude paths.

To inspect the all script's options and usage run:
```shell
python scripts/format.py --help
```

> [!NOTE]
> The script requires `python >= 3.9`

<br />
<br />

## Licence

The `CPP-GL` project uses the [MIT Licence](https://opensource.org/license/mit)

-->
