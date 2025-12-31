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

**Commands**: `configure`, `build [-v] [-j N]`, `clean`, `check-license-headers {check|fix} [--no-cache]`, `codegen wayland`, `format {check|fix}`, `lint {check|fix} [--no-cache]`, `python stubgen {generate|check}`, `python remove-pycache`, `setup vscode settings [-r]`

**Manual**: `uv run devutils <command>` (requires uv installed)

## devutils Package

Python CLI for building/linting. Typer-based, fully typed (mypy strict, PEP 561), uses uv for dependency management.

**Structure**: `commands/` (configure, build, clean, check_license_headers, codegen/wayland, format, lint, python/stubgen, python/remove_pycache, setup/vscode), `constants/` (paths, comments, extensions, license_header), `utils/` (filesystem, file_checking)

**configure**: Interactive CMake config (testing, build mode, checks Wayland codegen files). Saves to `config.yaml` (revision r2, JSON schema validated). Use `--reconfigure` to remove saved config and reconfigure. Subsequent runs load from config (no prompts). Subcommand: `configure migrate` migrates r1 to r2 configs. Config structure (r2):
```yaml
revision: 2
enable_testing: bool
build_mode: Debug|Release|RelWithDebInfo|MinSizeRel
logenium:
  enable_testing: bool
xheader:
  enable_testing: bool
debug:
  enable_testing: bool
  use_fmtlib: bool
  enable_color_logs: bool
corelib:
  enable_testing: bool
```
**build**: Ninja with progress, `-v/--verbose`, `-j/--jobs N`
**clean**: Remove build directory (handles read-only files)
**check-license-headers** (cls): Validate/fix SPDX headers (C/C++, Python, CMake, Batch, PowerShell) with parallel processing & YAML cache. Parallel (3-5x), cache (10-50x). Run `cls fix` for new files. **AI Agents**: Always use cache (default).
**format**: Check/fix formatting (clang-format for C/C++, ruff for Python). Python: 31x faster via package-level execution.
**lint**: Multi-tool linting with parallel processing & YAML cache. C/C++: clang-tidy+clang-check. Python: mypy (strict mode)+ruff. Package-level execution (31x faster), parallel (3-5x), cache (10-50x). **AI Agents**: Always use cache (default).
**python stubgen**: Generate/check `.pyi` stubs with mypy stubgen
**python remove-pycache**: Remove all `__pycache__` directories recursively from devutils root. Handles read-only files automatically.
**codegen wayland**: Generate Wayland protocol files from YAML config (`libs/xheader/data/codegen/wayland.yaml`). Linux: requires `wayland-scanner`, runs from YAML. Windows: creates stub files. JSON schema validated. Add protocols by editing YAML.
**setup vscode settings**: Create `.vscode/settings.json` with YAML schema mappings for config.yaml and codegen YAML files. Enables IntelliSense/validation in VS Code. Use `-r/--regenerate` to overwrite existing file.

### devutils Internals

**Constants** (`devutils.constants`): Class-level attributes pattern. `paths/` (Directories, Files, CodegenFiles, ConfigFiles, JsonSchemas, SettingsFiles - frozen dataclasses), `comments`, `extensions`, `license_header`. Access: `Directories.root`, `Extensions.c_source`. Fully typed (`ClassVar` for class attrs).

**Schemas**: `JsonSchemas.codegen` (codegen config, r1 at `data/schemas/r1/codegen.schema.json`), `JsonSchemas.config_r1`/`config_r2` (configure command, r1/r2 at `data/schemas/r1|r2/config.schema.json`), `JsonSchemas.config` (alias to `config_r2`). All configs include `revision` field, validated with jsonschema.

**Utilities**: `filesystem` (find_directories_by_name, find_files_by_name, find_files_by_extensions, get_files_recursively), `file_checking` (FileResult, FileStatus, LanguageConfig, Statistics, collect_files)

