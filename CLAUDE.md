# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Logenium is a Logisim-like logic circuit simulator in early development. Built with modern C++ (C++23) and targeting Windows with infrastructure for cross-platform support, the project currently focuses on establishing the GUI foundation. The project uses a layered architecture with platform abstraction through the xheader library.

**Current Status**: The application can create a window and run a basic Windows message loop. Event handling and custom window procedures are not yet implemented.

## Compiler Requirements

This project **requires Clang as the compiler** for both C and C++:
- **C compiler**: `clang`
- **C++ compiler**: `clang++`

**Important**: On Windows, `clang-cl` (MSVC-compatible Clang driver) is **not allowed**. You must use the GNU-compatible Clang driver.

### Setting Compilers

**Method 1: Environment variables** (recommended):
```bash
export CC=clang CXX=clang++
cmake -B build -G Ninja
```

**Method 2: CMake command line**:
```bash
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

The build system will automatically verify that Clang is being used and fail configuration if:
- The C compiler is not Clang
- The C++ compiler is not Clang
- `clang-cl` is detected on Windows

## Build Commands

### Standard Build
```bash
export CC=clang CXX=clang++
cmake -B build -G Ninja
cmake --build build
```

### Run Application
```bash
./build/logenium  # On Windows/WSL
# Creates a window and runs the message loop until window is closed
```

### Build with Tests
```bash
export CC=clang CXX=clang++
cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build  # Run all tests
```

### Clean Build
```bash
rm -rf build
export CC=clang CXX=clang++
cmake -B build -G Ninja
cmake --build build
```

### Using devutils (Alternative)
The project includes `devutils`, a Python-based development utility tool for interactive configuration and building. Uses **uv** for dependency management (no manual Python installation required).

#### Windows Users (Easiest)
Use the provided wrapper scripts which handle all setup automatically:

**Batch script** (cmd.exe):
```cmd
# Run from repository root - automatically installs uv if needed
devutils.bat configure
devutils.bat build
devutils.bat clean
devutils.bat --version
```

**PowerShell script**:
```powershell
# Run from repository root - automatically installs uv if needed
.\devutils.ps1 configure
.\devutils.ps1 build
.\devutils.ps1 clean
.\devutils.ps1 --version
```

Both scripts automatically:
- Detect and install uv if not present
- Install Python 3.14 if needed
- Create and manage virtual environment
- Install dependencies from requirements.txt
- Run devutils with your arguments

**First run may take a few minutes for setup. Subsequent runs are instant.**

#### Linux/macOS Users (Easiest)
Use the provided wrapper script which handles all setup automatically:

**Bash script**:
```bash
# Run from repository root - automatically installs uv if needed
./devutils.sh configure
./devutils.sh build
./devutils.sh clean
./devutils.sh --version
```

The script automatically:
- Detect and install uv if not present (to ~/.local/bin)
- Install Python 3.14 if needed
- Create and manage virtual environment
- Install dependencies from requirements.txt
- Run devutils with your arguments

**First run may take a few minutes for setup. Subsequent runs are instant.**

#### Manual Usage
```bash
# Run devutils commands (uv auto-manages Python 3.14+)
uv run devutils configure
# Prompts for: testing options, library-specific testing, build mode

uv run devutils build
# Options: -v/--verbose for verbose output, -j/--jobs for parallel jobs

uv run devutils clean
# Remove the build directory and all its contents

uv run devutils check-license-headers check  # or: cls check
# Check all files for correct SPDX license headers

uv run devutils python stubgen generate
# Generate Python stub files (.pyi) inline with source code

uv run devutils python stubgen check
# Check if Python stub files are up to date

