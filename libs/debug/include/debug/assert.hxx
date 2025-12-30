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

#include <debug/breakpoint_if_debugging.hxx>

#if (__LOGENIUM_DEBUG_USE_FMTLIB__ && __LOGENIUM_DEBUG_USE_COLOR_LOGS__)

#include <fmt/color.h>

#endif  // (__LOGENIUM_DEBUG_USE_FMTLIB__ && __LOGENIUM_DEBUG_USE_COLOR_LOGS__)

#endif  // NDEBUG

#if !__LOGENIUM_DEBUG_USE_FMTLIB__

#include <format>
#include <print>

#else

#include <fmt/base.h>
#include <fmt/format.h>

#endif

namespace debug {

namespace detail {

template <typename... Args>
struct AssertImpl {
#ifndef NDEBUG

#if !__LOGENIUM_DEBUG_USE_FMTLIB__

    AssertImpl(bool predicate, std::format_string<Args...> format, Args &&...args,
               std::source_location location = std::source_location::current(),
               const std::stacktrace &stacktrace = std::stacktrace::current()) {
        if (!predicate) {
            std::println("Assertion failed: {}", std::format(format, std::forward<Args>(args)...));
            std::println("Location: {}:{} in {}", location.file_name(), location.line(), location.function_name());

#if __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

            std::println("Stacktrace:\n{}", std::to_string(stacktrace));

#else

            std::println("Stacktrace:");
            for (auto frame = stacktrace.crbegin(); frame != stacktrace.crend(); ++frame) {
                std::println("  {}", std::to_string(*frame));
            }

#endif  // __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

            BreakpointIfDebugging();
            std::abort();
        }
    }

#else

    AssertImpl(bool predicate, fmt::format_string<Args...> format, Args &&...args,
               std::source_location location = std::source_location::current(),
               const std::stacktrace &stacktrace = std::stacktrace::current()) {
        if (!predicate) {

#if !__LOGENIUM_DEBUG_USE_COLOR_LOGS__

            fmt::println("Assertion failed: {}", fmt::format(format, std::forward<Args>(args)...));

#else

            fmt::println("{}: {}",
                         fmt::styled("Assertion failed", fmt::bg(fmt::color::red) | fmt::fg(fmt::color::white)),
                         fmt::format(format, std::forward<Args>(args)...));

#endif  // __LOGENIUM_DEBUG_USE_COLOR_LOGS__

            fmt::println("Location: {}:{} in {}", location.file_name(), location.line(), location.function_name());

#if __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

            fmt::println("Stacktrace:\n{}", std::to_string(stacktrace));

#else

            fmt::println("Stacktrace:");
            for (auto frame = stacktrace.crbegin(); frame != stacktrace.crend(); ++frame) {
                fmt::println("  {}", std::to_string(*frame));
            }

#endif  // __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__

            BreakpointIfDebugging();
            std::abort();
        }
    }

#endif  // __LOGENIUM_DEBUG_USE_FMTLIB__

#else

#if !__LOGENIUM_DEBUG_USE_FMTLIB__

    constexpr AssertImpl(bool predicate, std::format_string<Args...> format, Args &&...args) {}

#else

    constexpr AssertImpl(bool predicate, fmt::format_string<Args...> format, Args &&...args) {}

#endif  // __LOGENIUM_DEBUG_USE_FMTLIB__

#endif  // NDEBUG
};

#if !__LOGENIUM_DEBUG_USE_FMTLIB__

template <typename... Args>
AssertImpl(bool, std::format_string<Args...>, Args &&...) -> AssertImpl<Args...>;

#else

template <typename... Args>
AssertImpl(bool, fmt::format_string<Args...>, Args &&...) -> AssertImpl<Args...>;

#endif

}  // namespace detail

template <typename... Args>
using Assert = detail::AssertImpl<Args...>;

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_ASSERT_HXX
