# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Logenium is a Logisim-like logic circuit simulator in early development. Built with modern C++ (C++23) and targeting Windows and Linux platforms, the project currently focuses on establishing the GUI foundation. The project uses a layered architecture with platform abstraction through the xheader library.

**Current Status**:
- **Windows**: The application can create a window and run a basic message loop. Event handling and custom window procedures are not yet implemented.
- **Linux**:
  - **X11**: Fully functional window creation with XCB. Event loop implemented but event handling not yet processed.
  - **Wayland**: Fully functional window creation with server-side decorations. Event loop implemented, close event handled.

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

uv run devutils codegen wayland
# Generate Wayland protocol files using wayland-scanner

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
│   │   ├── codegen/           # Code generation commands
│   │   │   ├── __init__.py    # Codegen command group
│   │   │   └── wayland.py     # Wayland protocol generation
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

**Note**: When adding new source files or if license headers are missing, run `uv run devutils cls fix` to automatically add the correct SPDX headers to all files.

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
- Python: mypy (type checking with **strict mode**, read-only) + ruff (linting, can fix)
- Requires: clang-tidy/clang-check in PATH, `build/compile_commands.json`; mypy/ruff auto-installed via uv

**Python Type Checking** (mypy strict configuration in `devutils/pyproject.toml`):
- **Strict mode enabled**: All optional error checking flags active
- **Disallows**: Untyped definitions, incomplete definitions, untyped calls/decorators, explicit `Any` types, `Any` in expressions/generics
- **Enforces**: No implicit reexports, no implicit optional, strict equality checks
- **No `Incomplete` types**: All type annotations must be complete and explicit
- Configuration automatically loaded via `--config-file` argument in lint command

**Performance**:
- Parallel: 3-5x speedup (first run)
- Cache (`.cache/devutils/lint_cache.yaml`, mtime-based): 10-50x speedup (no changes), 5-10x (partial changes)

**Output Tags**: `[OK]`, `[CACHED:OK]`, `[WARNING]`, `[HAS_ISSUES]`, `[FIXED]`, `[PARTIAL]`, `[ERROR]`
- Warnings don't cause failure; errors do

Usage: `uv run devutils lint check [--no-cache]` | `uv run devutils lint fix [--no-cache]`

**Important for AI Coding Agents**: For performance and API usage efficiency, always use `uv run devutils lint check` with caching **enabled** (default behavior, omit `--no-cache` flag). The intelligent cache provides 10-50x speedup on unchanged files while maintaining accuracy through mtime-based validation.

**Architecture**: Multi-step parallel processing via `ThreadPoolExecutor`, thread-safe YAML cache (`LintCacheManager`), per-file per-tool cache entries. Cache key: `"{language}:{tool}:{relative_path}"`. To add lint steps, modify `lint_steps` in `get_language_configs()`. Dependencies: `pyyaml>=6.0.3`, `types-pyyaml>=6.0.12.20250915`.

#### python stubgen
Generates/checks Python stub files (`.pyi`) inline with source code using mypy's stubgen.

**Subcommands**: `generate [-v]`, `check [-v]`

**Process**: Generate to temp (`stubgen -p devutils`) → Copy `.pyi` alongside `.py` files → 23 stubs for devutils package

**Regenerate when**: Adding/modifying functions, classes, type annotations, or modules

Usage: `uv run devutils python stubgen generate` | `uv run devutils python stubgen check`

#### codegen wayland
Generates Wayland protocol files from XML protocol specifications using `wayland-scanner`.

**Requirements**:
- `wayland-scanner` executable must be in PATH
- `wayland-protocols` package must be installed (provides `/usr/share/wayland-protocols/`)

**Output Files** (per protocol):
- **xdg-shell**:
  - Client header: `libs/xheader/include/xheader/internal/xdg-shell-client-protocol.h`
  - Private code: `libs/xheader/src/internal/xdg-shell-protocol.c`
- **xdg-decoration-unstable-v1**:
  - Client header: `libs/xheader/include/xheader/internal/xdg-decoration-unstable-v1-client-protocol.h`
  - Private code: `libs/xheader/src/internal/xdg-decoration-unstable-v1-protocol.c`