uv run devutils --version
```

**Note**: devutils is optional. Standard CMake commands work identically. On all platforms, the wrapper scripts (devutils.bat, devutils.ps1, devutils.sh) provide the easiest experience. For manual usage, requires [uv](https://github.com/astral-sh/uv) to be installed manually.

## devutils Package

### Overview
`logenium-devutils` is a Python package providing interactive CLI tools for configuring and building the Logenium project. Built with Typer, it offers a user-friendly alternative to manual CMake commands. Uses **uv** for zero-config dependency management.

### Structure
```
devutils/
├── pyproject.toml              # Package configuration with uv support
├── src/devutils/
│   ├── __init__.py            # Package version
│   ├── __main__.py            # CLI entry point with Typer app
│   ├── commands/
│   │   ├── __init__.py        # Command exports
│   │   ├── configure.py       # Interactive CMake configuration
│   │   ├── build.py           # Ninja build with progress output
│   │   ├── clean.py           # Build directory cleanup
│   │   ├── check_license_headers.py  # License header validation and fixing
│   │   ├── format.py          # Code formatting with clang-format and ruff
│   │   ├── lint.py            # Code linting with clang-tidy, mypy, and ruff
│   │   └── python/            # Python-specific commands
│   │       ├── __init__.py    # Python command group
│   │       └── stubgen.py     # Stub file generation with mypy stubgen
│   ├── constants/
│   │   ├── __init__.py        # Constant exports
│   │   ├── paths.py           # Project path definitions
│   │   ├── comments.py        # Comment syntax for different languages
│   │   ├── extensions.py      # File extension mappings
│   │   └── license_header.py  # License header templates
│   └── utils/
│       ├── __init__.py        # Utility exports
│       ├── filesystem.py      # File system utilities
│       └── file_checking.py   # Shared file checking utilities
```

### Commands

#### configure
Interactive CMake project configuration with prompts for build options:
- **Testing**: Enable/disable project-wide tests (`LOGENIUM_BUILD_TESTS`)
- **Library Testing**: Enable/disable per-library tests (`LOGENIUM_XHEADER_BUILD_TESTS`, `LOGENIUM_DEBUG_BUILD_TESTS`)
- **Build Mode**: Debug, Release, RelWithDebInfo, or MinSizeRel
- **Generator**: Always uses Ninja

Usage: `uv run devutils configure`

#### build
Runs Ninja build with enhanced output:
- `-v/--verbose`: Detailed build output
- `-j/--jobs N`: Parallel jobs (0 = auto, capped to CPU count)
- `--no-ninja-override`: Preserve existing `NINJA_STATUS` environment variable
- Custom status format: `[finished/total : percentage - elapsed_time]` (e.g., `[42/238 : 17% - 0d 0h 0m 5s 234ms]`)

Usage: `uv run devutils build [-v] [-j N] [--no-ninja-override]`

#### clean
Removes build directory with proper read-only file handling (essential for FetchContent git objects on Windows).

Usage: `uv run devutils clean`

#### check-license-headers (alias: cls)
Validates and fixes SPDX license headers. Supports C/C++, Python, CMake, PowerShell, Batch.

**Subcommands**: `show-headers`, `check`, `fix`

**File Coverage**: C/C++ in `src/`/`libs/`, CMake files, Python in `devutils/`, Windows scripts

**Header Format**: `// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors` + `// SPDX-License-Identifier: BSD-3-Clause` (syntax varies by language)

Usage: `uv run devutils cls check` | `uv run devutils cls fix`

**Architecture**: Configuration-driven via `LanguageConfig` dataclass. To add languages, update `get_language_configs()` in `check_license_headers.py` and constants files.

#### format
Checks and fixes code formatting. Supports C/C++ (clang-format) and Python (ruff).

**Subcommands**: `check`, `fix`

**File Coverage**: C/C++ in `src/`/`libs/`, Python in `devutils/`

**Tools**: clang-format (must be in PATH), ruff (auto-installed via uv)

Usage: `uv run devutils format check` | `uv run devutils format fix`

**Architecture**: Configuration-driven via `LanguageConfig`. Shares utilities with `check-license-headers` in `devutils.utils.file_checking`.

#### lint
Multi-step linting with **parallel processing** and **YAML-based caching**. Supports C/C++ (clang-tidy + clang-check) and Python (mypy + ruff).

**Subcommands**: `check`, `fix` | **Flag**: `--no-cache`

**File Coverage**: C/C++ in `src/`/`libs/`, Python in `devutils/`

**Tools**:
- C/C++: clang-tidy (standards, can fix) + clang-check (static analysis, read-only)
- Python: mypy (type checking, read-only) + ruff (linting, can fix)
- Requires: clang-tidy/clang-check in PATH, `build/compile_commands.json`; mypy/ruff auto-installed via uv

**Performance**:
- Parallel: 3-5x speedup (first run)
- Cache (`.cache/devutils/lint_cache.yaml`, mtime-based): 10-50x speedup (no changes), 5-10x (partial changes)

