// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_DEBUG_ASSERT_HXX
#define LOGENIUM_DEBUG_ASSERT_HXX

#ifndef NDEBUG

#include <cstdlib>
#include <source_location>
#include <stacktrace>
#include <string>
#include <utility>

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>

#include <debug/breakpoint.hxx>
#include <debug/is_debugger_present.hxx>
#include <debug/tracing/macros.hxx>

#if !__LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

#include <iterator>

#endif  // !__LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

#endif  // NDEBUG

namespace debug {

/**
 * @defgroup debug-internal Internal Implementation
 * @ingroup debug
 * @brief Internal implementation details for the debug module
 *
 * This group contains implementation details that users should not directly depend on.
 * All public APIs are exposed through the parent debug namespace via template aliases
 * and wrapper functions.
 */

namespace detail {

/**
 * @ingroup debug-internal
 * @brief Implementation class for runtime assertions with formatted messages and diagnostics.
 *
 * This is the internal implementation class that powers the `debug::Assert` public API.
 * Users should not instantiate this class directly; instead, use the `debug::Assert`
 * template alias.
 *
 * In debug builds (NDEBUG not defined), the constructor:
 * 1. Evaluates the predicate condition
 * 2. If false, prints formatted assertion message, source location, and stack trace
 * 3. Calls `BreakpointIfDebugging()` to break if a debugger is attached
 * 4. Calls `std::abort()` to terminate the program
 *
 * The implementation supports multiple build configurations:
 * - **Standard library** (`__LOGENIUM_DEBUG_USE_FMTLIB__ == 0`): Uses `std::format` and `std::println`
 * - **fmtlib** (`__LOGENIUM_DEBUG_USE_FMTLIB__ == 1`): Uses `fmt::format` and `fmt::println`
 * - **Colored output** (`__LOGENIUM_DEBUG_USE_COLOR_LOGS__ == 1`, requires fmtlib): Highlights
 *   "Assertion failed" with red background and white text
 * - **Fast stacktrace** (`__LOGENIUM_DEBUG_USE_FAST_STACKTRACE__ == 1`): Prints stacktrace
 *   as-is instead of in reverse order
 *
 * In release builds (NDEBUG defined), the constructor is constexpr and does nothing,
 * allowing the compiler to completely eliminate assertion checks and formatting overhead.
 *
 * @tparam Args Variadic template parameters for format arguments
 *
 * @note This is an implementation detail. Use `debug::Assert` instead.
 *
 * @see Assert for the public API
 */
template <typename... Args>
struct AssertImpl {
#ifndef NDEBUG

    /**
     * @brief Constructor for debug builds using fmtlib formatting.
     *
     * Evaluates the predicate and, if false, prints diagnostic information and aborts.
     * This version uses fmtlib for formatting and optionally colored output.
     *
     * @param predicate The condition to check
     * @param format Format string (fmt::format_string)
     * @param args Arguments for the format string
     * @param location Source location (automatically captured)
     * @param stacktrace Stack trace (automatically captured)
     *
     * Diagnostic output includes:
     * - Formatted assertion message (with red background if __LOGENIUM_DEBUG_USE_COLOR_LOGS__)
     * - Source file, line, and function name
     * - Full stack trace (reversed for readability, unless __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__)
     *
     * If predicate is false:
     * 1. Prints diagnostics to stderr (with color if enabled)
     * 2. Calls IsDebuggerPresent()
     * 3. Calls Breakpoint() if a debugger is attached (otherwise calls std::abort())
     */
    AssertImpl(bool predicate, fmt::format_string<Args...> format, Args &&...args,
               std::source_location location = std::source_location::current(),
               const std::stacktrace &stacktrace = std::stacktrace::current()) {
        ZoneScoped;
        if (!predicate) {
            ZoneScopedN("Assertion handler");

            fmt::println("{}: {}",
                         fmt::styled("Assertion failed", fmt::bg(fmt::color::red) | fmt::fg(fmt::color::white)),
                         fmt::format(format, std::forward<Args>(args)...));

            fmt::println("Location: {}:{} in {}", location.file_name(), location.line(), location.function_name());

#if __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

            fmt::println("Stacktrace:\n{}", std::to_string(stacktrace));

#else

            fmt::println("Stacktrace:");
            auto frame_count = std::distance(stacktrace.crbegin(), stacktrace.crend());
            for (auto frame = stacktrace.crbegin(); frame != stacktrace.crend(); ++frame) {
                fmt::println("  {}: {}", frame_count--, std::to_string(*frame));
            }

#endif  // __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

            if (IsDebuggerPresent()) {
                Breakpoint();
            } else {
                std::abort();
            }
        }
    }

#else

