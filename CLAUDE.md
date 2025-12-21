# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Logenium is a Logisim-like logic circuit simulator in early development. Built with modern C++ (C++23) and targeting Windows with infrastructure for cross-platform support, the project currently focuses on establishing the GUI foundation. The project uses a layered architecture with platform abstraction through the xheader library.

**Current Status**: The application can create a window and run a basic Windows message loop. Event handling and custom window procedures are not yet implemented.

## Build Commands

### Standard Build
```bash
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
cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build  # Run all tests
```

### Clean Build
```bash
rm -rf build
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

#### Linux/macOS or Manual Usage
```bash
# Run devutils commands (uv auto-manages Python 3.14+)
uv run devutils configure
# Prompts for: testing options, xheader testing, build mode (Debug/Release/RelWithDebInfo/MinSizeRel)

uv run devutils build
# Options: -v/--verbose for verbose output, -j/--jobs for parallel jobs

uv run devutils clean
# Remove the build directory and all its contents

uv run devutils check-license-headers check  # or: cls check
# Check all files for correct SPDX license headers

uv run devutils --version
```

**Note**: devutils is optional. Standard CMake commands work identically. On Linux/macOS, requires [uv](https://github.com/astral-sh/uv) to be installed manually.

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
│   │   └── format.py          # Code formatting with clang-format and ruff
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
- **xheader Testing**: Enable/disable xheader library tests (`LOGENIUM_XHEADER_BUILD_TESTS`)
- **Build Mode**: Debug, Release, RelWithDebInfo, or MinSizeRel
- **Generator**: Always uses Ninja

Generates CMake build files in `build/` directory with selected options.

Usage: `uv run devutils configure`

#### build
Runs Ninja build with custom output handling:
- **Verbose mode**: `-v/--verbose` flag for detailed build output
- **Parallel jobs**: `-j/--jobs N` flag (0 = auto, capped to CPU count)
- **Enhanced status**: Custom `NINJA_STATUS` with human-readable elapsed time (format: `<d>d <h>h <m>m <s>s <ms>ms`)
- **No override mode**: `--no-ninja-override` flag to preserve existing `NINJA_STATUS` environment variable
- **Output normalization**: Converts Ninja's `\r` line updates to `\r\n` for continuous logs
- **Error handling**: Exits with Ninja's exit code on failure

**Default Ninja Status Format**: `[finished/total : percentage - elapsed_time]`

Example output: `[42/238 : 17% - 0d 0h 0m 5s 234ms]`

Requires prior configuration via `devutils configure` or manual CMake setup.

Usage: `uv run devutils build [-v] [-j N] [--no-ninja-override]`

#### clean
Removes the build directory and all its contents. Equivalent to `rm -rf build` but with proper handling of read-only files.

**Features**:
- Checks if build directory exists before attempting removal
- Handles read-only files (common in git objects from CMake FetchContent)
- Uses `shutil.rmtree()` with error handler for Windows compatibility
- Graceful handling when build directory doesn't exist

**Implementation Details**:
- Uses `handle_remove_readonly()` callback to make read-only files writable before deletion
- Essential for cleaning FetchContent dependencies with git objects on Windows

Usage: `uv run devutils clean`

#### check-license-headers (alias: cls)
Validates and fixes SPDX license headers across project files. Supports C, C++, Python, CMake, PowerShell, and Batch files.

**Subcommands**:
- **show-headers**: Display expected license header format for each file type
- **check**: Scan all project files and report header status (OK/MISSING/INCORRECT/ERROR)
- **fix**: Automatically prepend missing or incorrect headers to files

**File Coverage**:
- C/C++ source files (`.c`, `.h`, `.cxx`, `.hxx`) in `src/` and `libs/`
- CMake files (`CMakeLists.txt`, `*.cmake`) in root, `cmake/`, and library directories
- Python files (`.py`, `.pyi`) in `devutils/src/`
- Windows scripts (`devutils.bat`, `devutils.ps1`)

**Expected Header Format** (SPDX-compliant):
```
// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause
```
(Comment syntax varies by language: `//` for C/C++, `#` for Python/CMake/PowerShell, `@REM` for Batch)

**Exit Codes**:
- `0`: Success (all headers correct or successfully fixed)
- `1`: Failure (missing/incorrect headers in check mode, or errors in fix mode)

Usage:
```bash
uv run devutils check-license-headers show-headers  # Display header templates
uv run devutils cls check                           # Check all files (CI-friendly)
uv run devutils cls fix                             # Fix all files
```

