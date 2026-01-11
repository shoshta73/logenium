// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_LOGGING_INTERNAL_UTILS_TO_COLOR_HXX
#define LOGENIUM_LOGGING_INTERNAL_UTILS_TO_COLOR_HXX

/**
 * @file to_color.hxx
 * @brief Color conversion utilities for logging levels
 *
 * This header provides utilities to convert logging levels to fmtlib text styles
 * with appropriate colors. Only available when both LOGENIUM_LOGGING_USE_COLOR_LOGS
 * and LOGENIUM_LOGGING_USE_FMTLIB are enabled.
 */

#include <fmt/color.h>

#include <logging/level.hxx>

/**
 * @defgroup logging_utils Logging Utilities
 * @ingroup logging
 * @brief Internal utility functions for the logging module
 */

namespace logging::internal::utils {

/**
 * @ingroup logging_utils
 * @brief Convert a logging level to its corresponding text style with color
 *
 * Maps each logging level to a specific color scheme using fmtlib's text_style:
 * - Ignore: No styling (empty text_style)
 * - Trace3, Trace2, Trace1, Trace: Gray foreground
 * - Debug: Cyan foreground
 * - Info: Green foreground
 * - Warn: Yellow foreground
 * - Error: Red foreground
 * - Fatal: White foreground on red background
 *
 * @param level The logging level to convert
 * @return fmt::text_style with appropriate foreground/background colors
 *
 * @note This function is only available when both __LOGENIUM_LOGGING_USE_COLOR_LOGS__
 *       and __LOGENIUM_LOGGING_USE_FMTLIB__ are defined
 * @note The color scheme is designed to be visible on both light and dark terminals
 */
fmt::text_style ToColor(Level level);

}  // namespace logging::internal::utils

#endif  // LOGENIUM_LOGGING_INTERNAL_UTILS_TO_COLOR_HXX
