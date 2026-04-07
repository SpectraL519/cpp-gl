# CPP-GL Benchmarks

## Overview

- The benchmarks in this directory are designed to evaluate the performance of the CPP-GL library.
- The benchmarks are implemented using the Google Benchmark framework, which provides a structured way to measure and report the performance of the library.
- The benchmarks are organized into different suites, each targeting specific functionalities of the CPP-GL library.
- Some of the benchmarks may require additional dependencies, such as the Boost Graph Library, to be installed on your system (if configured).

<br/>

## Building and Running the Benchmarks

To build the benchmarks, you can use the following commands:

```shell
cmake -B build_bench -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build_bench/ # -j<n>
```

> [!INFO]
> In order to include the Boost Graph Library benchmarks, you need to have Boost installed on your system and ensure that CMake can find it.
> Moreover, you will need to use the `-DBENCH_INCLUDE_BGL=ON` flag when configuring the build to enable the BGL benchmarks.
> **NOTE:** The BGL benchmarks are defined only for some of the benchmark suites.

After building the benchmarks, you can run them using the following command:

```shell
./build_bench/benchmarks/gl_benchmarks
```

> [!TIP]
> You can