**Output Tags**: `[OK]`, `[CACHED:OK]`, `[WARNING]`, `[HAS_ISSUES]`, `[FIXED]`, `[PARTIAL]`, `[ERROR]`
- Warnings don't cause failure; errors do

Usage: `uv run devutils lint check [--no-cache]` | `uv run devutils lint fix [--no-cache]`

**Architecture**: Multi-step parallel processing via `ThreadPoolExecutor`, thread-safe YAML cache (`LintCacheManager`), per-file per-tool cache entries. Cache key: `"{language}:{tool}:{relative_path}"`. To add lint steps, modify `lint_steps` in `get_language_configs()`. Dependencies: `pyyaml>=6.0.3`, `types-pyyaml>=6.0.12.20250915`.

#### python stubgen
Generates/checks Python stub files (`.pyi`) inline with source code using mypy's stubgen.

**Subcommands**: `generate [-v]`, `check [-v]`

**Process**: Generate to temp (`stubgen -p devutils`) → Copy `.pyi` alongside `.py` files → 23 stubs for devutils package

**Regenerate when**: Adding/modifying functions, classes, type annotations, or modules

Usage: `uv run devutils python stubgen generate` | `uv run devutils python stubgen check`

### devutils Constants & Utilities

**Singletons** (frozen dataclasses in `devutils.constants`):
- `Directories`: Project paths (`root`, `build`, `libs`, library-specific paths)
- `Files`: Build file paths (`ninja_build_file`)
- `Comments`: Language comment syntax (`//`, `#`, `@REM`)
- `Extensions`: File extension lists (`.c/.h`, `.cxx/.hxx`, `.py/.pyi`, etc.)
- `LicenseHeaders`: SPDX header templates for each language

**Utilities** (`devutils.utils`):
- `fs`: File system utilities (`find_files_by_extensions`, `get_files_recursively`)
- `file_checking`: Shared file processing utilities (5 modules: `file_result`, `file_status`, `language_config`, `statistics`, `utils`)
  - Key classes: `FileResult`, `FileStatus`, `LanguageConfig`, `Statistics`
  - Key functions: `collect_files`, `format_file_path`, `print_status`

### Development

**Dependencies**: Typer 0.20.1+, Python 3.14+ (auto-managed by uv)

**Running with uv** (no installation needed):
```bash
uv run devutils <command>
```

**Code Quality Tools** (configured in `pyproject.toml`):
- **Ruff**: Linting and formatting (120 char line length)
  - Enabled rules: pycodestyle, pyflakes, isort, flake8-bugbear, comprehensions, pyupgrade
- **Mypy**: Strict type checking with Python 3.14 target

**Entry Points**: Two CLI entry points (`logenium-devutils`, `devutils`) map to `devutils.__main__:app`

### devutils Development Notes

**Common Pitfalls**:
1. Capture subprocess error output in `FileResult`: `error_output = result.stdout + result.stderr`
2. Display error messages from `FileResult.error` in `check_files()`/`fix_files()`
3. Clear `__pycache__` after changes: `find devutils -name "__pycache__" -type d -exec rm -rf {} +`

**Why uv?**: Auto-manages Python versions, fast (Rust-based), zero config, deterministic

**Wrapper Scripts**: Platform-specific wrapper scripts auto-install uv, Python 3.14, create venv, install dependencies, and forward commands:
- **Windows**: `devutils.bat` (cmd.exe) and `devutils.ps1` (PowerShell)
- **Unix-like**: `devutils.sh` (bash for Linux/macOS)

First run: ~few minutes; subsequent: instant.

## Architecture Overview

### Library Architecture

1. **xheader library** (C23): Cross-platform API abstraction
   - Location: `libs/xheader/`
   - Provides platform-specific APIs and stub implementations for unsupported platforms
   - **Windows API** (`xheader/windows.h`):
     - Each Windows API function has corresponding `.c` files with A/W suffix variants
     - Provides stub implementations for non-Windows platforms
   - **Linux dlfcn API** (`xheader/dlfcn.h`):
     - Re-exports system `<dlfcn.h>` on Linux
     - Provides stub implementations for non-Linux platforms
     - Functions: `dlopen`, `dlclose`
     - Links against `dl` library on Linux
   - **Important**: Library is C23, but tests are C++ (GoogleTest requires C++)
   - CMake declares `LANGUAGES C CXX` to support both library and tests

