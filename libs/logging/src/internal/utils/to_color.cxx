// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file to_color.cxx
 * @brief Implementation of color conversion utilities for logging levels
 */

#if (__LOGENIUM_LOGGING_USE_COLOR_LOGS__ && __LOGENIUM_LOGGING_USE_FMTLIB__)

#include "fmt/color.h"

#include <debug/assert.hxx>

#include <logging/internal/utils/to_color.hxx>
#include <logging/level.hxx>

namespace logging::internal::utils {

fmt::text_style ToColor(Level level) {
    switch (level) {
        using enum logging::Level;
        case Ignore: {
            // No styling for Ignore level
            return {};
        }

        case Trace3:
            [[fallthrough]];
        case Trace2:
            [[fallthrough]];
        case Trace1:
            [[fallthrough]];
        case Trace: {
            // All trace levels use gray to indicate low-priority debug output
            return fmt::fg(fmt::color::gray);
        }

        case Debug: {
            // Cyan for debug messages
            return fmt::fg(fmt::color::cyan);
        }

        case Info: {
            // Green for informational messages (success/normal operation)
            return fmt::fg(fmt::color::green);
        }

        case Warn: {
            // Yellow for warnings (attention needed but not critical)
            return fmt::fg(fmt::color::yellow);
        }

        case Error: {
            // Red for errors (something went wrong)
            return fmt::fg(fmt::color::red);
        }

        case Fatal: {
            // White text on red background for fatal errors (maximum visibility)
            return fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red);
        }

        default: {
            debug::Assert(false, "Invalid logging::Level");
        }
    }
}

}  // namespace logging::internal::utils

#endif  // __LOGENIUM_LOGGING_USE_COLOR_LOGS__ && __LOGENIUM_LOGGING_USE_FMTLIB__
