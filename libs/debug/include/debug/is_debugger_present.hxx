// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_DEBUG_IS_DEBUGGER_PRESENT_HXX
#define LOGENIUM_DEBUG_IS_DEBUGGER_PRESENT_HXX

namespace debug {

#ifndef NDEBUG

/**
 * @ingroup debug
 * @brief Checks whether the process is currently running under a debugger.
 *
 * This function detects if a debugger is attached to the current process at the
 * time of invocation. It can be used to conditionally enable debugging features,
 * modify behavior for better debugging experience, or trigger breakpoints only
 * when actively debugging.
 *
 * The implementation uses platform-specific mechanisms:
 * - **Windows**: Calls the Windows API `IsDebuggerPresent()` function, which checks
 *   the PEB (Process Environment Block) `BeingDebugged` flag
 * - **Linux/POSIX**: Currently returns `false` (stub implementation)
 *
 * @return `true` if a debugger is currently attached to the process, `false` otherwise.
 *
 * @note In release builds (when NDEBUG is defined), this function becomes a
 *       consteval function that always returns `false`, allowing the compiler to
 *       optimize away debugging-specific code paths completely.
 *
 * @note The result reflects the debugger state at the moment of the call. A debugger
 *       can attach or detach at any time, so the result may change between calls.
 *
 * @note On Windows, this function is reliable and accurate. On Linux/POSIX platforms,
 *       the current implementation is a stub and always returns `false`. Full support
 *       for Linux debugger detection may be added in future versions.
 *
 * Example usage:
 * @code
 * void log_message(const char *msg) {
 *     if (debug::IsDebuggerPresent()) {
 *         // When debugging, output detailed diagnostic information
 *         std::println(stderr, "[DEBUG] {}: {}", __func__, msg);
 *     } else {
 *         // In normal execution, use standard logging
 *         std::println("{}", msg);
 *     }
 * }
 * @endcode
 *
 * @code
 * // Example: Conditional breakpoint based on debugger presence
 * void process_critical_data(const Data &data) {
 *     if (!validate(data)) {
 *         // Only break if we're actually debugging
 *         if (debug::IsDebuggerPresent()) {
 *             debug::Breakpoint();
 *         } else {
 *             // Log error and continue in production
 *             log_error("Invalid data detected");
 *         }
 *     }
 * }
 * @endcode
 *
 * @code
 * // Example: Enable verbose tracing only when debugging
 * class Component {
 *     bool verbose_trace = debug::IsDebuggerPresent();
 * public:
 *     void process() {
 *         if (verbose_trace) {
 *             std::println("Processing step 1...");
 *         }
 *         // ... processing logic ...
 *     }
 * };
 * @endcode
 *
 * @see BreakpointIfDebugging() which combines this check with Breakpoint()
 */
bool IsDebuggerPresent();

#else

/**
 * @ingroup debug
 * @brief Consteval version of IsDebuggerPresent() for release builds.
 *
 * In release builds (when NDEBUG is defined), this function is a consteval function
 * that always returns `false`. This allows the compiler to completely optimize away
 * any debugging-specific code paths at compile time, resulting in zero runtime overhead.
 *
 * @return Always returns `false` in release builds.
 *
 * @note This is a consteval function, meaning the return value is a compile-time
 *       constant. Conditional code based on this function will be completely eliminated
 *       by the compiler in release builds.
 */
consteval bool IsDebuggerPresent() { return false; }

#endif

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_IS_DEBUGGER_PRESENT_HXX
