// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

// Only test color functionality when it's enabled
#if (__LOGENIUM_LOGGING_USE_COLOR_LOGS__ && __LOGENIUM_LOGGING_USE_FMTLIB__)

#include <fmt/color.h>

#include <logging/internal/utils/to_color.hxx>
#include <logging/level.hxx>

using namespace logging::internal::utils;

// Test that Ignore level returns empty text_style
TEST(ToColor, ignore_level_returns_empty_style) {
    auto style = ToColor(logging::Level::Ignore);
    // Empty text_style should have no foreground or background set
    EXPECT_FALSE(style.has_foreground());
    EXPECT_FALSE(style.has_background());
}

// Test that all Trace levels return gray foreground
TEST(ToColor, trace3_level_returns_gray) {
    auto style = ToColor(logging::Level::Trace3);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::gray).value());
    EXPECT_FALSE(style.has_background());
}

TEST(ToColor, trace2_level_returns_gray) {
    auto style = ToColor(logging::Level::Trace2);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::gray).value());
    EXPECT_FALSE(style.has_background());
}

TEST(ToColor, trace1_level_returns_gray) {
    auto style = ToColor(logging::Level::Trace1);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::gray).value());
    EXPECT_FALSE(style.has_background());
}

TEST(ToColor, trace_level_returns_gray) {
    auto style = ToColor(logging::Level::Trace);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::gray).value());
    EXPECT_FALSE(style.has_background());
}

// Test Debug level returns cyan foreground
TEST(ToColor, debug_level_returns_cyan) {
    auto style = ToColor(logging::Level::Debug);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::cyan).value());
    EXPECT_FALSE(style.has_background());
}

// Test Info level returns green foreground
TEST(ToColor, info_level_returns_green) {
    auto style = ToColor(logging::Level::Info);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::green).value());
    EXPECT_FALSE(style.has_background());
}

// Test Warn level returns yellow foreground
TEST(ToColor, warn_level_returns_yellow) {
    auto style = ToColor(logging::Level::Warn);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::yellow).value());
    EXPECT_FALSE(style.has_background());
}

// Test Error level returns red foreground
TEST(ToColor, error_level_returns_red) {
    auto style = ToColor(logging::Level::Error);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::red).value());
    EXPECT_FALSE(style.has_background());
}

// Test Fatal level returns white foreground on red background
TEST(ToColor, fatal_level_returns_white_on_red) {
    auto style = ToColor(logging::Level::Fatal);
    EXPECT_TRUE(style.has_foreground());
    EXPECT_TRUE(style.has_background());
    EXPECT_EQ(style.get_foreground().value(), fmt::detail::color_type(fmt::color::white).value());
    EXPECT_EQ(style.get_background().value(), fmt::detail::color_type(fmt::color::red).value());
}

// Test that all levels are handled (no missing cases)
TEST(ToColor, all_levels_handled) {
    // Should compile and not throw - testing all enum values are handled
    ToColor(logging::Level::Ignore);
    ToColor(logging::Level::Trace3);
    ToColor(logging::Level::Trace2);
    ToColor(logging::Level::Trace1);
    ToColor(logging::Level::Trace);
    ToColor(logging::Level::Debug);
    ToColor(logging::Level::Info);
    ToColor(logging::Level::Warn);
    ToColor(logging::Level::Error);
    ToColor(logging::Level::Fatal);
    SUCCEED();
}

// Test that returned text_style can be used with fmt::format
TEST(ToColor, text_style_works_with_format) {
    auto style = ToColor(logging::Level::Info);
    auto formatted = fmt::format(style, "Test message");
    // Just verify it doesn't throw and produces output
    EXPECT_FALSE(formatted.empty());
}

// Test that different levels produce different styles (except trace variants)
TEST(ToColor, different_levels_produce_different_styles) {
    auto ignore_style = ToColor(logging::Level::Ignore);
    auto trace_style = ToColor(logging::Level::Trace);
    auto debug_style = ToColor(logging::Level::Debug);
    auto info_style = ToColor(logging::Level::Info);
    auto warn_style = ToColor(logging::Level::Warn);
    auto error_style = ToColor(logging::Level::Error);
    auto fatal_style = ToColor(logging::Level::Fatal);

    // Trace variants should all be the same
    EXPECT_EQ(ToColor(logging::Level::Trace3).get_foreground().value(), trace_style.get_foreground().value());
    EXPECT_EQ(ToColor(logging::Level::Trace2).get_foreground().value(), trace_style.get_foreground().value());
    EXPECT_EQ(ToColor(logging::Level::Trace1).get_foreground().value(), trace_style.get_foreground().value());

    // All other levels should have different foregrounds (or no foreground for Ignore)
    EXPECT_FALSE(ignore_style.has_foreground());
    EXPECT_NE(trace_style.get_foreground().value(), debug_style.get_foreground().value());
    EXPECT_NE(debug_style.get_foreground().value(), info_style.get_foreground().value());
    EXPECT_NE(info_style.get_foreground().value(), warn_style.get_foreground().value());
    EXPECT_NE(warn_style.get_foreground().value(), error_style.get_foreground().value());
    EXPECT_NE(error_style.get_foreground().value(), fatal_style.get_foreground().value());
}

#endif  // __LOGENIUM_LOGGING_USE_COLOR_LOGS__ && __LOGENIUM_LOGGING_USE_FMTLIB__
