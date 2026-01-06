// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_DEBUG_BREAKPOINT_IF_DEBUGGING_HXX
#define LOGENIUM_DEBUG_BREAKPOINT_IF_DEBUGGING_HXX

namespace debug {

#ifndef NDEBUG

/**
 * @ingroup debug
 * @brief Triggers a debugger breakpoint only if a debugger is currently attached.
 *
 * This is a convenience function that combines `IsDebuggerPresent()` and `Breakpoint()`
 * to provide safe, conditional breakpoints. It checks if a debugger is attached and only
 * triggers a breakpoint if one is present, avoiding unwanted behavior when running without
 * a debugger.
 *
 * In debug builds, this function:
 * 1. Calls `IsDebuggerPresent()` to check for an attached debugger
 * 2. If a debugger is present, calls `Breakpoint()` to halt execution
 * 3. If no debugger is attached, returns immediately without side effects
 *
 * This is particularly useful for:
 * - **Assertion-like breakpoints**: Break on unexpected conditions during debugging, but
 *   allow normal execution in production or when running tests
 * - **Safe breakpoints in automated tests**: Place breakpoints that won't crash test runs
 * - **Conditional debugging**: Break at specific code points only when actively debugging
 * - **Development convenience**: Leave debugging breakpoints in code that won't affect
 *   non-debugging workflows
 *
 * @note In release builds (when NDEBUG is defined), this function becomes a consteval
 *       no-op that compiles away completely, resulting in zero runtime overhead.
 *
 * @note Unlike `Breakpoint()`, this function is safe to call in any context (tests,
 *       production debug builds, etc.) because it only breaks when a debugger is actually
 *       attached.
 *
 * @note On platforms where `IsDebuggerPresent()` always returns `false` (currently Linux/POSIX),
 *       this function will never trigger a breakpoint, effectively becoming a no-op.
 *
 * Example usage:
 * @code
 * void process_transaction(const Transaction &txn) {
 *     if (!txn.is_valid()) {
 *         // Break here during development to inspect invalid transactions,
 *         // but allow tests and production to continue
 *         debug::BreakpointIfDebugging();
 *
 *         // Handle error gracefully
 *         log_error("Invalid transaction");
 *         return;
 *     }
 *     // ... process transaction ...
 * }
 * @endcode
 *
 * @code
 * // Example: Debug a specific iteration without affecting test runs
 * for (size_t i = 0; i < items.size(); ++i) {
 *     if (i == 42) {
 *         // When debugging, break at iteration 42 to inspect state.
 *         // When running tests, this does nothing.
 *         debug::BreakpointIfDebugging();
 *     }
 *     process(items[i]);
 * }
 * @endcode
 *
 * @code
 * // Example: Safe assertion-like breakpoint for development
 * class Cache {
 * public:
 *     void insert(const Key &key, const Value &value) {
 *         if (map.contains(key)) {
 *             // This shouldn't happen, but if it does during debugging,
 *             // break so we can investigate. In tests/production, continue.
 *             debug::BreakpointIfDebugging();
 *         }
 *         map[key] = value;
 *     }
 * private:
 *     std::unordered_map<Key, Value> map;
 * };
 * @endcode
 *
 * @see Breakpoint() for unconditional breakpoints
 * @see IsDebuggerPresent() to check debugger state without breaking
 */
void BreakpointIfDebugging();

#else

/**
 * @ingroup debug
 * @brief No-op version of BreakpointIfDebugging() for release builds.
 *
 * In release builds (when NDEBUG is defined), this function is a consteval no-op
 * that compiles away completely. The compiler eliminates all calls at compile time,
 * resulting in zero runtime overhead and no code generation.
 *
 * @note This is a consteval function, meaning it's evaluated at compile time and
 *       produces no runtime code whatsoever.
 */
consteval void BreakpointIfDebugging() {}

#endif

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_BREAKPOINT_IF_DEBUGGING_HXX