2. **debug library** (C++23): Debug utilities and assertions
   - Location: `libs/debug/`
   - Provides debugging helpers that are no-ops in release builds
   - Depends on xheader for platform-specific debugging features
   - Header: `libs/debug/include/debug.hxx`

3. **logenium application** (C++23): GUI framework
   - Uses abstract base classes with platform-specific implementations
   - Factory pattern via `Application::Create()`
   - Singleton pattern for application instance
   - Main entry point: `main()` creates application and calls `Run()` method

### Key Class Hierarchy

```
Application (abstract)
└── WindowsApplication
    └── Creates WindowsWindow

Window (abstract)
└── WindowsWindow
```

### NativeHandle Pattern

Both `Application` and `Window` classes use a `NativeHandle` nested struct that:
- Wraps platform-specific handles (HMODULE, HWND)
- Provides implicit conversions to/from void* and platform types
- Ensures type safety while maintaining flexibility

### debug Library

The debug library provides debugging utilities that are completely eliminated in release builds:

**Core Functions**:
- `debug::Assert<Args...>`: Template-based assertion with formatted output
  - Takes a predicate and format string with arguments
  - On failure: prints formatted message, source location, and stacktrace, then triggers breakpoint
  - Uses CTAD (Class Template Argument Deduction)
  - Example: `debug::Assert{condition, "Expected {}, got {}", expected, actual}`

- `debug::Breakpoint()`: Unconditional debugger breakpoint
  - Triggers `__debugbreak()` on Windows (via xheader)
  - No-op on other platforms

- `debug::BreakpointIfDebugging()`: Conditional breakpoint
  - Only triggers if debugger is attached
  - Completely eliminated at compile-time in release builds

- `debug::IsDebuggerPresent()`: Debugger detection
  - Returns `true` if debugger is attached (Windows only)
  - Returns `false` on non-Windows platforms

**Build Configuration**:
- Debug builds (`#ifndef NDEBUG`): All functions are active
- Release builds (`#ifdef NDEBUG`): All functions become `consteval` no-ops
  - Zero runtime overhead
  - Eliminates all debug code at compile-time

**Modern C++ Features Used**:
- `std::format` and `std::format_string` for type-safe formatting
- `std::source_location::current()` for automatic location capture
- `std::stacktrace::current()` for full call stack on assertion failure
- `std::print` and `std::println` for output
- CTAD for simplified template instantiation

**Namespace Convention**:
- Primary namespace: `debug::`
- When `__LOGENIUM_SOURCE__` is defined: symbols available in `logenium::` via `using namespace`

## Important Implementation Details

### xheader Library

**Windows API Convention**: Windows API functions follow ANSI (A) / Wide (W) naming:
- `*A` functions use `LPCSTR`, `WNDCLASSEXA` (narrow char)
- `*W` functions use `LPCWSTR`, `WNDCLASSEXW` (wide char)
- Implementation files MUST match header declarations exactly
- Stub implementations wrapped in `#ifndef _WIN32` blocks

**Linux dlfcn API Convention**:
- Re-exports system header on Linux: `#include <dlfcn.h> // IWYU pragma: export`
- Provides stub implementations on non-Linux platforms
- Platform detection: `#ifdef __linux__` for header re-export, `#ifndef __linux__` for stubs
- Stub behavior:
  - `dlopen(const char *__file, int __mode)`: Returns `NULL`
  - `dlclose(void *__handle)`: Returns `0`
- CMake linkage: Links against `dl` library on Linux via `$<$<PLATFORM_ID:Linux>:dl>`

**C23 Constexpr Limitation**: In C23, constexpr pointers can only be null. Non-null pointer constants (like `IDI_APPLICATION`, `IDC_ARROW`) must use `#define` macros, not `constexpr`.

### Application Initialization and Lifecycle

1. **Initialization** (`main()` → `Application::Create()`):
   - Factory returns `WindowsApplication` on Windows, nullptr otherwise
   - `WindowsApplication` constructor:
     - Gets module handle via `GetModuleHandle(nullptr)`
     - Calls `RegisterWindowClass()`
     - Creates `WindowsWindow` instance
     - Sets `state.is_running = true`
   - `WindowsWindow` constructor:
     - Calls `CreateWindowEx()` with `WS_OVERLAPPEDWINDOW` style
     - Shows window with `ShowWindow(SW_SHOW)`
     - Updates with `UpdateWindow()`