**Protocol Sources**:
- `/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml`
- `/usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml`

**Process**:
1. Checks if `wayland-scanner` is available
2. For each protocol:
   - Verifies protocol file exists (skips if not found)
   - Generates client header with `wayland-scanner client-header`
   - Generates private code with `wayland-scanner private-code`

**Installation** (if wayland-scanner not found):
- Arch/Manjaro: `sudo pacman -S wayland-protocols`
- Ubuntu/Debian: `sudo apt install wayland-protocols libwayland-dev`
- Fedora: `sudo dnf install wayland-protocols-devel`

Usage: `uv run devutils codegen wayland`

**Note**: This command is Linux-specific. On other platforms, it will fail gracefully if `wayland-scanner` is not found.

**Git Integration**: Generated files are listed in `libs/xheader/.gitignore` and should not be committed to version control. Run `uv run devutils codegen wayland` after cloning the repository to regenerate these files.

### devutils Constants & Utilities

**Constants Architecture** in `devutils.constants`:

All constant modules use **class-level attributes pattern** - classes accessed directly without instantiation:
- All exports are public classes (no private `_ClassName` pattern)
- Access via class attributes: `Directories.root`, `Extensions.c_source`, `Comments.python`
- Values are class-level constants (not instance attributes)
- Memory-efficient (no instance creation, no object overhead)
- Enables clean type inference and autocomplete in IDEs

**Key Implementation Details**:
- `Directories` and `Files`: Frozen dataclasses with simple default values from module-level constants
- `Files`: References `Directories` class for derived paths (e.g., `Directories.build / "build.ninja"`)
- `Comments`, `Extensions`, `LicenseHeaders`: Regular classes (not dataclasses) with class attributes
  - Avoids dataclass mutable default issues
  - Simpler for never-instantiated namespace classes

**Available Constants**:
- `Directories`: Project paths (`root`, `build`, `libs`, library-specific paths)
- `Files`: Build and config file paths (`ninja_build_file`, `devutils_lint_cache_file`, `devutils_pyproject_toml`)
- `Comments`: Language comment syntax (`c="//",` `python="#"`, `bat="@REM"`)
- `Extensions`: File extension lists (`c_source=[".c", ".h"]`, `python_source=[".py", ".pyi"]`)
- `LicenseHeaders`: SPDX header templates for each language (runtime-generated from `Comments`)

**Type Safety**:
- `Directories` and `Files`: Frozen dataclasses with explicit type annotations
- `Comments`, `Extensions`, `LicenseHeaders`: Regular classes with `ClassVar` annotations for class-level attributes
  - Example: `c_source: ClassVar[list[str]] = [".c", ".h"]`
  - Required for correct mypy strict mode and stubgen compatibility
- No private classes - all public exports via `__all__` where applicable
- Passes strict mypy checks with zero `Incomplete` or `Any` types
- Full type inference: mypy understands `Directories.root: Path`, `Extensions.c_source: ClassVar[list[str]]`, etc.
- Stub files (`.pyi`) auto-generated by stubgen preserve `ClassVar` annotations

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
- **Mypy**: Maximum strictness type checking with Python 3.14 target
  - `strict = true`: All optional error checking flags enabled
  - **Disallow untyped code**: `disallow_untyped_defs`, `disallow_incomplete_defs`, `disallow_untyped_calls`, `disallow_untyped_decorators`
  - **Disallow Any types**: `disallow_any_unimported`, `disallow_any_expr`, `disallow_any_decorated`, `disallow_any_explicit`, `disallow_any_generics`, `disallow_subclassing_any`
  - **Additional checks**: `warn_return_any`, `warn_redundant_casts`, `warn_unused_ignores`, `warn_no_return`, `warn_unreachable`, `no_implicit_reexport`, `no_implicit_optional`, `strict_equality`, `extra_checks`
  - **No `Incomplete` types allowed**: All module-level Typer objects must have explicit `typer.Typer` type annotations

**Entry Points**: Two CLI entry points (`logenium-devutils`, `devutils`) map to `devutils.__main__:app`

### devutils Development Notes

