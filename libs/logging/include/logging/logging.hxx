// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_LOGGING_LOGGING_HXX
#define LOGENIUM_LOGGING_LOGGING_HXX

/**
 * @file logging.hxx
 * @brief Header-only logging utility with compile-time format string validation
 *
 * This header provides a simple, type-safe logging interface with compile-time
 * format string validation. The logging backend can use either C++20 standard
 * library formatting (std::format/std::println) or fmtlib (fmt::format/fmt::println)
 * based on the LOGENIUM_LOGGING_USE_FMTLIB CMake option.
 */

/**
 * @defgroup logging Logging Module
 * @brief Simple, type-safe logging with compile-time format string validation
 *
 * The logging module provides a header-only logging utility that wraps format
 * string validation and output to stdout. It supports two backends:
 *
 * - **Standard library** (default when LOGENIUM_LOGGING_USE_FMTLIB=OFF):
 *   Uses std::format_string, std::println from C++20 <format> and <print>
 *
 * - **fmtlib** (when LOGENIUM_LOGGING_USE_FMTLIB=ON):
 *   Uses fmt::format_string, fmt::println from fmtlib v12.1.0
 *
 * ## Features
 *
 * - **Compile-time validation**: Format strings are checked at compile time
 * - **Type safety**: Arguments are type-checked against the format string
 * - **Source location tracking**: Automatically captures and displays file, line, and function
 * - **Level-based logging**: Supports 10 severity levels from Trace3 to Fatal
 * - **Perfect forwarding**: Arguments are forwarded without unnecessary copies
 * - **Zero overhead**: Template-based design with no runtime cost
 *
 * ## Usage
 *
 * Basic logging with automatic type deduction and level selection:
 * @code
 * #include <logging/logging.hxx>
 *
 * // Default Info level
 * logging::log("Hello, {}!", "world");
 * // Output: [Info] Hello, world! (file.cxx:42 in main)
 *
 * // Level-specific logging
 * logging::debug("Debug value: {}", value);
 * logging::warn("Warning: {}", condition);
 * logging::error("Error occurred: {}", error_msg);
 * logging::fatal("Fatal error: {}", critical_error);
 *
 * // All levels: trace3, trace2, trace1, trace, debug, info, warn, error, fatal
 * @endcode
 *
 * ## Output Format
 *
 * All log messages follow this format:
 * @code
 * [Level] message (file:line in function)
 * @endcode
 *
 * ## Implementation Notes
 *
 * The logging module is header-only and uses template metaprogramming to provide
 * compile-time format string validation. The actual formatting backend is selected
 * at compile time based on the __LOGENIUM_LOGGING_USE_FMTLIB__ preprocessor macro.
 *
 * Source location is automatically captured via std::source_location::current()
 * and displayed in every log message, providing precise debugging information.
 */

#include <source_location>

#include <debug/tracing/macros.hxx>

#if __LOGENIUM_LOGGING_USE_FMTLIB__

#include <fmt/base.h>
#include <fmt/format.h>

#if __LOGENIUM_LOGGING_USE_COLOR_LOGS__

#include <fmt/color.h>

#endif  // __LOGENIUM_LOGGING_USE_COLOR_LOGS__

#else

#include <format>
#include <print>

#endif

#include <logging/level.hxx>

#if __LOGENIUM_LOGGING_USE_COLOR_LOGS__

#include <logging/internal/utils/to_color.hxx>

#endif  // __LOGENIUM_LOGGING_USE_COLOR_LOGS__

namespace logging {

/**
 * @defgroup logging_internal Logging Internal Implementation
 * @ingroup logging
 * @brief Internal implementation details for the logging module
 *
 * This namespace contains implementation details that should not be used
 * directly by users. Use the public API provided by the logging::log family
 * of type aliases instead.
 *
 * @note These are internal implementation details and may change without notice
 */
namespace detail {

/**
 * @ingroup logging_internal
 * @brief Implementation struct for compile-time validated logging
 *
 * This struct provides the actual logging implementation with compile-time
 * format string validation. It uses CTAD (Class Template Argument Deduction)
 * to automatically deduce template arguments from constructor parameters.
 *
 * @tparam L The logging level for this log message
 * @tparam Args Variadic template parameter pack for format arguments
 *
 * @note This is an implementation detail. Users should use the logging::log
 *       family of alias templates instead of instantiating LogImpl directly.
 */
template <Level L, typename... Args>
struct LogImpl {
#if __LOGENIUM_LOGGING_USE_FMTLIB__