2. **Main Loop** (`app->Run()`):
   - Windows message loop using `GetMessage()`, `TranslateMessage()`, `DispatchMessage()`
   - Continues while `state.is_running` is true
   - Exits on `WM_QUIT` message (GetMessage returns 0) or error (returns -1)

3. **Cleanup** (`~WindowsApplication()`):
   - Sets `state.is_running = false`
   - Destroys window via `window.reset()`
   - Unregisters window class via `UnregisterClass()`
   - Clears module handle

### Application State Management

The `Application::State` struct tracks runtime state:
```cpp
struct State {
    bool is_running{false};
};
```

Accessible via `GetState()` method. Used to control the main loop execution.

### Window Class Configuration

Window class name is `"LogeniumWindowClass"` with:
- Style: `CS_HREDRAW | CS_VREDRAW`
- Window procedure: `DefWindowProc`
- Background: `COLOR_WINDOW + 1`
- Icon/Cursor: Standard application resources

## Code Style and Conventions

### File Naming
- Headers: `.hxx` for C++, `.h` for C
- Source: `.cxx` for C++, `.c` for C
- Platform-specific files in `platform/windows/` subdirectory

### Include Guards
```cpp
#ifndef LOGENIUM_PATH_TO_FILE_HXX
#define LOGENIUM_PATH_TO_FILE_HXX
// ...
#endif  // LOGENIUM_PATH_TO_FILE_HXX
```

### Platform Conditionals
```cpp
#if defined(_WIN32)
    // Windows-specific code
#endif
```

### Documentation Comments
**DO NOT** add documentation comments (docstrings, Javadoc-style comments, brief descriptions, etc.) or short inline code comments unless explicitly requested by the user. The code should be self-documenting through clear naming and structure. Only add comments where the logic is truly non-obvious and cannot be clarified through better code organization.

### Include Order (.clang-format)
The project uses clang-format with specific include ordering priorities:
1. **Priority 7**: Project headers (`logenium/`)
2. **Priority 6**: Debug library (`debug/`)
3. **Priority 5**: xheader library (`xheader/`)
4. **Priority 4**: Third-party headers (`gtest/`, `gmock/`)
5. **Priority 3**: System headers (`Windows.h`)
6. **Priority 2**: Standard library headers (`<vector>`, `<string>`, etc.)
7. **Priority 1**: Other headers

Additional clang-format settings:
- Column limit: 120
- Based on Google style
- 4-space indentation
- Pointer/reference alignment: right (`int *ptr`, `Type &ref`)
- Empty lines around namespace bodies

### CMake Logging
Use custom logging functions from `cmake/logging.cmake`:
- `log_info()`, `log_status()`, `log_warning()`, `log_error()`

### CMake Module Includes
The project uses two patterns for including CMake modules:

1. **Module name only** (searches CMAKE_MODULE_PATH):
   ```cmake
   include(options)
   include(dependencies)
   ```
   Used at root level where `CMAKE_MODULE_PATH` includes `cmake/`

2. **Full path** (explicit file location):
   ```cmake
   include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake")
   include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")
   ```
   Used in subdirectories for explicit control over which file is included

## Git Commit Message Conventions

### Commit Message Tagging

The project uses a tag-based commit message convention to categorize changes by component:

**Tagged Commits** (component-specific changes):
- `[xheader]` - Changes to the xheader library
- `[debug]` - Changes to the debug library
- `[devutils]` - Changes to the devutils package
- `[build]` - Changes to the build system (CMake files)
- `[logenium]` - General logenium application changes
- `[logenium(application)]` - Specific application component changes (with sub-component in parentheses)

**Untagged Commits** (repository-wide changes):
- No tag prefix indicates changes that affect the entire repository
- Examples: license headers, README updates, editorconfig, repository-wide documentation

### Message Format

1. **Tag placement**: Tags appear at the beginning in square brackets `[tag]`
2. **Sub-components**: Use parentheses for sub-components: `[logenium(application)]`
3. **Message style**:
   - Use lowercase after the tag
   - Use imperative mood (e.g., "add", "fix", "update")
   - Keep messages concise and single-line
   - No period at the end