**Architecture**:
The command uses a configuration-driven architecture via the `LanguageConfig` dataclass:
- **LanguageConfig**: Encapsulates language name, extensions, search directories, specific files, and license header template
- **get_language_configs()**: Returns list of all language configurations
- **collect_files()**: Method on LanguageConfig that gathers files from search directories and specific file paths
- All commands (`show-headers`, `check`, `fix`) iterate over the language configs

**Adding New Language Support**:
To add a new language, add an entry to `get_language_configs()` in `check_license_headers.py`:
```python
LanguageConfig(
    name="Language Name",
    extensions=Extensions.language_source,  # from constants/extensions.py
    search_dirs=[Directories.target_dir],   # from constants/paths.py
    specific_files=[Directories.root / "specific_file.ext"],
    license_header=LicenseHeaders.language, # from constants/license_header.py
)
```
Also update the corresponding constants files (`extensions.py`, `license_header.py`, optionally `comments.py`).

#### format
Checks and fixes code formatting across the project using language-specific formatters. Supports C/C++ (via clang-format) and Python (via ruff).

**Subcommands**:
- **check**: Verify all files are formatted correctly without modifying them
- **fix**: Automatically format all files that need formatting

**File Coverage**:
- C/C++ files (`.c`, `.h`, `.cxx`, `.hxx`) in `src/`, `include/`, `libs/xheader/`, `libs/debug/`
- Python files (`.py`, `.pyi`) in `devutils/src/`

**Formatter Tools**:
- **clang-format**: Used for C/C++ files
  - Check mode: `clang-format --dry-run -Werror <file>`
  - Fix mode: `clang-format -i <file>`
  - Must be installed and available in PATH
- **ruff**: Used for Python files
  - Check mode: `uv run ruff format --check <file>`
  - Fix mode: `uv run ruff format <file>`
  - Provided by virtual environment (no separate installation needed)

**Exit Codes**:
- `0`: Success (all files formatted correctly or successfully formatted)
- `1`: Failure (unformatted files in check mode, tool not available, or errors in fix mode)

Usage:
```bash
uv run devutils format check  # Check all files (CI-friendly)
uv run devutils format fix    # Format all files
```

**Architecture**:
Similar to `check-license-headers`, the command uses a configuration-driven architecture:
- **LanguageConfig**: Encapsulates language name, extensions, search directories, formatter tool, and tool arguments
- **get_language_configs()**: Returns list of language configurations with formatter settings
- **Tool availability check**: Verifies required formatter tools are installed before processing files
- Shared utilities with `check-license-headers` in `devutils.utils.file_checking` module

**Shared Utilities**:
The format and check-license-headers commands share common code in `devutils/utils/file_checking.py`:
- **collect_files()**: Gathers files from search directories and specific file paths
- **FileResult**: Dataclass for file processing results
- **print_status()**: Standardized file status output formatting
- **format_file_path()**: Converts absolute paths to repository-relative paths

### Path Constants

The `devutils.constants` module provides frozen dataclass instances with project paths:

**Directories** (via `Directories` singleton):
- `root`: Repository root directory
- `build`: Build output directory
- `libs`: Libraries directory
- `logenium_source`, `logenium_include`, `logenium_cmake`: Main application paths
- `xheader_root`, `xheader_source`, `xheader_include`, `xheader_cmake`, `xheader_tests`: xheader library paths (under `libs/`)
- `debug_root`, `debug_source`, `debug_include`: debug library paths (under `libs/`)
- `devutils_root`, `devutils_source`: devutils package paths

**Files** (via `Files` singleton):
- `ninja_build_file`: Path to `build/build.ninja`

All paths are `pathlib.Path` objects calculated relative to the package installation location.

### Additional Constants

**Comments** (via `Comments` singleton in `devutils.constants.comments`):
Frozen dataclass containing comment syntax for different languages:
- `c`, `cpp`: `//` (C-style line comments)
- `python`, `cmake`, `powershell`: `#` (hash comments)
- `bat`: `@REM` (Batch remark)

**Extensions** (via `Extensions` singleton in `devutils.constants.extensions`):
Frozen dataclass with file extension lists:
- `c_source`: `[".c", ".h"]`
- `cpp_source`: `[".cxx", ".hxx"]`
- `cmake_source`: `[".cmake"]`
- `python_source`: `[".py", ".pyi"]`
- `powershell_source`: `[".ps1"]`
- `bat_source`: `[".bat"]`

**LicenseHeaders** (via `LicenseHeaders` singleton in `devutils.constants.license_header`):
Frozen dataclass containing SPDX license header templates as `list[str]` (each line includes newline):
- `c`, `cpp`: C/C++ style with `//` comments
- `python`, `cmake`, `powershell`: Hash-style comments
- `bat`: Batch-style comments with `@REM`