**Common Pitfalls**:
1. **NEVER edit .pyi stub files manually** - they are automatically generated by stubgen and will be overwritten. Always make changes to the .py source files instead
2. For class-level attributes, use `ClassVar` in the source .py files to ensure correct stub generation: `field: ClassVar[type] = value`
3. Capture subprocess error output in `FileResult`: `error_output = result.stdout + result.stderr`
4. Display error messages from `FileResult.error` in `check_files()`/`fix_files()`
5. Clear `__pycache__` after changes: `find devutils -name "__pycache__" -type d -exec rm -rf {} +`

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
   - **Linux XCB API** (`xheader/xcb/xcb.h`, `xheader/xcb/xproto.h`):
     - Re-exports system `<xcb/xcb.h>` and `<xcb/xproto.h>` on Linux
     - Provides stub implementations for non-Linux platforms
     - Functions: Connection management, window creation, event handling
     - Links against `xcb` library on Linux
   - **Wayland Protocols** (`xheader/wayland-client-protocol.h`, `xheader/xdg-shell-client-protocol.h`, `xheader/xdg-decoration-unstable-v1-client-protocol.h`):
     - Re-exports system Wayland headers on Linux
     - Provides stub implementations for non-Linux platforms
     - Links against `wayland-client` library on Linux
   - **Generated Files** (`xheader/internal/`):
     - Wayland protocol files generated by `wayland-scanner`
     - Located in `include/xheader/internal/` and `src/internal/`
     - Protocols: `xdg-shell`, `xdg-decoration-unstable-v1`
     - Listed in `libs/xheader/.gitignore` (not committed to version control)
     - Regenerate with `uv run devutils codegen wayland` after cloning
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
├── WindowsApplication
│   └── Creates WindowsWindow
└── LinuxApplication
    ├── X11Application
    │   └── Creates X11Window
    └── WaylandApplication
        └── Creates WaylandWindow

Window (abstract)
├── WindowsWindow
└── LinuxWindow
    ├── X11Window
    └── WaylandWindow
```

### NativeHandle Pattern

Both `Application` and `Window` classes use a `NativeHandle` nested struct that:
- Wraps platform-specific handles:
  - Windows: HMODULE (Application), HWND (Window)
  - Linux: void* from dlopen (Application)
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

#### Windows Platform

1. **Initialization** (`main()` → `Application::Create()`):
   - Factory returns `WindowsApplication` on Windows
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

#### Linux Platform

1. **Initialization** (`main()` → `Application::Create()` → `LinuxApplication::Create()`):
   - Factory checks `XDG_SESSION_TYPE` environment variable
   - Returns `X11Application` if session type is "x11"
   - Returns `WaylandApplication` if session type is "wayland"
   - Asserts on unsupported session types
   - `LinuxApplication` base constructor:
     - Calls `dlopen(nullptr, RTLD_NOW)` to self-load the application
     - Stores handle in `native_handle`
   - `X11Application`/`WaylandApplication` constructors:
     - Call parent constructor
     - Call `RegisterWindowClass()` (currently stub)
   - Window creation: Not yet implemented

2. **Main Loop** (`app->Run()`):
   - Currently stub implementations (empty functions)
   - No event loop implemented yet

3. **Cleanup** (`~LinuxApplication()`):
   - Calls `dlclose(native_handle)` to close self-loaded handle
   - Asserts on failure

### Application State Management

The `Application::State` struct tracks runtime state:
```cpp
struct State {
    bool is_running{false};
};
```

Accessible via `GetState()` method. Used to control the main loop execution.

### Linux Application Architecture

The Linux platform implementation uses a hierarchical structure with session detection:

**Session Type Detection**:
- Uses `XDG_SESSION_TYPE` environment variable to detect display server
- Supported values: `"x11"` (X11), `"wayland"` (Wayland)
- Detection happens in `LinuxApplication::Create()` static factory method
- Asserts on unsupported or missing session types

**Class Hierarchy**:
```
LinuxApplication (base)
├── X11Application (X11-specific)
└── WaylandApplication (Wayland-specific)