    /**
     * @brief Construct and execute a log operation using fmtlib
     *
     * Validates the format string at compile time and outputs the formatted
     * message to stdout using fmt::println with the format:
     * `[Level] message (file:line in function)`
     *
     * @param format Format string with compile-time validation (fmt::format_string)
     * @param args Variadic arguments to be formatted (forwarded)
     * @param location Source location for debugging (automatically captured)
     *
     * @note Arguments are perfectly forwarded to avoid unnecessary copies
     * @note Source location is automatically captured and displayed in output
     */
    LogImpl(fmt::format_string<Args...> format, Args &&...args,
            std::source_location location = std::source_location::current()) {
        ZoneScoped;

#if __LOGENIUM_LOGGING_USE_COLOR_LOGS__

        auto level_string = fmt::format(internal::utils::ToColor(L), "{}", L);

#else

        auto level_string = fmt::format("{}", L);

#endif

        fmt::println("[{}] {} ({}:{} in {})", level_string, fmt::format(format, std::forward<Args>(args)...),
                     location.file_name(), location.line(), location.function_name());
    }

#else

    /**
     * @brief Construct and execute a log operation using standard library
     *
     * Validates the format string at compile time and outputs the formatted
     * message to stdout using std::println with the format:
     * `[Level] message (file:line in function)`
     *
     * @param format Format string with compile-time validation (std::format_string)
     * @param args Variadic arguments to be formatted (forwarded)
     * @param location Source location for debugging (automatically captured)
     *
     * @note Arguments are perfectly forwarded to avoid unnecessary copies
     * @note Source location is automatically captured and displayed in output
     */
    LogImpl(std::format_string<Args...> format, Args &&...args,
            std::source_location location = std::source_location::current()) {
        ZoneScoped;
        std::println("[{}] {} ({}:{} in {})", L, std::format(format, std::forward<Args>(args)...), location.file_name(),
                     location.line(), location.function_name());
    }

#endif
};

#if __LOGENIUM_LOGGING_USE_FMTLIB__

/**
 * @ingroup logging_internal
 * @brief Deduction guide for LogImpl with fmtlib backend
 *
 * Enables CTAD (Class Template Argument Deduction) to automatically deduce
 * template parameters from constructor arguments when using fmtlib.
 *
 * @tparam Args Deduced argument types from constructor
 */
template <Level L = Level::Ignore, typename... Args>
LogImpl(bool, fmt::format_string<Args...>, Args &&...) -> LogImpl<L, Args...>;

#else

/**
 * @ingroup logging_internal
 * @brief Deduction guide for LogImpl with standard library backend
 *
 * Enables CTAD (Class Template Argument Deduction) to automatically deduce
 * template parameters from constructor arguments when using standard library.
 *
 * @tparam Args Deduced argument types from constructor
 */
template <Level L = Level::Ignore, typename... Args>
LogImpl(bool, std::format_string<Args...>, Args &&...) -> LogImpl<L, Args...>;

#endif

}  // namespace detail

/**
 * @ingroup logging
 * @brief Type alias for Trace3-level logging (most verbose trace)
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using trace3 = detail::LogImpl<Level::Trace3, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Trace2-level logging (detailed trace)
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using trace2 = detail::LogImpl<Level::Trace2, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Trace1-level logging (basic trace)
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using trace1 = detail::LogImpl<Level::Trace1, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Trace-level logging
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using trace = detail::LogImpl<Level::Trace, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Debug-level logging
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using debug = detail::LogImpl<Level::Debug, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Info-level logging (same as log)
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using info = detail::LogImpl<Level::Info, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Warn-level logging
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using warn = detail::LogImpl<Level::Warn, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Error-level logging
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using error = detail::LogImpl<Level::Error, Args...>;

/**
 * @ingroup logging
 * @brief Type alias for Fatal-level logging (highest severity)
 * @tparam Args Variadic template parameter pack for format arguments
 */
template <typename... Args>
using fatal = detail::LogImpl<Level::Fatal, Args...>;

}  // namespace logging

namespace log = logging;

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::logging;
using namespace ::log;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_LOGGING_LOGGING_HXX
