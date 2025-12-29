# CLAUDE.md

Guidance for Claude Code when working with this repository.

## Project Overview

Logenium: Logisim-like logic circuit simulator in early development. C++23, targets Windows/Linux, focuses on GUI foundation with platform abstraction via xheader library.

**Status**: Windows (window creation, message loop), X11 (XCB window, event loop), Wayland (window with decorations, event loop, close handling). Event processing not yet implemented.

## Compiler Requirements

**Requires Clang** (`clang`/`clang++`). On Windows: GNU-compatible driver only, not `clang-cl`.

**Setup**: `export CC=clang CXX=clang++` or `-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++`

Build system verifies Clang usage and fails if requirements not met.

## Build Commands

**Standard**: `export CC=clang CXX=clang++; cmake -B build -G Ninja; cmake --build build`
**With Tests**: Add `-DLOGENIUM_BUILD_TESTS=ON` to cmake
**Run**: `./build/logenium`
**Clean**: `rm -rf build`

### devutils (Optional Python CLI)

Wrapper scripts auto-install uv/Python 3.14, manage env:
- Windows: `devutils.bat` or `devutils.ps1`
- Linux/macOS: `./devutils.sh`

**Commands**: `configure`, `build [-v] [-j N]`, `clean`, `check-license-headers {check|fix} [--no-cache]`, `codegen wayland`, `format {check|fix}`, `lint {check|fix} [--no-cache]`, `python stubgen {generate|check}`

**Manual**: `uv run devutils <command>` (requires uv installed)

## devutils Package

Python CLI for building/linting. Typer-based, fully typed (mypy strict, PEP 561), uses uv for dependency management.

**Structure**: `commands/` (configure, build, clean, check_license_headers, codegen/wayland, format, lint, python/stubgen), `constants/` (paths, comments, extensions, license_header), `utils/` (filesystem, file_checking)

**configure**: Interactive CMake config (testing, build mode, checks Wayland codegen files). Saves to `config.yaml` (revision r1, JSON schema validated). Use `--reconfigure` to remove saved config and reconfigure. Subsequent runs load from config (no prompts). Config structure:
```yaml
revision: 1
enable_testing: bool
enable_xheader_testing: bool
enable_debug_testing: bool
enable_corelib_testing: bool
build_mode: Debug|Release|RelWithDebInfo|MinSizeRel
```
**build**: Ninja with progress, `-v/--verbose`, `-j/--jobs N`
**clean**: Remove build directory (handles read-only files)
**check-license-headers** (cls): Validate/fix SPDX headers (C/C++, Python, CMake, Batch, PowerShell) with parallel processing & YAML cache. Parallel (3-5x), cache (10-50x). Run `cls fix` for new files. **AI Agents**: Always use cache (default).
**format**: Check/fix formatting (clang-format for C/C++, ruff for Python). Python: 31x faster via package-level execution.
**lint**: Multi-tool linting with parallel processing & YAML cache. C/C++: clang-tidy+clang-check. Python: mypy (strict mode)+ruff. Package-level execution (31x faster), parallel (3-5x), cache (10-50x). **AI Agents**: Always use cache (default).
**python stubgen**: Generate/check `.pyi` stubs with mypy stubgen
**codegen wayland**: Generate Wayland protocol files from YAML config (`libs/xheader/data/codegen/wayland.yaml`). Linux: requires `wayland-scanner`, runs from YAML. Windows: creates stub files. JSON schema validated. Add protocols by editing YAML.

### devutils Internals

**Constants** (`devutils.constants`): Class-level attributes pattern. `paths/` (Directories, Files, CodegenFiles, ConfigFiles, JsonSchemas - frozen dataclasses), `comments`, `extensions`, `license_header`. Access: `Directories.root`, `Extensions.c_source`. Fully typed (`ClassVar` for class attrs).

**Schemas**: `JsonSchemas.codegen` (codegen config, r1), `JsonSchemas.config` (configure command, r1). All configs include `revision` field, validated with jsonschema.

**Utilities**: `fs` (filesystem ops), `file_checking` (FileResult, FileStatus, LanguageConfig, Statistics, collect_files)

**Development**: Python 3.14+, Typer 0.20.1+, Ruff (formatting/linting), Mypy (strict mode, no `Any`/`Incomplete`). Entry points: `logenium-devutils`, `devutils`.