**Development**: Python 3.14+, Typer 0.20.1+, Ruff (formatting/linting), Mypy (strict mode, no `Any`/`Incomplete`). Entry points: `logenium-devutils`, `devutils`.

**Pitfalls**: Never edit `.pyi` manually (stubgen-generated). Use `ClassVar` for class attrs. Capture subprocess stderr. Clear `__pycache__` after changes.

## Architecture

### Libraries

1. **xheader** (C23, `libs/xheader/`): Cross-platform API abstraction. Windows API (A/W variants), Linux dlfcn (`dlopen`/`dlclose`), XCB (connection, window mgmt), Wayland protocols (re-exports on Linux, stubs on Windows). Generated files in `internal/` (not in git, regenerate with `codegen wayland`). Tests are C++ (GoogleTest), declares `LANGUAGES C CXX`.

2. **debug** (C++23, `libs/debug/`): Debug utilities (Assert, Breakpoint, BreakpointIfDebugging, IsDebuggerPresent). No-ops in release builds (consteval). Uses std::source_location, std::stacktrace. **Always use `debug::Assert` instead of `<cassert>`** - provides better diagnostics with formatted messages, source location, and stack traces.

   **CMake Options**:
   - `LOGENIUM_DEBUG_USE_FAST_STACKTRACE` (OFF): Use fast stacktrace (not reversed printing in assert). Sets `__LOGENIUM_DEBUG_USE_FAST_STACKTRACE__`.
   - `LOGENIUM_DEBUG_USE_FMTLIB` (OFF): Use fmtlib for debug logging (fmt::format/fmt::println instead of std::format/std::println). Links fmt::fmt (v12.1.0 via FetchContent). Sets `__LOGENIUM_DEBUG_USE_FMTLIB__`.
   - `LOGENIUM_DEBUG_USE_COLOR_LOGS` (OFF): Enable colored output (requires fmtlib). Adds red background/white text to "Assertion failed" via fmt::styled. Sets `__LOGENIUM_DEBUG_USE_COLOR_LOGS__`. Validation: automatically disabled if fmtlib not enabled.

   Configuration controlled by devutils config r2: `debug.use_fmtlib`, `debug.enable_color_logs`.

3. **corelib** (C++23, `libs/corelib/`): Core utilities library. Organized into functional modules under `include/corelib/` with convenience headers.

   **utility**: Type utilities including `type_name<T>()` for compile-time type name extraction using compiler intrinsics (`__PRETTY_FUNCTION__` for GCC/Clang, `__FUNCSIG__` for MSVC). Template overload returns type as `std::string_view`, value overload uses forwarding reference to preserve cv-qualifiers. MSVC specializations normalize `__int64` types to "long long". RAII utilities including `Defer<Functor>` for scope-exit execution (executes functor in destructor, supports move semantics) and `AutoRelease<T, InvalidValue>` for automatic resource management with custom releasers (non-copyable, move-only, with Reset/Get/operators, calls releaser on destruction if value != InvalidValue). Include: `<corelib/utility.hxx>` (all utilities) or `<corelib/utility/type_name.hxx>`, `<corelib/utility/defer.hxx>`, `<corelib/utility/auto_release.hxx>` (specific).

   **casting**: RTTI-free casting utilities inspired by LLVM's casting infrastructure. Uses `classof` static methods for type checking without C++ RTTI. Template metaprogramming with SFINAE, type traits, and recursive type simplification. Main APIs: `isa<T>()` (type checking, variadic support), `cast<T>()` (checked cast with assertion), `dyn_cast<T>()` (dynamic cast returning null on failure). Variants: `isa_and_present<T>()`, `cast_if_present<T>()`, `dyn_cast_if_present<T>()`, `cast_or_null<T>()`, `dyn_cast_or_null<T>()`, `unique_dyn_cast<T>()`, `unique_dyn_cast_or_null<T>()`. Predicate functors: `IsaPred<T>`, `IsaAndPresentPred<T>`, `CastTo<T>`, `DynCastTo<T>`, `CastIfPresentTo<T>`, `DynCastIfPresentTo<T>`, `StaticCastTo<T>`. Supports pointers, references, const-correctness, `std::unique_ptr`, and `std::optional`. Include: `<corelib/casting.hxx>` (all) or `<corelib/casting/isa.hxx>`, `<corelib/casting/cast.hxx>`, `<corelib/casting/dyn_cast.hxx>`, `<corelib/casting/predicates.hxx>`.