    /**
     * @brief Constexpr no-op constructor for release builds (fmt::format version).
     *
     * In release builds, this constructor is constexpr and does nothing. The compiler
     * completely eliminates assertion checks and all formatting overhead.
     *
     * @param predicate Ignored
     * @param format Ignored
     * @param args Ignored
     */
    constexpr AssertImpl(bool predicate, fmt::format_string<Args...> format, Args &&...args) {}

#endif  // NDEBUG
};

/**
 * @ingroup debug-internal
 * @brief Deduction guide for fmt::format_string (fmtlib version).
 */
template <typename... Args>
AssertImpl(bool, fmt::format_string<Args...>, Args &&...) -> AssertImpl<Args...>;

}  // namespace detail

/**
 * @ingroup debug
 * @brief Runtime assertion with formatted messages, source location, and stack traces.
 *
 * Assert provides enhanced runtime checking with detailed diagnostic output. Unlike
 * standard `<cassert>`, it supports formatted messages, automatically captures source
 * location and stack traces, and can optionally break into the debugger before aborting.
 *
 * ## Usage
 *
 * Assertions are created using brace initialization syntax:
 * @code
 * debug::Assert{condition, "format string", args...};
 * @endcode
 *
 * The assertion checks `condition` and, if false:
 * 1. Prints the formatted message with source location and stack trace
 * 2. Calls `BreakpointIfDebugging()` to break if a debugger is attached
 * 3. Calls `std::abort()` to terminate the program
 *
 * ## Build Configurations
 *
 * The implementation adapts to different build configurations:
 *
 * ### Debug vs Release
 * - **Debug builds** (`NDEBUG` not defined): Full assertion checking with diagnostics
 * - **Release builds** (`NDEBUG` defined): Constexpr no-op, completely optimized away
 *
 * ### Formatting Library
 * - **Standard library** (default): Uses `std::format` and `std::println`
 * - **fmtlib** (`LOGENIUM_DEBUG_USE_FMTLIB=ON`): Uses `fmt::format` and `fmt::println`
 *
 * ### Colored Output
 * - **Disabled** (default): Plain text output
 * - **Enabled** (`LOGENIUM_DEBUG_USE_COLOR_LOGS=ON`, requires fmtlib): "Assertion failed"
 *   highlighted with red background and white text
 *
 * ### Stack Trace Format
 * - **Reversed** (default): Stack frames printed in reverse order (most recent last)
 * - **Fast** (`LOGENIUM_DEBUG_USE_FAST_STACKTRACE=ON`): Stack frames in natural order
 *
 * ## Examples
 *
 * @code
 * // Simple assertion with message
 * void process(int *ptr) {
 *     debug::Assert{ptr != nullptr, "Null pointer passed to process()"};
 *     // ... use ptr safely ...
 * }
 * @endcode
 *
 * @code
 * // Assertion with formatted message
 * void set_value(int value) {
 *     debug::Assert{value >= 0 && value <= 100,
 *                   "Value {} out of range [0, 100]", value};
 *     // ... proceed with valid value ...
 * }
 * @endcode
 *
 * @code
 * // Complex condition with detailed diagnostics
 * void transfer(Account &from, Account &to, double amount) {
 *     debug::Assert{from.balance >= amount,
 *                   "Insufficient funds: account {} has ${:.2f}, need ${:.2f}",
 *                   from.id, from.balance, amount};
 *     // ... perform transfer ...
 * }
 * @endcode
 *
 * @code
 * // Mathematical invariant checking
 * int divide(int a, int b) {
 *     debug::Assert{b != 0, "Division by zero: {} / {}", a, b};
 *     return a / b;
 * }
 * @endcode
 *
 * ## Why Use Assert Over <cassert>?
 *
 * - **Formatted messages**: Use modern formatting instead of plain strings
 * - **Automatic context**: Source location captured without manual `__FILE__`/`__LINE__`
 * - **Stack traces**: See the full call chain leading to the failure
 * - **Debugger integration**: Automatically breaks if a debugger is attached
 * - **Better diagnostics**: Clear, formatted output helps debug issues faster
 *
 * @tparam Args Variadic template parameters for format arguments (deduced automatically)
 *
 * @note Always prefer `debug::Assert` over `<cassert>` in this codebase for better
 *       diagnostics and debugger integration.
 *
 * @note In release builds, assertions compile away completely with zero overhead.
 *
 * @note Source location and stack traces are captured automatically at the assertion
 *       site, not at the AssertImpl constructor site.
 *
 * @see BreakpointIfDebugging() for the automatic debugger breaking behavior
 * @see detail::AssertImpl for implementation details
 */
template <typename... Args>
using Assert = detail::AssertImpl<Args...>;

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_ASSERT_HXX