LinuxWindow (base)
├── X11Window (X11-specific)
└── WaylandWindow (Wayland-specific)
```

**Native Handle Usage**:
- `LinuxApplication` uses `dlopen(nullptr, RTLD_NOW)` for self-loading
- Returns void* handle stored in `native_handle`
- Cleaned up with `dlclose()` in destructor
- Required for dynamic symbol resolution (future use)

**Current Implementation Status**:
- Session detection: ✓ Implemented
- Application factory: ✓ Implemented
- **X11 Implementation**:
  - XCB connection: ✓ Implemented (xcb_connect, xcb_disconnect)
  - Window creation: ✓ Implemented (X11Window uses xcb_create_window, xcb_map_window)
  - Event loop: ✓ Implemented (xcb_wait_for_event in Run() method)
  - Event handling: ✗ Events are consumed but not processed
  - RegisterWindowClass/UnregisterWindowClass: Empty stubs (not needed for XCB)
- **Wayland Implementation**:
  - Wayland connection: ✓ Implemented (wl_display_connect, wl_display_disconnect)
  - Registry binding: ✓ Implemented (wl_compositor, xdg_wm_base, wl_shm, zxdg_decoration_manager_v1)
  - Window creation: ✓ Implemented (WaylandWindow uses wl_compositor, xdg_surface, xdg_toplevel)
  - Server-side decorations: ✓ Implemented (xdg-decoration protocol for titlebar/borders)
  - Shared memory buffer: ✓ Implemented (creates 1280x720 buffer via memfd_create)
  - Event loop: ✓ Implemented (wl_display_dispatch in Run() method)
  - Event handling: ✓ Partial (configure events handled, close event stops application)
  - RegisterWindowClass/UnregisterWindowClass: Empty stubs (not needed for Wayland)

**File Locations**:
- Base classes: `include/logenium/platform/linux/`, `src/platform/linux/`
- X11 implementation: `include/logenium/platform/linux/X11/`, `src/platform/linux/X11/`
- Wayland implementation: `include/logenium/platform/linux/wayland/`, `src/platform/linux/wayland/`

### Wayland Window Implementation

The Wayland implementation uses the modern Wayland protocol with server-side decorations:

**Protocols Used**:
- `wl_compositor` - Surface creation and management
- `xdg_wm_base` - XDG shell for desktop windows
- `wl_shm` - Shared memory buffers
- `zxdg_decoration_manager_v1` - Server-side window decorations (titlebar, borders)

**Window Creation Flow** (`WaylandWindow` constructor):
1. Create `wl_surface` from compositor
2. Create `xdg_surface` from XDG shell base
3. Attach `xdg_surface_listener` for configure events
4. Create `xdg_toplevel` from XDG surface
5. Attach `xdg_toplevel_listener` for configure/close events
6. Set window title to "Logenium"
7. Request server-side decorations (if decoration manager available)
8. Commit surface (initial configuration request)

**Event Handling**:
- `XdgSurfaceConfigure`: Acknowledges compositor configuration, creates buffer on first configure
- `XdgToplevelConfigure`: Handles window resize requests (updates width/height if non-zero)
- `XdgToplevelClose`: Sets `is_running = false` to stop application

**Buffer Creation** (`CreateBuffer` method):
1. Calculate buffer size: `width * height * 4` bytes (XRGB8888 format)
2. Create anonymous file descriptor via `memfd_create`
3. Set file size via `ftruncate`
4. Map memory via `mmap` with `MAP_SHARED`
5. Fill buffer with gray color (0x80)
6. Create `wl_shm_pool` from file descriptor
7. Create `wl_buffer` from pool
8. Attach buffer to surface and commit

**Destruction Order** (`~WaylandApplication`):
1. Reset `is_running` flag (if still running)
2. **Destroy window first** via `window.reset()` (critical to avoid segfault)
3. Disconnect from display via `wl_display_disconnect`

**Server-Side Decorations**:
- Requested via `zxdg_decoration_manager_v1_get_toplevel_decoration`
- Mode set to `ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE`
- Compositor provides native titlebar, borders, and window controls
- Gracefully degrades if decoration manager not available (null check)

**Important Notes**:
- Window must be destroyed before display disconnect to prevent use-after-free
- Buffer is created once during first configure event, never updated
- Uses POSIX headers: `<fcntl.h>`, `<sys/mman.h>`, `<unistd.h>` (via xheader wrappers)
- Default window size: 1280x720 pixels

### Window Class Configuration (Windows)

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

## Linux XCB API Functions in xheader

The xheader library provides X11 XCB (X protocol C-language Binding) functions via `<xheader/xcb/xcb.h>` and `<xheader/xcb/xproto.h>`:

### XCB Connection Functions

- **xcb_connect**: Establishes connection to X server
  - Signature: `xcb_connection_t *xcb_connect(const char *displayname, int *screenp)`
  - On Linux: System implementation from `<xcb/xcb.h>`
  - On non-Linux: Stub returns `NULL`
  - Usage: `xcb_connect(nullptr, nullptr)` for default display

- **xcb_connection_has_error**: Checks connection error status
  - Signature: `int xcb_connection_has_error(xcb_connection_t *c)`
  - On Linux: System implementation from `<xcb/xcb.h>`
  - On non-Linux: Stub returns `0` (no error)
  - Returns non-zero error code if connection failed

- **xcb_disconnect**: Closes connection to X server
  - Signature: `void xcb_disconnect(xcb_connection_t *c)`
  - On Linux: System implementation from `<xcb/xcb.h>`
  - On non-Linux: Stub does nothing
  - Should be called to clean up connection resources

- **xcb_get_setup**: Retrieves X server setup information
  - Signature: `const struct xcb_setup_t *xcb_get_setup(xcb_connection_t *c)`
  - On Linux: System implementation from `<xcb/xcb.h>`
  - On non-Linux: Stub returns `NULL`
  - Returns setup information including screen roots

- **xcb_generate_id**: Generates unique resource ID
  - Signature: `uint32_t xcb_generate_id(xcb_connection_t *c)`
  - On Linux: System implementation from `<xcb/xcb.h>`
  - On non-Linux: Stub returns `0`
  - Used to allocate IDs for windows and other resources

- **xcb_flush**: Flushes pending requests to X server
  - Signature: `int xcb_flush(xcb_connection_t *c)`
  - On Linux: System implementation from `<xcb/xcb.h>`
  - On non-Linux: Stub returns `0`
  - Ensures all requests are sent to server

- **xcb_wait_for_event**: Waits for and retrieves next event
  - Signature: `xcb_generic_event_t *xcb_wait_for_event(xcb_connection_t *c)`
  - On Linux: System implementation from `<xcb/xcb.h>`
  - On non-Linux: Stub returns `NULL`
  - Blocks until an event is available

### XCB Protocol Functions

- **xcb_setup_roots_iterator**: Gets iterator for screen roots
  - Signature: `xcb_screen_iterator_t xcb_setup_roots_iterator(const xcb_setup_t *R)`
  - On Linux: System implementation from `<xcb/xproto.h>`
  - On non-Linux: Stub returns zero-initialized iterator
  - Used to access screen information from setup

- **xcb_create_window**: Creates a new window
  - Signature: `xcb_void_cookie_t xcb_create_window(xcb_connection_t *c, uint8_t depth, xcb_window_t wid, xcb_window_t parent, int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t border_width, uint16_t _class, xcb_visualid_t visual, uint32_t value_mask, const void *value_list)`
  - On Linux: System implementation from `<xcb/xproto.h>`
  - On non-Linux: Stub returns zero cookie
  - Creates window with specified parameters

- **xcb_map_window**: Maps (displays) a window
  - Signature: `xcb_void_cookie_t xcb_map_window(xcb_connection_t *c, xcb_window_t window)`
  - On Linux: System implementation from `<xcb/xproto.h>`
  - On non-Linux: Stub returns zero cookie
  - Makes window visible on screen

- **xcb_destroy_window**: Destroys a window
  - Signature: `xcb_void_cookie_t xcb_destroy_window(xcb_connection_t *c, xcb_window_t window)`
  - On Linux: System implementation from `<xcb/xproto.h>`
  - On non-Linux: Stub returns zero cookie
  - Removes window and frees resources

### XCB Type Definitions

The library provides stub type definitions for non-Linux platforms:

**Connection Types** (`<xheader/xcb/xcb.h>`):
- `xcb_connection_t`: Opaque connection handle
- `xcb_generic_event_t`: Generic event structure for all X11 events
- `xcb_void_cookie_t`: Cookie returned by requests that don't return data

**Protocol Types** (`<xheader/xcb/xproto.h>`):
- `xcb_keycode_t`: Key code type (`uint8_t`)
- `xcb_window_t`: Window identifier (`uint32_t`)
- `xcb_colormap_t`: Colormap identifier (`uint32_t`)
- `xcb_visualid_t`: Visual identifier (`uint32_t`)
- `xcb_screen_t`: Screen information structure
- `xcb_setup_t`: Server setup information structure
- `xcb_screen_iterator_t`: Screen iterator structure
- `xcb_window_class_t`: Enum for window classes (COPY_FROM_PARENT, INPUT_OUTPUT, INPUT_ONLY)

**Constants** (`<xheader/xcb/xcb.h>`):
- `XCB_COPY_FROM_PARENT`: Value to copy attribute from parent window

### All Implemented Functions
Complete list of xheader XCB stubs (see `libs/xheader/CMakeLists.txt` for current list):
- **Connection management**: `xcb_connect`, `xcb_connection_has_error`, `xcb_disconnect`, `xcb_get_setup`, `xcb_generate_id`, `xcb_flush`, `xcb_wait_for_event`
- **Window management**: `xcb_create_window`, `xcb_map_window`, `xcb_destroy_window`
- **Protocol utilities**: `xcb_setup_roots_iterator`

**Note**: Functions support basic X11 window creation and event handling. Additional XCB functions will be added as needed.

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

## Adding New Linux XCB Functions

1. Determine appropriate header file:
   - Connection functions: `libs/xheader/include/xheader/xcb/xcb.h`
   - Protocol functions: `libs/xheader/include/xheader/xcb/xproto.h`
2. Add function declaration to appropriate header
3. Create stub implementation file:
   - For `xcb.h` functions: `libs/xheader/src/xcb/xcb/function_name.c`
   - For `xproto.h` functions: `libs/xheader/src/xcb/xproto/function_name.c`
4. Add to `libs/xheader/CMakeLists.txt` sources
5. Create corresponding test file:
   - For `xcb.h` functions: `libs/xheader/tests/xcb/xcb/function_name.cxx`
   - For `xproto.h` functions: `libs/xheader/tests/xcb/xproto/function_name.cxx`
6. Add test source to appropriate test CMakeLists.txt:
   - For `xcb.h` tests: `libs/xheader/tests/xcb/xcb/CMakeLists.txt`
   - For `xproto.h` tests: `libs/xheader/tests/xcb/xproto/CMakeLists.txt`
7. Ensure proper platform detection:
   - Header: Use `#ifdef __linux__` to re-export system header
   - Implementation: Use `#ifndef __linux__` to wrap stub implementation
   - Example:
     ```c
     // In xcb.h
     #ifdef __linux__
     #include <xcb/xcb.h>  // IWYU pragma: export
     #else
     void xcb_new_function(xcb_connection_t *c);
     #endif

     // In xcb/xcb_new_function.c
     #ifndef __linux__
     void xcb_new_function(xcb_connection_t *c) {}
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

### Windows Platform
- No custom window procedure (uses `DefWindowProc` only)
- No event handling callbacks or custom message handlers
- Minimal error handling (mostly assertions)
- No configuration options for window sizing/positioning
- No support for multiple windows

### Linux Platform

**X11 Implementation:**
- Basic window creation implemented (creates 1280x720 window)
- Event loop implementation (waits for events via `xcb_wait_for_event`)
- XCB connection management working
- No event handling callbacks or custom event processing
- No configuration options for window sizing/positioning
- No support for multiple windows
- RegisterWindowClass/UnregisterWindowClass are empty stubs (not needed for XCB)

**Wayland Implementation:**
- Basic window creation implemented (creates 1280x720 window with gray fill)
- Server-side decorations working (titlebar, borders, window controls)
- Event loop implementation (dispatches events via `wl_display_dispatch`)
- Configure events handled (window resize requests processed)
- Close event handled (stops application gracefully)
- No custom event processing beyond configure/close
- No configuration options for window sizing/positioning
- No support for multiple windows
- Buffer is created once and never updated (static gray screen)
- RegisterWindowClass/UnregisterWindowClass are empty stubs (not needed for Wayland)