4. **logenium** (C++23): GUI framework with RTTI-free type system. Abstract base classes, factory pattern (`Application::Create()`), singleton. Main: `main()` → `Create()` → `Run()`.

   **Build Structure**: Split into `logeniumlib` (library) and `logenium` (executable). `logeniumlib` contains all application/window implementation (PUBLIC interface), `logenium` is thin wrapper with `main()`. This separation enables testing - tests link against `logeniumlib`.

   **RTTI-Free Type System**: Application and Window hierarchies implement LLVM-style casting using `classof` pattern, enabling use of corelib's `isa<T>()`, `cast<T>()`, `dyn_cast<T>()` without C++ RTTI.

   - **ApplicationKind enum**: `AK_Windows`, `AK_Linux`, `AK_LinuxWayland`, `AK_LinuxX11`
   - **WindowKind enum**: `WK_Windows`, `WK_Linux`, `WK_LinuxWayland`, `WK_LinuxX11`
   - **GetKind() method**: Returns kind for runtime type queries
   - **classof() pattern**: All derived classes implement `static bool classof(const Base *)`:
     - WindowsApplication/WindowsWindow: Exact match (`kind == AK_Windows`/`WK_Windows`)
     - LinuxApplication/LinuxWindow: Range check (`kind >= AK_Linux && kind <= AK_LinuxX11`)
     - X11Application/X11Window: Exact match (`kind == AK_LinuxX11`/`WK_LinuxX11`)
     - WaylandApplication/WaylandWindow: Exact match (`kind == AK_LinuxWayland`/`WK_LinuxWayland`)

   **Usage Example**:
   ```cpp
   Application *app = Application::Create().get();
   if (isa<WindowsApplication>(app)) {
       auto *win_app = cast<WindowsApplication>(app);  // Checked cast with assertion
   }
   auto *x11_app = dyn_cast<X11Application>(app);  // Returns nullptr if not X11
   ```

   **Tests**: `tests/` directory with consolidated CMakeLists.txt. Test target `logenium_tests` links against `logeniumlib`, integrated into `all_logenium_tests`.

### Class Hierarchy

```
Application → {Windows,Linux{X11,Wayland}}Application → Creates corresponding Window
Window → {Windows,Linux{X11,Wayland}}Window
```

**NativeHandle**: Wraps platform handles (Windows: HMODULE/HWND, Linux: dlopen void*). Implicit conversions for type safety. Constructors are non-constexpr (constexpr incompatible with virtual base classes).

## Implementation Details

**xheader Conventions**:
- Windows API: A/W variants (ANSI/Wide). Stubs in `#ifndef _WIN32`.
- Linux APIs: Re-export system headers on Linux (`#include <header.h> // IWYU pragma: export`), stubs on non-Linux (`#ifndef __linux__`).
- C23 limitation: Non-null pointer constants use `#define`, not `constexpr`.
- Null pointers: Use `nullptr` instead of `NULL` in C libraries (xheader is C23, supports `nullptr`).

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

**Casting System Details**:

Architecture organized into public API (`include/corelib/casting/`) and detail implementation (`include/corelib/detail/casting/`):