**Pitfalls**: Never edit `.pyi` manually (stubgen-generated). Use `ClassVar` for class attrs. Capture subprocess stderr. Clear `__pycache__` after changes.

## Architecture

### Libraries

1. **xheader** (C23, `libs/xheader/`): Cross-platform API abstraction. Windows API (A/W variants), Linux dlfcn (`dlopen`/`dlclose`), XCB (connection, window mgmt), Wayland protocols (re-exports on Linux, stubs on Windows). Generated files in `internal/` (not in git, regenerate with `codegen wayland`). Tests are C++ (GoogleTest), declares `LANGUAGES C CXX`.

2. **debug** (C++23, `libs/debug/`): Debug utilities (Assert, Breakpoint, BreakpointIfDebugging, IsDebuggerPresent). No-ops in release builds (consteval). Uses std::format, std::source_location, std::stacktrace.

3. **corelib** (C++23, `libs/corelib/`): Core utilities library. Currently provides `type_name<T>()` for compile-time type name extraction using compiler intrinsics (`__PRETTY_FUNCTION__` for GCC/Clang, `__FUNCSIG__` for MSVC). Template overload returns type as `std::string_view`, value overload uses forwarding reference to preserve cv-qualifiers. MSVC specializations normalize `__int64` types to "long long".

4. **logenium** (C++23): GUI framework. Abstract base classes, factory pattern (`Application::Create()`), singleton. Main: `main()` → `Create()` → `Run()`.

### Class Hierarchy

```
Application → {Windows,Linux{X11,Wayland}}Application → Creates corresponding Window
Window → {Windows,Linux{X11,Wayland}}Window
```

**NativeHandle**: Wraps platform handles (Windows: HMODULE/HWND, Linux: dlopen void*). Implicit conversions for type safety.

## Implementation Details

**xheader Conventions**:
- Windows API: A/W variants (ANSI/Wide). Stubs in `#ifndef _WIN32`.
- Linux APIs: Re-export system headers on Linux (`#include <header.h> // IWYU pragma: export`), stubs on non-Linux (`#ifndef __linux__`).
- C23 limitation: Non-null pointer constants use `#define`, not `constexpr`.

**Application Lifecycle**:

**Windows**: `Create()` → `WindowsApplication` (GetModuleHandle, RegisterWindowClass, create WindowsWindow with CreateWindowEx/ShowWindow) → `Run()` (GetMessage/TranslateMessage/DispatchMessage loop) → cleanup (destroy window, UnregisterClass).

**Linux**: `Create()` checks `XDG_SESSION_TYPE` → X11Application or WaylandApplication. Base: `dlopen(nullptr, RTLD_NOW)` for self-load. X11: xcb_connect, create X11Window (xcb_create_window, xcb_map_window), Run (xcb_wait_for_event loop). Wayland: wl_display_connect, registry bind (compositor, xdg_wm_base, shm, decorations), create WaylandWindow, Run (wl_display_dispatch loop).

**State**: `struct State { bool is_running; }`. Accessed via `GetState()`.

**Linux Details**:
- Session detection via `XDG_SESSION_TYPE`. Factory in `LinuxApplication::Create()`.
- X11: XCB (connect, create_window, map_window, wait_for_event loop). Events consumed, not processed.
- Wayland: wl_display (connect, registry bind), protocols (compositor, xdg_wm_base, shm, decorations). Window: surface→xdg_surface→xdg_toplevel, server-side decorations. Buffer: memfd_create, mmap (1280x720 XRGB8888, gray fill). Events: configure (creates buffer), close (stops app). **Critical**: Destroy window before display disconnect.
- Files: `include/logenium/platform/linux/{,X11/,wayland/}`, `src/platform/linux/{,X11/,wayland/}`

**Windows Details**: Window class `"LogeniumWindowClass"` (CS_HREDRAW|CS_VREDRAW, DefWindowProc, COLOR_WINDOW+1).

## Code Style

- Files: `.hxx`/`.cxx` (C++), `.h`/`.c` (C). Platform subdirs: `platform/{windows,linux}/{X11,wayland}/`
- Include guards: `#ifndef LOGENIUM_PATH_TO_FILE_HXX`
- Platform: `#if defined(_WIN32)`, `#ifdef __linux__`
- **No documentation comments** unless explicitly requested. Self-documenting code only.
- Include order (clang-format): logenium(7) > debug(6) > xheader(5) > third-party(4) > system(3) > stdlib(2) > other(1). 120 col, Google style, 4-space, right-aligned pointers (`int *ptr`).
- CMake: Custom logging (`log_info`, `log_status`, `log_warning`, `log_error`). Includes: module name only at root, full path in subdirs.

