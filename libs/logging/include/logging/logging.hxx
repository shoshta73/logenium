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
 * - **Source location tracking**: Accepts std::source_location for future use
 * - **Perfect forwarding**: Arguments are forwarded without unnecessary copies
 * - **Zero overhead**: Template-based design with no runtime cost
 *
 * ## Usage
 *
 * Basic logging with automatic type deduction:
 * @code
 * #include <logging/logging.hxx>
 *
 * logging::log("Hello, {}!", "world");
 * logging::log("The answer is {}", 42);
 * logging::log("Multiple values: {}, {}, {}", 1, 2.5, "three");
 * @endcode
 *
 * With source location tracking (parameter accepted but not currently used):
 * @code
 * logging::log("Debug message", std::source_location::current());
 * @endcode
 *
 * ## Implementation Notes
 *
 * The logging module is header-only and uses template metaprogramming to provide
 * compile-time format string validation. The actual formatting backend is selected
 * at compile time based on the __LOGENIUM_LOGGING_USE_FMTLIB__ preprocessor macro.
 *
 * Source location tracking is accepted but not currently utilized in output.
 * Future versions may include file/line/function information in log messages.
 */

#include <source_location>

#if __LOGENIUM_LOGGING_USE_FMTLIB__

#include <fmt/base.h>

#else

#include <format>
#include <print>

#endif

namespace logging {

namespace detail {

/**
 * @ingroup logging
 * @brief Implementation struct for compile-time validated logging
 *
 * This struct provides the actual logging implementation with compile-time
 * format string validation. It uses CTAD (Class Template Argument Deduction)
 * to automatically deduce template arguments from constructor parameters.
 *
 * @tparam Args Variadic template parameter pack for format arguments
 *
 * @note This is an implementation detail. Users should use the logging::log
 *       alias template instead of instantiating LogImpl directly.
 */
template <typename... Args>
struct LogImpl {
#if __LOGENIUM_LOGGING_USE_FMTLIB__

    /**
     * @brief Construct and execute a log operation using fmtlib
     *
     * Validates the format string at compile time and outputs the formatted
     * message to stdout using fmt::println.
     *
     * @param format Format string with compile-time validation (fmt::format_string)
     * @param args Variadic arguments to be formatted (forwarded)
     * @param location Source location for debugging (currently unused)
     *
     * @note Arguments are perfectly forwarded to avoid unnecessary copies
     * @note The source_location parameter is accepted but not used in current implementation
     */
    LogImpl(fmt::format_string<Args...> format, Args &&...args,
            std::source_location location = std::source_location::current()) {
        fmt::println(format, std::forward<Args>(args)...);
    }

#else

    /**
     * @brief Construct and execute a log operation using standard library
     *
     * Validates the format string at compile time and outputs the formatted
     * message to stdout using std::println.
     *
     * @param format Format string with compile-time validation (std::format_string)
     * @param args Variadic arguments to be formatted (forwarded)
     * @param location Source location for debugging (currently unused)
     *
     * @note Arguments are perfectly forwarded to avoid unnecessary copies
     * @note The source_location parameter is accepted but not used in current implementation
     */
    LogImpl(std::format_string<Args...> format, Args &&...args,
            std::source_location location = std::source_location::current()) {
        std::println(format, std::forward<Args>(args)...);
    }

#endif
};

#if __LOGENIUM_LOGGING_USE_FMTLIB__

/**
 * @ingroup logging
 * @brief Deduction guide for LogImpl with fmtlib backend
 *
 * Enables CTAD (Class Template Argument Deduction) to automatically deduce
 * template parameters from constructor arguments when using fmtlib.
 *
 * @tparam Args Deduced argument types from constructor
 */
template <typename... Args>
LogImpl(bool, fmt::format_string<Args...>, Args &&...) -> LogImpl<Args...>;

#else

/**
 * @ingroup logging
 * @brief Deduction guide for LogImpl with standard library backend
 *
 * Enables CTAD (Class Template Argument Deduction) to automatically deduce
 * template parameters from constructor arguments when using standard library.
 *
 * @tparam Args Deduced argument types from constructor
 */
template <typename... Args>
LogImpl(bool, std::format_string<Args...>, Args &&...) -> LogImpl<Args...>;

#endif

}  // namespace detail

/**
 * @ingroup logging
 * @brief Type alias for convenient logging with compile-time validation
 *
 * This alias template provides a convenient interface for logging operations.
 * Users should use this instead of directly instantiating logging::detail::LogImpl.
 *
 * @tparam Args Variadic template parameter pack for format arguments
 *
 * Example usage:
 * @code
 * logging::log("Hello, {}!", "world");
 * logging::log("Value: {}", 42);
 * logging::log("Multiple: {}, {}", 1, 2);
 * @endcode
 *
 * @note The template parameters are automatically deduced via CTAD
 * @note Format strings are validated at compile time
 * @note Output goes to stdout via println
 */
template <typename... Args>
using log = detail::LogImpl<Args...>;

}  // namespace logging

#endif  // LOGENIUM_LOGGING_LOGGING_HXX
