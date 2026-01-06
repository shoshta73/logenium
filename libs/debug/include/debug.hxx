// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_DEBUG_HXX
#define LOGENIUM_DEBUG_HXX

/**
 * @file debug.hxx
 * @brief Convenience header for all debug module components
 *
 * This header includes all debug utilities: breakpoints, assertions, and debugger
 * detection. Include this file to get access to all debug features at once.
 */

/**
 * @defgroup debug Debug Module
 * @brief Debug utilities for development and diagnostics
 *
 * The debug module provides essential debugging tools that help during development:
 * - **Breakpoints**: Trigger debugger breakpoints for inspection
 * - **Assertions**: Runtime checks with detailed diagnostics and stack traces
 * - **Debugger detection**: Check if running under a debugger
 *
 * All debug utilities are designed to have minimal or zero overhead in release builds:
 * - Breakpoint() becomes a consteval no-op in release mode (NDEBUG defined)
 * - Assert() checks are completely removed in release builds
 * - IsDebuggerPresent() remains available in all builds for runtime behavior changes
 *
 * ## Components
 *
 * ### Breakpoints
 * - `Breakpoint()`: Unconditionally trigger a debugger breakpoint (debug builds only)
 * - `BreakpointIfDebugging()`: Conditionally break only if debugger is attached
 *
 * ### Assertions
 * - `Assert()`: Runtime checks with formatted messages and stack traces
 *
 * ### Debugger Detection
 * - `IsDebuggerPresent()`: Check if the process is running under a debugger
 *
 * ## Usage
 *
 * Include the convenience header to get all debug utilities:
 * @code
 * #include <debug.hxx>
 * @endcode
 *
 * Or include specific headers:
 * @code
 * #include <debug/breakpoint.hxx>
 * #include <debug/assert.hxx>
 * #include <debug/is_debugger_present.hxx>
 * @endcode
 */

#include "debug/assert.hxx"                   // IWYU pragma: export
#include "debug/breakpoint.hxx"               // IWYU pragma: export
#include "debug/breakpoint_if_debugging.hxx"  // IWYU pragma: export
#include "debug/is_debugger_present.hxx"      // IWYU pragma: export

#endif  // LOGENIUM_DEBUG_HXX