All templates follow the format:
```
<comment> SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
<comment> SPDX-License-Identifier: BSD-3-Clause
<blank line>
```

### Utility Modules

**filesystem** (via `devutils.utils.fs`):
- `find_files_by_extensions(path: Path, extensions: list[str]) -> list[Path]`:
  - Recursively searches `path` for files matching any extension in `extensions`
  - Uses `Path.rglob()` for each extension pattern
  - Returns sorted list of matching file paths

- `get_files_recursively(path: Path) -> list[Path]`:
  - Recursively searches `path` for all files and directories
  - Uses `Path.rglob("*")` to match everything
  - Returns sorted list of all paths (files and directories)

**file_checking** (via `devutils.utils.file_checking`):
Shared utilities for file checking commands (format, check-license-headers):
- `FileResult`: Dataclass for file processing results
  - `path: pathlib.Path` - The file path
  - `status: Enum` - The status enum value (specific to each command)
  - `error: str | None` - Optional error message

- `collect_files(extensions: list[str], search_dirs: list[Path], specific_files: list[Path]) -> list[Path]`:
  - Gathers files from search directories and specific file paths
  - Recursively searches each search directory for files with matching extensions
  - Includes specified files if they exist
  - Returns sorted list of collected file paths

- `format_file_path(file_path: Path) -> str`:
  - Converts absolute file path to repository-relative path
  - Used for consistent output formatting
  - Returns string representation relative to repository root

- `print_status(status_label: str, color: str, file_path: Path, message: str = "") -> None`:
  - Standardized file status output with color formatting
  - Displays status label, file path, and optional message
  - Used by both format and check-license-headers commands

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

