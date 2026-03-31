# CPP-GL Benchmarks

<!-- TODO: Full description -->

Building the benchmarks:

```shell
cmake -B build_bench -DBUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release $cmake_gcc15
```

Running the benchmarks:

```shell
./build_bench/benchmarks/gl_benchmarks --bip-v 9000 --benchmark_repetitions=10 --benchmark_display_aggregates_only=true
```
