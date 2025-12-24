// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_DEBUG_ASSERT_HXX
#define LOGENIUM_DEBUG_ASSERT_HXX

#include <format>
#include <print>
#include <source_location>
#include <stacktrace>
#include <string>
#include <utility>

#include <debug/breakpoint.hxx>

namespace debug {

namespace detail {

template <typename... Args>
struct AssertImpl {
    AssertImpl(bool predicate, std::format_string<Args...> format, Args &&...args,
               std::source_location location = std::source_location::current(),
               const std::stacktrace &stacktrace = std::stacktrace::current()) {
        if (!predicate) {
            std::println("Assertion failed: {}", std::format(format, std::forward<Args>(args)...));
            std::println("Location: {}:{} in {}", location.file_name(), location.line(), location.function_name());
            std::println("Stacktrace:\n{}", std::to_string(stacktrace));
            Breakpoint();
        }
    }
};

template <typename... Args>
AssertImpl(bool, std::format_string<Args...>, Args &&...) -> AssertImpl<Args...>;

}  // namespace detail

template <typename... Args>
using Assert = detail::AssertImpl<Args...>;

}  // namespace debug

#endif  // LOGENIUM_DEBUG_ASSERT_HXX