### Why uv?
The project uses [uv](https://github.com/astral-sh/uv) instead of traditional `pip` because:
- **No Python installation required**: uv automatically downloads and manages Python versions
- **Fast**: Written in Rust, significantly faster than pip
- **Zero configuration**: Users don't need to manually install Python 3.14+ or create virtual environments
- **Deterministic**: Ensures consistent dependency resolution across environments

### Windows Scripts (devutils.bat & devutils.ps1)

Located at repository root: `devutils.bat` and `devutils.ps1`

Convenience wrappers for Windows users that automate the entire devutils setup and execution process.

**Features** (both scripts):
1. **Auto-install uv**: Detects if uv is installed; if not, downloads and installs it automatically
2. **Environment refresh**: Refreshes PATH after uv installation
3. **Python installation**: Ensures Python 3.14 is installed via `uv python install 3.14`
4. **Virtual environment**: Creates `.venv` if it doesn't exist
5. **Dependencies**: Installs packages from `requirements.txt` on first run
6. **Command forwarding**: Passes all arguments to devutils
7. **User-friendly**: Pauses at end so users can see output

#### devutils.bat (Batch Script)

**Implementation Details**:
- Uses `@echo off` for clean output
- Checks uv availability with `uv --version >nul 2>&1`
- Downloads uv installer: `powershell -c "irm https://astral.sh/uv/install.ps1 | iex"`
- Refreshes environment via registry query (HKLM System and HKCU paths)
- Only creates venv/installs dependencies once (checks for `.venv` directory)
- Runs `uv run devutils %*` where `%*` forwards all script arguments

**Usage**: `devutils.bat <command>` from repository root (cmd.exe)

#### devutils.ps1 (PowerShell Script)

**Implementation Details**:
- Uses `Get-Command` with `-ErrorAction Stop` to check for uv
- Downloads uv installer: `Invoke-RestMethod https://astral.sh/uv/install.ps1 | Invoke-Expression`
- Refreshes environment via `[System.Environment]::GetEnvironmentVariable()` for Machine and User paths
- Uses `Test-Path ".venv"` to check for existing virtual environment
- Runs `uv run devutils $args` where `$args` forwards all script arguments
- Color-coded output with `Write-Host -ForegroundColor`
- Interactive pause using `$Host.UI.RawUI.ReadKey()`

**Usage**: `.\devutils.ps1 <command>` from repository root (PowerShell)

**Note**: First run of either script may take a few minutes for setup. Subsequent runs are instant.

## Architecture Overview

### Library Architecture

1. **xheader library** (C23): Cross-platform Windows API abstraction
   - Location: `libs/xheader/`
   - Provides stub implementations for non-Windows platforms
   - Each Windows API function has corresponding `.c` files with A/W suffix variants
   - Header: `libs/xheader/include/xheader/windows.h`
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

**Critical Convention**: Windows API functions follow ANSI (A) / Wide (W) naming:
- `*A` functions use `LPCSTR`, `WNDCLASSEXA` (narrow char)
- `*W` functions use `LPCWSTR`, `WNDCLASSEXW` (wide char)
- Implementation files MUST match header declarations exactly

**C23 Constexpr Limitation**: In C23, constexpr pointers can only be null. Non-null pointer constants (like `IDI_APPLICATION`, `IDC_ARROW`) must use `#define` macros, not `constexpr`.

**Platform Detection**: All stub implementations wrapped in `#ifndef _WIN32` blocks.

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
- Locations:
  - Root level: `cmake/dependencies.cmake`
  - xheader level: `libs/xheader/cmake/dependencies.cmake`
- Automatically fetched from GitHub during CMake configuration
- Shallow clone with progress reporting
- Provides: `gtest`, `gmock`, `gtest_main`, `gmock_main` targets
- FetchContent temporarily disables `CMAKE_EXPORT_COMPILE_COMMANDS` during fetch

**Note**: The xheader library is written in C23, but its tests are written in C++ (using GoogleTest framework). This is why the xheader CMakeLists.txt declares both `LANGUAGES C CXX`.

### Test Targets

The project uses a hierarchical test target structure:

1. **Library-specific test executables** (e.g., `xheader_win32api_tests`):
   - Self-contained test executable per library
   - Links against its specific library and GoogleTest
   - Can be run independently

2. **Aggregate library test target** (e.g., `xheader_tests`):
   - Reuses sources from library-specific tests
   - Provides a unified test target per library
   - Same sources, alternative target for aggregation

3. **Project-wide aggregate** (`all_logenium_tests`):
   - Combines all library test targets
   - Single executable for running all project tests
   - Built at root level when `LOGENIUM_BUILD_TESTS=ON`

### xheader Tests
The xheader library includes comprehensive unit tests using GoogleTest/GMock:
- Location: `libs/xheader/tests/`
- Test executables: `xheader_win32api_tests`, `xheader_tests`
- Coverage: All Windows API stub functions have corresponding tests
- Build control: `LOGENIUM_XHEADER_BUILD_TESTS` option (ON by default)

### Running Tests
```bash
# Build and run all tests
cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build

# Run specific test executable
./build/libs/xheader/tests/xheader_win32api_tests
./build/libs/xheader/tests/xheader_tests
./build/all_logenium_tests
```

### Test Organization
- One test file per Windows API function
- Separate tests for A/W variants (e.g., `get_message_a.cxx`, `get_message_w.cxx`)
- All source paths use `${CMAKE_CURRENT_SOURCE_DIR}/` prefix for portability
- Tests verify stub implementations compile and link correctly
- All test files are C++ (`.cxx`) even though the library is C
- Test targets explicitly specify C++23 and C23 standard requirements
- Tests are discovered automatically via `gtest_discover_tests()`

## CMake Configuration Options

Available CMake options (defined in `cmake/options.cmake` and `libs/xheader/cmake/options.cmake`):

- `LOGENIUM_BUILD_TESTS`: Master switch for all tests (default: ON)
- `LOGENIUM_XHEADER_BUILD_TESTS`: Build xheader library tests (default: ON)

### How Testing Options Work
1. Root `CMakeLists.txt`:
   - Includes `dependencies.cmake` to fetch GoogleTest
   - Checks `LOGENIUM_BUILD_TESTS`
   - If ON: enables CTest and GoogleTest integration via `enable_testing()`, `CTest`, and `GoogleTest` modules
   - Creates `all_logenium_tests` target that aggregates tests from all libraries

2. xheader `CMakeLists.txt`:
   - Includes `cmake/dependencies.cmake` (library-level GoogleTest fetch)
   - Checks BOTH `LOGENIUM_BUILD_TESTS` AND `LOGENIUM_XHEADER_BUILD_TESTS`
   - Only then does it add the `tests` subdirectory
   - Creates both `xheader_win32api_tests` and `xheader_tests` targets

### Usage Examples
```bash
# Build with all tests (default)
cmake -B build -G Ninja

# Disable all tests
cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=OFF

# Build only specific library tests (xheader tests disabled)
cmake -B build -G Ninja -DLOGENIUM_XHEADER_BUILD_TESTS=OFF
```

## Current Limitations

- No custom window procedure (uses `DefWindowProc` only)
- No event handling callbacks or custom message handlers
- Windows-only functionality (cross-platform infrastructure exists but unused)
- Minimal error handling (mostly assertions)
- No configuration options for window sizing/positioning
- No support for multiple windows
