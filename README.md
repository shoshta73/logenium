# Logenium

Element of logic

## Requirements

Build requirements:

- CMake 4.0 or newer
- Ninja
- Clang compiler (clang/clang++ with C23 and C++23 support)

optional:

- uv (for devutils)

### CMake

CMake is used generated build files for the project.

If you don't have CMake Installed you can download it from [cmake.org](https://cmake.org/download/).

### Ninja

Ninja is used to build the project.

If you don't have Ninja Installed you can download it from [ninja-build.org](https://ninja-build.org/).

### Clang Compiler

**This project requires Clang as the compiler.** Other compilers (GCC, MSVC, etc.) are not supported.

- **Required**: `clang` and `clang++` with C23 and C++23 support
- **Not allowed**: `clang-cl` (the MSVC-compatible Clang driver on Windows)

If you don't have Clang installed, download it from the LLVM project GitHub [releases](https://github.com/llvm/llvm-project/releases).

> [!IMPORTANT]
> On Windows, install the **GNU-compatible** Clang driver (clang/clang++), not clang-cl. The build system will reject clang-cl during configuration.

#### Setting the Compiler

Before running CMake, set the compiler environment variables:

```bash
export CC=clang CXX=clang++
```

Or specify compilers directly in the CMake command:

```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

## License

BSD 3-Clause License
Copyright (c) 2025, Logenium Authors and Contributors

For full license text, see the [LICENSE](LICENSE) file.