4. **Examples**:
   - `[xheader] add dlfcn.h stubs and cross platform implementation`
   - `[debug] fix builds on linux`
   - `[devutils] add parallel processing to the lint command`
   - `[logenium(application)] add linux specific application skeleton`
   - `fix license headers` (no tag = repo-wide)

## Windows API Functions in xheader

### Message Loop Functions
The following Windows API functions are implemented for message processing:

- **GetMessageA/W**: Retrieves messages from the message queue
  - Return values: -1 (error), 0 (WM_QUIT), >0 (normal message)
  - Macro: `GetMessage` maps to A or W variant based on UNICODE

- **TranslateMessage**: Translates virtual-key messages into character messages
  - Takes `const MSG*` parameter
  - No A/W variants (operates on MSG structure)

- **DispatchMessageA/W**: Dispatches message to window procedure
  - Returns `LRESULT` from window procedure
  - Macro: `DispatchMessage` maps to A or W variant based on UNICODE

### All Implemented Functions
Complete list of xheader Windows API stubs (see `libs/xheader/CMakeLists.txt` for current list):
- Window creation: `CreateWindowEx`, `DestroyWindow`
- Window management: `ShowWindow`, `UpdateWindow`
- Window class: `RegisterClassEx`, `UnregisterClass`, `DefWindowProc`
- Message loop: `GetMessage`, `TranslateMessage`, `DispatchMessage`, `PostQuitMessage`
- Module/resource: `GetModuleHandle`, `LoadIcon`, `LoadCursor`
- Window data: `GetWindowLongPtr`, `SetWindowLongPtr`
- Debugging: `IsDebuggerPresent`, `__debugbreak`

## Linux dlfcn API Functions in xheader

The xheader library provides Linux dynamic linking functions via `<xheader/dlfcn.h>`:

### Dynamic Linking Functions

- **dlopen**: Opens a dynamic library
  - Signature: `void *dlopen(const char *__file, int __mode)`
  - On Linux: System implementation from `<dlfcn.h>`
  - On non-Linux: Stub returns `NULL`
  - Usage: `dlopen(nullptr, RTLD_NOW)` for self-loading

- **dlclose**: Closes a dynamic library handle
  - Signature: `int dlclose(void *__handle)`
  - On Linux: System implementation from `<dlfcn.h>`
  - On non-Linux: Stub returns `0` (success)

### All Implemented Functions
Complete list of xheader dlfcn stubs (see `libs/xheader/CMakeLists.txt` for current list):
- Dynamic linking: `dlopen`, `dlclose`

**Note**: Additional dlfcn functions (`dlsym`, `dlerror`) are not yet implemented.

## Adding New Windows API Functions

1. Add function declaration to `libs/xheader/include/xheader/windows.h`
2. Create stub implementation files:
   - `libs/xheader/src/windows/function_name_a.c` (ANSI variant)
   - `libs/xheader/src/windows/function_name_w.c` (Wide variant)
   - For non-variant functions: `function_name.c`
3. Add to `libs/xheader/CMakeLists.txt` sources
4. Create corresponding test files in `libs/xheader/tests/windows/`
5. Add UNICODE macros if needed:
   ```c
   #ifdef UNICODE
   #define FunctionName FunctionNameW
   #else
   #define FunctionName FunctionNameA
   #endif
   ```

## Adding New Linux dlfcn Functions

1. Add function declaration to `libs/xheader/include/xheader/dlfcn.h`
2. Create stub implementation file:
   - `libs/xheader/src/dlfcn/function_name.c`
3. Add to `libs/xheader/CMakeLists.txt` sources
4. Create corresponding test file in `libs/xheader/tests/dlfcn/`
5. Ensure proper platform detection:
   - Header: Use `#ifdef __linux__` to re-export system header
   - Implementation: Use `#ifndef __linux__` to wrap stub implementation
   - Example:
     ```c
     // In header
     #ifdef __linux__
     #include <dlfcn.h>  // IWYU pragma: export
     #else
     void *dlsym(void *__handle, const char *__name);
     #endif

     // In implementation
     #ifndef __linux__
     void *dlsym(void *__handle, const char *__name) { return NULL; }
     #endif
     ```

## Adding Type Definitions to xheader

### Basic Types
Use standard Windows type conventions matching the Windows SDK.

### Handle Types
Define opaque struct with dummy member, then typedef pointer:
```c
struct HFOO__ {
    int dummy;
};
typedef struct HFOO__ *HFOO;
```