Public API:
- `isa.hxx`: Type checking functions (`isa<T>()`, `isa_and_present<T>()`)
- `cast.hxx`: Checked casting functions (`cast<T>()`, `cast_if_present<T>()`, `cast_or_null<T>()`)
- `dyn_cast.hxx`: Dynamic casting functions (`dyn_cast<T>()`, `dyn_cast_if_present<T>()`, `dyn_cast_or_null<T>()`, `unique_dyn_cast<T>()`)
- `predicates.hxx`: Predicate functors for use with algorithms

Detail implementation (6 consolidated files):
- `forwarding.hxx`: Helper utilities (`NullableValueCastFailed`, `DefaultDoCastIfPossible`, `ForwardToPointerCast`, `ConstStrippingForwardingCast`)
- `traits.hxx`: Type traits (`SimplifyType`, `IsSimpleType`, `ValueIsPresent`, `isPresent()`, `unwrapValue()`, `SelfType`, `IsNullable`)
- `isa.hxx`: Type checking implementation (`IsaImpl`, `IsaImplConst`, `IsaImplWrap`)
- `adapters.hxx`: Type-specific adapters (`UniquePtrCast`, `OptionalValueCast`, `ValueFromPointerCast`)
- `cast.hxx`: Cast infrastructure (`CastRetty`, `CastRettyImpl`, `CastRettyWrap`, `CastIsPossible`, `CastConvertVal`, `CastInfo`)
- `predicates.hxx`: Predicate implementation (`IsaCheckPredicate`, `IsaAndPresentCheckPredicate`, casting functors)

Template pattern: All cast operations use `CastInfo<To, From>` orchestrator → delegates to specialized implementations via `SimplifyType` recursive unwrapping → calls appropriate `DoCast`/`IsPossible`/`CastFailed` methods. CRTP pattern with `SelfType` helper for customization.

Requirements: Types must implement `static bool classof(const BaseType *)` for RTTI-free type checking. Uses `assert()` in debug builds for checked casts.

Usage notes:
- **Template parameters**: Never include reference qualifiers in cast template parameters. Use `cast<T>(val)` not `cast<T &>(val)`. The cast functions automatically return the correct reference/pointer type based on the input.
- When using `type_name<T>` in `debug::Assert` calls within casting functions, always include the function call parentheses: `type_name<T>()` not `type_name<T>`. The function template must be called to return `std::string_view` for proper template argument deduction.

## Code Style

- Files: `.hxx`/`.cxx` (C++), `.h`/`.c` (C). Platform subdirs: `platform/{windows,linux}/{X11,wayland}/`
- Include guards: `#ifndef LOGENIUM_PATH_TO_FILE_HXX`
- Platform: `#if defined(_WIN32)`, `#ifdef __linux__`
- Assertions: Use `debug::Assert` instead of `<cassert>` for better diagnostics
- **No documentation comments** unless explicitly requested. Self-documenting code only.
- Include order (clang-format): logenium(9) > corelib(8) > debug(7) > xheader(6) > fmt(5) > gtest/gmock(4) > system(3) > stdlib(2) > other(1). 120 col, Google style, 4-space, right-aligned pointers (`int *ptr`).
- CMake: Custom logging (`log_info`, `log_status`, `log_warning`, `log_error`). Includes: module name only at root, full path in subdirs. CMakeLists.txt structure: include options.cmake before dependencies.cmake (allows dependencies to use option values).

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

**Windows API**: 1) Add to `windows.h`, 2) Create `src/windows/function_name_{a,w}.c` (or just `.c`), 3) Add to CMakeLists, 4) Create tests in matching directory (auto-discovered by `GLOB_RECURSE`), 5) Add UNICODE macros if needed.

**Linux dlfcn**: 1) Add to `dlfcn.h` with `#ifdef __linux__` re-export, 2) Create `src/dlfcn/function_name.c` with `#ifndef __linux__` stub, 3) Add to CMakeLists, 4) Create tests in matching directory (auto-discovered by `GLOB_RECURSE`).

