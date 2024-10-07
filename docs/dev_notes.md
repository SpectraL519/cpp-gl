# Dev notes

Here you can find the necessery information to be able to work on the project

<br />

## Building and testing

> [!NOTE]
> The project uses [doctest](https://github.com/doctest/doctest) framework for unit testing, however it is already installed in the [tests/external](/tests/external/) directory, so there is no need to install it sepparately.

### Build the testing executable

```shell
cmake -B build
cd build
make # -j <n>
```

This will build the test executable `run` in the `<project-root>/build/tests` directory.

### Run the tests

```shell
cd build
./tests/run # -ts=<test-suite-name>
```

> [!NOTE]
> Test suites in the project have the same names as the files they're in except for the `test_extarnal_libs_config.cpp` file which defines the `test_doctest_config` test suite.

<br />

## Formatting

> [!NOTE]
> The project uses `clang-format-18` which is natively available on the `Ubuntu-24.04 LTS` system.
>
> If the `llvm-18` toolchain isn't available on your system, the installation instructions can be found on the [llvm apt](https://apt.llvm.org/) page. After installing the toolchain run `sudo apt install clang-format-18`.

You can format the code manually using `clang-format-18` or you can use the prepared python script:

```shell
python scripts/format.py
```

To inspect the all script's options and usage run:

```shell
python scripts/format.py --help
```

> [!NOTE]
> The script requires `python >= 3.9`