## Git Commits

**Tags**: `[xheader]`, `[debug]`, `[corelib]`, `[devutils]`, `[build]`, `[logenium]`, `[logenium(subcomponent)]`. Untagged = repo-wide.
**Format**: `[tag] lowercase imperative, no period`. Ex: `[xheader] add dlfcn.h stubs`, `[corelib] fix type_name const preservation`, `fix license headers`

## xheader API Functions

**Windows API** (A/W variants, UNICODE macros):
- Window: CreateWindowEx, DestroyWindow, ShowWindow, UpdateWindow, GetWindowLongPtr, SetWindowLongPtr
- Class: RegisterClassEx, UnregisterClass, DefWindowProc
- Message loop: GetMessage (returns -1/0/>0), TranslateMessage, DispatchMessage, PostQuitMessage
- Module/resource: GetModuleHandle, LoadIcon, LoadCursor
- Debug: IsDebuggerPresent, __debugbreak

**Linux dlfcn**: `dlopen` (returns NULL stub), `dlclose` (returns 0 stub). Self-load: `dlopen(nullptr, RTLD_NOW)`. Note: `dlsym`, `dlerror` not implemented.

**Linux XCB** (xcb.h, xproto.h):
- Connection: xcb_connect, xcb_connection_has_error, xcb_disconnect, xcb_get_setup, xcb_generate_id, xcb_flush, xcb_wait_for_event
- Window: xcb_create_window, xcb_map_window, xcb_destroy_window, xcb_setup_roots_iterator
- Types: xcb_connection_t, xcb_generic_event_t, xcb_void_cookie_t, xcb_window_t, xcb_screen_t, etc.

See `libs/xheader/CMakeLists.txt` for full list.

## Adding xheader Functions

**Windows API**: 1) Add to `windows.h`, 2) Create `src/windows/function_name_{a,w}.c` (or just `.c`), 3) Add to CMakeLists, 4) Create tests, 5) Add UNICODE macros if needed.

**Linux dlfcn**: 1) Add to `dlfcn.h` with `#ifdef __linux__` re-export, 2) Create `src/dlfcn/function_name.c` with `#ifndef __linux__` stub, 3) Add to CMakeLists, 4) Create tests.

**Linux XCB**: 1) Choose header (xcb.h or xproto.h), 2) Add with `#ifdef __linux__` re-export, 3) Create `src/xcb/{xcb,xproto}/function_name.c` with `#ifndef __linux__` stub, 4) Add to CMakeLists, 5) Create tests in matching directory, 6) Add to test CMakeLists.

**Types**: Handle types: `struct HFOO__ { int dummy; }; typedef struct HFOO__ *HFOO;`. Constants: `constexpr` for numeric, `#define` for non-null pointers.

## Testing

**GoogleTest v1.17.0** via FetchContent. xheader: C23 lib with C++ tests (declares `LANGUAGES C CXX`).

**Targets**:
1. Per-API: `xheader_win32api_tests`, `xheader_dlfnc_tests`, `debug_utility_tests`, `corelib_type_name_tests`
2. Per-library: `xheader_tests`, `debug_tests`, `corelib_tests`
3. Project-wide: `all_logenium_tests`

**Coverage**: xheader (Windows API, dlfcn stubs), debug (Assert, Breakpoint, IsDebuggerPresent), corelib (type_name template/value overloads, cv-qualifiers, constexpr evaluation). Test files: `.cxx`.

**Options**: `LOGENIUM_BUILD_TESTS` (master, ON), `LOGENIUM_XHEADER_BUILD_TESTS` (ON), `LOGENIUM_DEBUG_BUILD_TESTS` (ON), `LOGENIUM_CORELIB_BUILD_TESTS` (ON).

**Run**: `cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=ON; cmake --build build; ctest --test-dir build`

## Limitations

**Windows**: DefWindowProc only, no custom event handling, fixed window size, single window.
**Linux**: X11/Wayland: basic window creation (1280x720), event loop exists, minimal event processing (Wayland: configure/close only), single window, static buffer (Wayland gray fill).
