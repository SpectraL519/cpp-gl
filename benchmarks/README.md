# CPP-GL Benchmarks

## Overview

This directory contains the performance evaluation suite for the CPP-GL library. Built on top of **Google Benchmark** and driven by a custom CLI, these benchmarks provide a structured way to measure, filter, and report the library's performance across various graph algorithms and data structures.

**Key Features:**
* **Modular Suites:** Benchmarks are organized into independent suites targeting specific functionalities (e.g., `is-bipartite`).
* **Comparative Baselines:** Optional integration with the Boost Graph Library (BGL) to compare CPP-GL's performance against industry standards.
* **Ergonomic CLI:** A custom interface for easy suite selection, combined with the raw filtering power of Google Benchmark.

<br/>
<br/>

## Building the Benchmarks

To configure and build the benchmark executable, use the following standard CMake commands from the project root:

```shell
cmake -B build_bench -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build_bench # -j<n>
```

### Enabling Boost Graph Library (BGL) Comparisons

If you want to compile the comparative BGL benchmarks, you must have Boost installed on your system. Enable them by passing the `-DBENCH_INCLUDE_BGL=ON` flag during configuration:

```shell
cmake -B build_bench -DBUILD_BENCHMARKS=ON -DBENCH_INCLUDE_BGL=ON -DCMAKE_BUILD_TYPE=Release
```

> [!NOTE]
>
> BGL comparative benchmarks are only defined for specific benchmark suites where a direct equivalent exists in Boost.

<br/>
<br/>

## Running the Benchmarks

Once built, the benchmark executable provides a rich command-line interface.

### Basic Usage

To run all registered benchmark suites and print the results to your terminal:

```shell
./build_bench/benchmarks/gl_benchmarks
```

### Global Options

The benchmark runner supports several custom global flags that modify its behavior. To view all available options, use the `--help` flag:

```shell
./build_bench/benchmarks/gl_benchmarks --help
```

> [!NOTE]
> To view Google Benchmark's native options, use the `--gbench-help` flag instead.

#### Advanced Filtering

Because the runner passes unhandled arguments directly to Google Benchmark, you can combine our custom suite selector with Google's native regex filter (--benchmark_filter) to drill down into specific data structures.

**Example 1: Run a specific suite**

```shell
./build_bench/benchmarks/gl_benchmarks --suites is-bipartite
```

***Example 2: Test a specific data structure across ALL suites***

```shell
./build_bench/benchmarks/gl_benchmarks --benchmark_filter="flat_matrix"
```

***Example 3: A combination of both***

```shell
./build_bench/benchmarks/gl_benchmarks --suites is-bipartite --benchmark_filter="flat_matrix"
```