**Linux XCB**: 1) Choose header (xcb.h or xproto.h), 2) Add with `#ifdef __linux__` re-export, 3) Create `src/xcb/{xcb,xproto}/function_name.c` with `#ifndef __linux__` stub, 4) Add to CMakeLists, 5) Create tests in matching directory (auto-discovered by `GLOB_RECURSE`).

**Types**: Handle types: `struct HFOO__ { int dummy; }; typedef struct HFOO__ *HFOO;`. Constants: `constexpr` for numeric, `#define` for non-null pointers.

## Testing

**GoogleTest v1.17.0** via FetchContent. xheader: C23 lib with C++ tests (declares `LANGUAGES C CXX`).

**Organization**: Consolidated test structure with single CMakeLists.txt per library. Uses `file(GLOB_RECURSE ... CONFIGURE_DEPENDS *.cxx)` to automatically collect all test sources recursively. Test subdirectories no longer have individual CMakeLists.txt files.

**Targets**:
1. Per-library: `xheader_tests`, `debug_tests`, `corelib_tests`, `logenium_tests` (consolidated from all test subdirectories)
2. Project-wide: `all_logenium_tests`

**Test Structure**:
- **libs/corelib/tests/casting/**: Public API tests organized by functionality
  - `common.hxx`: Shared TestShape hierarchy fixture
  - **isa/**: `isa.cxx` (type checking), `isa_and_present.cxx` (null-safe type checking)
  - **cast/**: `cast.cxx` (checked casts), `cast_if_present.cxx`, `cast_or_null.cxx`
  - **dyn_cast/**: `dyn_cast.cxx` (dynamic casts), `dyn_cast_if_present.cxx`, `dyn_cast_or_null.cxx`, `unique_dyn_cast.cxx`, `unique_dyn_cast_or_null.cxx`
  - **predicates/**: `predicates.cxx` (IsaPred, IsaAndPresentPred, StaticCastTo, CastTo, CastIfPresentTo, DynCastIfPresentTo, DynCastTo functors)

- **libs/corelib/tests/detail/casting/**: Implementation detail tests
  - `common.hxx`: Re-exports casting/common.hxx
  - **traits/**: `traits.cxx` (SimplifyType, IsSimpleType, ValueIsPresent, isPresent(), unwrapValue(), SelfType, IsNullable)
  - **forwarding/**: `forwarding.cxx` (NullableValueCastFailed, DefaultDoCastIfPossible)
  - **isa/**: `isa.cxx` (IsaImpl, IsaImplConst specializations)
  - **cast/**: `cast.cxx` (CastRetty infrastructure)
  - **predicates/**: `predicates.cxx` (IsaCheckPredicate, CastFunc, DynCastFunc)

- **libs/corelib/tests/type_traits/**: Type trait tests
  - **add_const_past_pointer/**: `add_const_past_pointer.cxx`
  - **add_lvalue_reference_if_not_pointer/**: `add_lvalue_reference_if_not_pointer.cxx`
  - **const_pointer_or_const_ref/**: `const_pointer_or_const_ref.cxx`
  - **is_integral_or_enum/**: `is_integral_or_enum.cxx`

- **tests/**: Logenium framework tests (85 tests)
  - **application/**: `application.cxx` (42 Application tests using mock implementations)
  - **window/**: `window.cxx` (43 Window tests using mock implementations)

**Coverage**:
- xheader: Windows API, dlfcn stubs
- debug: Assert, Breakpoint, IsDebuggerPresent
- corelib/utility: type_name (template/value overloads, cv-qualifiers, constexpr evaluation), defer (RAII wrapper, scope-exit execution, LIFO ordering, exception safety, move semantics), auto_release (resource manager, custom releasers, Reset/Get/operators, move-only semantics, invalid value handling, pointer/string/custom types)
- corelib/casting (public API): isa/isa_and_present (type checking with pointers/references/unique_ptr/const-correctness, variadic support), cast (reference/pointer downcasts, const preservation), dyn_cast (successful/failed casts with const support, unique_ptr ownership transfer), cast_if_present/dyn_cast_if_present (null-safe casting), cast_or_null/dyn_cast_or_null (nullable variants), unique_dyn_cast/unique_dyn_cast_or_null (unique_ptr specialized casting), predicates (IsaPred, IsaAndPresentPred, StaticCastTo, CastTo, CastIfPresentTo, DynCastIfPresentTo, DynCastTo functors)
- corelib/detail/casting (implementation): Type simplification (SimplifyType, IsSimpleType), nullable detection (IsNullable), value presence (ValueIsPresent, isPresent(), unwrapValue()), forwarding strategies, type-checking implementation, cast infrastructure
- corelib/type_traits: add_const_past_pointer (const propagation past pointers), add_lvalue_reference_if_not_pointer (conditional reference addition), const_pointer_or_const_ref (const pointer/reference selection), is_integral_or_enum (integral/enum detection)
- logenium: Application and Window RTTI-free casting system
  - **Enum values**: ApplicationKind/WindowKind enum value verification
  - **GetKind()**: Kind retrieval for all platform variants (Windows, Linux, X11, Wayland)
  - **classof()**: RTTI-free type checking with exact match (Windows/X11/Wayland) and range checks (Linux base)
  - **corelib integration**: Full casting API coverage (isa, cast, dyn_cast, cast_or_null, dyn_cast_or_null, isa_and_present, cast_if_present, dyn_cast_if_present) with variadic support and const preservation
  - **NativeHandle**: Default/void*/nullptr/platform-specific constructors, conversions, XCB window round-trip with std::bit_cast
  - **State**: Default values and modification (Application only)
  - **Predicates**: IsaPred, IsaAndPresentPred, CastTo, DynCastTo functors
  - **Mock implementations**: Lightweight mock classes for testing without platform dependencies, respects singleton pattern

