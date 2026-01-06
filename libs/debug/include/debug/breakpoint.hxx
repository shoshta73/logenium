// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_DEBUG_BREAKPOINT_HXX
#define LOGENIUM_DEBUG_BREAKPOINT_HXX

namespace debug {

#ifndef NDEBUG

/**
 * @ingroup debug
 * @brief Triggers a debugger breakpoint unconditionally (debug builds only).
 *
 * When called in debug builds, this function halts execution and transfers control
 * to an attached debugger at the point of invocation. This allows developers to
 * inspect program state, variables, and call stacks interactively.
 *
 * The implementation uses platform-specific mechanisms to trigger the breakpoint:
 * - **Clang/GCC**: `__builtin_debugtrap()` (preferred, generates optimal code)
 * - **MSVC**: `__debugbreak()` intrinsic
 * - **Windows (fallback)**: `DebugBreak()` Windows API function
 * - **POSIX (fallback)**: `std::raise(SIGTRAP)` signal
 *
 * @note In release builds (when NDEBUG is defined), this function becomes a
 *       consteval no-op that compiles away completely, resulting in zero runtime
 *       overhead.
 *
 * @note If no debugger is attached when this function is called, the behavior
 *       depends on the platform:
 *       - **Windows**: The system displays a "Just-In-Time Debugging" dialog
 *       - **POSIX**: The process receives SIGTRAP and may terminate if not handled
 *
 * @note For conditional breakpoints that only trigger when a debugger is attached,
 *       use `BreakpointIfDebugging()` instead.
 *
 * Example usage:
 * @code
 * void investigate_bug(int value) {
 *     if (value < 0) {
 *         // Always break here in debug builds to inspect why value is negative
 *         debug::Breakpoint();
 *     }
 *     // ... rest of function ...
 * }
 * @endcode
 *
 * @code
 * // Example: Break at a specific iteration during debugging
 * for (int i = 0; i < 1000; ++i) {
 *     if (i == 42) {
 *         debug::Breakpoint();  // Inspect state at iteration 42
 *     }
 *     process(i);
 * }
 * @endcode
 *
 * @see BreakpointIfDebugging() for conditional breakpoints
 * @see IsDebuggerPresent() to check if a debugger is attached before breaking
 */
void Breakpoint();

#else

/**
 * @ingroup debug
 * @brief No-op version of Breakpoint() for release builds.
 *
 * In release builds (when NDEBUG is defined), this function is a consteval no-op
 * that compiles away completely. The compiler eliminates all calls at compile time,
 * resulting in zero runtime overhead and no code generation.
 *
 * @note This is a consteval function, meaning it's evaluated at compile time and
 *       produces no runtime code whatsoever.
 */
consteval void Breakpoint() {}

#endif

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_BREAKPOINT_HXX
