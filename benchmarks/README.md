# CPP-GL Benchmarks

Building the benchmarks:

```shell
cmake -B build_bench -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build_bench/ # -j<n>
```

> [!NOTE]
> The benchmarks require having the Boost Graph Library installed.
> To install the library you can use

Running the benchmarks (example):

```shell
./build_bench/benchmarks/gl_benchmarks --bip-v 9000 --benchmark_repetitions=10 --benchmark_display_aggregates_only=true
```