Test files: `.cxx`.

**Options**: `LOGENIUM_BUILD_TESTS` (master, ON), `LOGENIUM_XHEADER_BUILD_TESTS` (ON), `LOGENIUM_DEBUG_BUILD_TESTS` (ON), `LOGENIUM_CORELIB_BUILD_TESTS` (ON).

**Logenium Test Implementation Notes**:
- **No `#define private public`**: MSVC STL forbids macroizing keywords. Tests use public API only via mock implementations.
- **Mock Classes**: Five mock classes per hierarchy (MockApplication, MockWindowsApplication, MockLinuxApplication, MockX11Application, MockWaylandApplication) with minimal implementations (empty Run(), RegisterWindowClass(), UnregisterWindowClass()). Each implements `classof()` matching production behavior.
- **Singleton Pattern**: Application uses static instance pointer. Tests respect singleton by ensuring only one Application exists at a time. Destructor resets instance to nullptr. Tests requiring multiple types use scoped blocks: `{ MockWindows app; test(&app); } { MockLinux app; test(&app); }`.
- **Template Macro Workarounds**: Variadic templates in EXPECT_TRUE/EXPECT_FALSE require parentheses: `EXPECT_TRUE((isa<A, B>(ptr)))`. Variable templates (IsaPred, CastTo) use `auto`: `auto pred = IsaPred<T>;`.
- **XCB Handle Testing**: Window::NativeHandle XCB conversions tested with round-trip using `std::bit_cast<void *>(static_cast<std::uintptr_t>(xcb_window))`.

**Run**: `cmake -B build -G Ninja -DLOGENIUM_BUILD_TESTS=ON; cmake --build build; ctest --test-dir build`

## Limitations

**Windows**: DefWindowProc only, no custom event handling, fixed window size, single window.
**Linux**: X11/Wayland: basic window creation (1280x720), event loop exists, minimal event processing (Wayland: configure/close only), single window, static buffer (Wayland gray fill).