### Constants
- Numeric constants: Use `constexpr` when possible
- Non-null pointer constants: Use `#define` macros (C23 limitation)
- Follow Windows SDK values exactly

## Testing Infrastructure

### Dependencies
The project uses **GoogleTest v1.17.0** for unit testing, managed via CMake FetchContent:
- Locations: `cmake/dependencies.cmake`, `libs/xheader/cmake/dependencies.cmake`, `libs/debug/cmake/dependencies.cmake`
- Automatically fetched from GitHub during CMake configuration (shallow clone)
- Provides: `gtest`, `gmock`, `gtest_main`, `gmock_main` targets

**Note**: xheader library is C23, but tests are C++ (GoogleTest requires C++). xheader CMakeLists.txt declares both `LANGUAGES C CXX`.

### Test Targets

The project uses a hierarchical test target structure:

1. **Library-specific test executables**: `xheader_win32api_tests`, `xheader_dlfnc_tests`, `debug_utility_tests`
   - Self-contained test executable per library/API
   - Links against its specific library and GoogleTest
   - Can be run independently

2. **Aggregate library test targets**: `xheader_tests`, `debug_tests`
   - Reuses sources from library-specific tests
   - `xheader_tests` combines both `xheader_win32api_tests` and `xheader_dlfnc_tests`
   - Provides unified test target per library

3. **Project-wide aggregate**: `all_logenium_tests`
   - Combines all library test targets
   - Single executable for running all project tests

### xheader Tests
- Location: `libs/xheader/tests/`
- Test executables: `xheader_win32api_tests`, `xheader_dlfnc_tests`, `xheader_tests`
- Coverage:
  - Windows API stub functions (`tests/windows/`)
  - Linux dlfcn stub functions (`tests/dlfcn/`)
- Build control: `LOGENIUM_XHEADER_BUILD_TESTS` (default: ON)
- Organization:
  - Windows API: One test file per API function, separate A/W variants
  - Linux dlfcn: One test file per function

### debug Tests
- Location: `libs/debug/tests/`
- Test executables: `debug_utility_tests`, `debug_tests`
- Coverage: Assert, Breakpoint, BreakpointIfDebugging, IsDebuggerPresent
- Build control: `LOGENIUM_DEBUG_BUILD_TESTS` (default: ON)
- Test strategy: Safe predicates in debug builds, consteval verification in release builds
- Key issue: Must link against `logenium::debug` alias (not `debug`) to avoid CMake target name conflicts

### Running Tests
```bash
# Build and run all tests
cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build

# Run specific test executables
./build/libs/xheader/tests/xheader_win32api_tests
./build/libs/xheader/tests/xheader_dlfnc_tests
./build/libs/debug/tests/debug_utility_tests
./build/all_logenium_tests
```

### Test Organization
- Source paths use `${CMAKE_CURRENT_SOURCE_DIR}/` prefix for portability
- Test files are C++ (`.cxx`) even for C libraries
- Test targets specify required C++/C standards
- Tests discovered automatically via `gtest_discover_tests()`

## CMake Configuration Options

Available CMake options:

- `LOGENIUM_BUILD_TESTS`: Master switch for all tests (default: ON)
- `LOGENIUM_XHEADER_BUILD_TESTS`: Build xheader library tests (default: ON)
- `LOGENIUM_DEBUG_BUILD_TESTS`: Build debug library tests (default: ON)

### How Testing Options Work
1. Root `CMakeLists.txt`: Checks `LOGENIUM_BUILD_TESTS`, enables CTest/GoogleTest, creates `all_logenium_tests`
2. Library `CMakeLists.txt`: Checks BOTH master switch AND library-specific option, adds `tests` subdirectory

### Usage Examples
```bash
cmake -B build -G Ninja                              # All tests enabled (default)
cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=OFF   # All tests disabled
cmake -B build -G Ninja -DLOGENIUM_DEBUG_BUILD_TESTS=OFF  # Debug tests disabled only
```

## Current Limitations

- No custom window procedure (uses `DefWindowProc` only)
- No event handling callbacks or custom message handlers
- Windows-only functionality (cross-platform infrastructure exists but unused)
- Minimal error handling (mostly assertions)
- No configuration options for window sizing/positioning
- No support for multiple windows
