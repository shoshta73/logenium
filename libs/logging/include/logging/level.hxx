// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_LOGGING_LEVEL_HXX
#define LOGENIUM_LOGGING_LEVEL_HXX

#include <utility>

#if __LOGENIUM_LOGGING_USE_FMTLIB__

#include <fmt/base.h>
#include <fmt/format.h>

#else

#include <format>

#endif

#include <debug/assert.hxx>

#include <corelib/types/int.hxx>

namespace logging {

/**
 * @ingroup logging
 * @brief Logging severity levels
 *
 * Enumeration representing different logging severity levels, ordered from
 * lowest to highest severity. The underlying type is `corelib::u8` for efficient
 * storage.
 *
 * @note Custom formatter provided for both std::format (C++20) and fmtlib
 * @note Use format specifier `{:d}` for debug mode (shows enum name and underlying value)
 *
 * @code
 * logging::Level level = logging::Level::Info;
 * std::format("{}", level);    // Output: "Info"
 * std::format("{:d}", level);  // Output: "logging::Level( Info, 6 )"
 * @endcode
 */
enum class Level : corelib::u8 {
    /**
     * @brief Ignore level - lowest priority
     *
     * Used to suppress all logging output. Typically used as a filter threshold
     * rather than as a message level.
     */
    Ignore,

    /**
     * @brief Trace level 3 - most verbose trace
     *
     * Extremely detailed diagnostic information for deep debugging. Third level
     * of trace granularity.
     */
    Trace3,

    /**
     * @brief Trace level 2 - detailed trace
     *
     * Detailed diagnostic information for debugging. Second level of trace
     * granularity.
     */
    Trace2,

    /**
     * @brief Trace level 1 - basic trace
     *
     * Basic diagnostic information for debugging. First level of trace
     * granularity.
     */
    Trace1,

    /**
     * @brief Trace level - standard trace
     *
     * Standard trace-level diagnostic information for debugging program flow.
     */
    Trace,

    /**
     * @brief Debug level
     *
     * Debug-level messages for development and troubleshooting. Generally
     * disabled in production builds.
     */
    Debug,

    /**
     * @brief Info level
     *
     * Informational messages about normal program operation. Typically enabled
     * in production for audit trails and general status updates.
     */
    Info,

    /**
     * @brief Warning level
     *
     * Warning messages for potentially problematic situations that don't prevent
     * program operation but may require attention.
     */
    Warn,

    /**
     * @brief Error level
     *
     * Error messages for serious problems that prevent specific operations from
     * completing successfully but don't terminate the program.
     */
    Error,

    /**
     * @brief Fatal level - highest severity
     *
     * Critical error messages for severe problems that typically result in
     * program termination. Highest severity level.
     */
    Fatal,
};

}  // namespace logging

namespace log = logging;

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::logging;
using namespace ::log;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#if __LOGENIUM_LOGGING_USE_FMTLIB__

/**
 * @brief Custom formatter for logging::Level enum (fmtlib backend)
 *
 * Provides formatting support for logging::Level enum values when using fmtlib
 * as the formatting backend. Supports two format modes:
 * - Default (`{}`): Outputs the enum name only (e.g., "Info")
 * - Debug mode (`{:d}`): Outputs full details including underlying value
 *   (e.g., "logging::Level( Info, 6 )")
 *
 * @note This specialization is used when LOGENIUM_LOGGING_USE_FMTLIB is enabled
 *
 * @code
 * logging::Level level = logging::Level::Warn;
 * fmt::format("{}", level);    // Output: "Warn"
 * fmt::format("{:d}", level);  // Output: "logging::Level( Warn, 7 )"
 * @endcode
 */
template <>
struct fmt::formatter<logging::Level> {
    constexpr auto parse(fmt::format_parse_context &ctx) {
        auto it = ctx.begin();
        if (it == ctx.end()) {
            return it;
        }
        if (*it == 'd') {
            debug_flag = true;
            ++it;
        }
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("Invalid format arguments for logging::Level");
        }
        return it;
    }

    auto format(const logging::Level &level, fmt::format_context &ctx) const {
        switch (level) {
            using enum logging::Level;
            case Ignore: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Ignore, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Ignore");
            }
            case Trace3: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Trace3, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Trace3");
            }
            case Trace2: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Trace2, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Trace2");
            }
            case Trace1: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Trace1, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Trace1");
            }
            case Trace: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Trace, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Trace");
            }
            case Debug: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Debug, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Debug");
            }
            case Info: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Info, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Info");
            }
            case Warn: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Warn, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Warn");
            }
            case Error: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Error, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Error");
            }
            case Fatal: {
                if (debug_flag) {
                    return fmt::format_to(ctx.out(), "logging::Level( Fatal, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return fmt::format_to(ctx.out(), "Fatal");
            }

            default: {
                debug::Assert(false, "Invalid logging::Level");
            }
        }
    }

  private:
    bool debug_flag = false;
};

#else

/**
 * @brief Custom formatter for logging::Level enum (standard library backend)
 *
 * Provides formatting support for logging::Level enum values when using the
 * C++20 standard library std::format as the formatting backend. Supports two
 * format modes:
 * - Default (`{}`): Outputs the enum name only (e.g., "Info")
 * - Debug mode (`{:d}`): Outputs full details including underlying value
 *   (e.g., "logging::Level( Info, 6 )")
 *
 * @note This specialization is used when LOGENIUM_LOGGING_USE_FMTLIB is disabled
 *
 * @code
 * logging::Level level = logging::Level::Error;
 * std::format("{}", level);    // Output: "Error"
 * std::format("{:d}", level);  // Output: "logging::Level( Error, 8 )"
 * @endcode
 */
template <>
struct std::formatter<logging::Level> {
    constexpr auto parse(std::format_parse_context &ctx) {
        auto it = ctx.begin();
        if (it == ctx.end()) {
            return it;
        }
        if (*it == 'd') {
            debug_flag = true;
            ++it;
        }
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format arguments for logging::Level");
        }
        return it;
    }

    auto format(const logging::Level &level, std::format_context &ctx) const {
        switch (level) {
            using enum logging::Level;
            case Ignore: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Ignore, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Ignore");
            }
            case Trace3: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Trace3, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Trace3");
            }
            case Trace2: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Trace2, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Trace2");
            }
            case Trace1: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Trace1, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Trace1");
            }
            case Trace: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Trace, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Trace");
            }
            case Debug: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Debug, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Debug");
            }
            case Info: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Info, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Info");
            }
            case Warn: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Warn, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Warn");
            }
            case Error: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Error, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Error");
            }
            case Fatal: {
                if (debug_flag) {
                    return std::format_to(ctx.out(), "logging::Level( Fatal, {} )",
                                          static_cast<corelib::u32>(std::to_underlying(level)));
                }

                return std::format_to(ctx.out(), "Fatal");
            }

            default: {
                debug::Assert(false, "Invalid logging::Level");
            }
        }
    }

  private:
    bool debug_flag = false;
};

#endif

#endif  // LOGENIUM_LOGGING_LEVEL_HXX
