// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <format>
#include <string>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include <corelib/types/int.hxx>

#include <logging/level.hxx>

// Test that Level is an enum class
TEST(Level, is_enum) {
    EXPECT_TRUE(std::is_enum_v<logging::Level>);
    EXPECT_TRUE(std::is_scoped_enum_v<logging::Level>);
}

// Test that underlying type is u8
TEST(Level, underlying_type) { EXPECT_TRUE((std::is_same_v<std::underlying_type_t<logging::Level>, corelib::u8>)); }

// Test all enum values and their underlying values
TEST(Level, enum_values) {
    using enum logging::Level;

    EXPECT_EQ(std::to_underlying(Ignore), 0);
    EXPECT_EQ(std::to_underlying(Trace3), 1);
    EXPECT_EQ(std::to_underlying(Trace2), 2);
    EXPECT_EQ(std::to_underlying(Trace1), 3);
    EXPECT_EQ(std::to_underlying(Trace), 4);
    EXPECT_EQ(std::to_underlying(Debug), 5);
    EXPECT_EQ(std::to_underlying(Info), 6);
    EXPECT_EQ(std::to_underlying(Warn), 7);
    EXPECT_EQ(std::to_underlying(Error), 8);
    EXPECT_EQ(std::to_underlying(Fatal), 9);
}

// Test enum ordering (severity increases with underlying value)
TEST(Level, ordering) {
    using enum logging::Level;

    EXPECT_LT(std::to_underlying(Ignore), std::to_underlying(Trace3));
    EXPECT_LT(std::to_underlying(Trace3), std::to_underlying(Trace2));
    EXPECT_LT(std::to_underlying(Trace2), std::to_underlying(Trace1));
    EXPECT_LT(std::to_underlying(Trace1), std::to_underlying(Trace));
    EXPECT_LT(std::to_underlying(Trace), std::to_underlying(Debug));
    EXPECT_LT(std::to_underlying(Debug), std::to_underlying(Info));
    EXPECT_LT(std::to_underlying(Info), std::to_underlying(Warn));
    EXPECT_LT(std::to_underlying(Warn), std::to_underlying(Error));
    EXPECT_LT(std::to_underlying(Error), std::to_underlying(Fatal));
}

// Test formatter with Ignore level (normal mode)
TEST(Level, format_ignore) {
    logging::Level level = logging::Level::Ignore;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Ignore");
}

// Test formatter with Trace3 level (normal mode)
TEST(Level, format_trace3) {
    logging::Level level = logging::Level::Trace3;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Trace3");
}

// Test formatter with Trace2 level (normal mode)
TEST(Level, format_trace2) {
    logging::Level level = logging::Level::Trace2;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Trace2");
}

// Test formatter with Trace1 level (normal mode)
TEST(Level, format_trace1) {
    logging::Level level = logging::Level::Trace1;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Trace1");
}

// Test formatter with Trace level (normal mode)
TEST(Level, format_trace) {
    logging::Level level = logging::Level::Trace;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Trace");
}

// Test formatter with Debug level (normal mode)
TEST(Level, format_debug) {
    logging::Level level = logging::Level::Debug;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Debug");
}

// Test formatter with Info level (normal mode)
TEST(Level, format_info) {
    logging::Level level = logging::Level::Info;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Info");
}

// Test formatter with Warn level (normal mode)
TEST(Level, format_warn) {
    logging::Level level = logging::Level::Warn;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Warn");
}

// Test formatter with Error level (normal mode)
TEST(Level, format_error) {
    logging::Level level = logging::Level::Error;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Error");
}

// Test formatter with Fatal level (normal mode)
TEST(Level, format_fatal) {
    logging::Level level = logging::Level::Fatal;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Fatal");
}

// Test formatter with Ignore level (debug mode)
TEST(Level, format_ignore_debug) {
    logging::Level level = logging::Level::Ignore;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Ignore, 0 )");
}

// Test formatter with Trace3 level (debug mode)
TEST(Level, format_trace3_debug) {
    logging::Level level = logging::Level::Trace3;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Trace3, 1 )");
}

// Test formatter with Trace2 level (debug mode)
TEST(Level, format_trace2_debug) {
    logging::Level level = logging::Level::Trace2;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Trace2, 2 )");
}

// Test formatter with Trace1 level (debug mode)
TEST(Level, format_trace1_debug) {
    logging::Level level = logging::Level::Trace1;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Trace1, 3 )");
}

// Test formatter with Trace level (debug mode)
TEST(Level, format_trace_debug) {
    logging::Level level = logging::Level::Trace;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Trace, 4 )");
}

// Test formatter with Debug level (debug mode)
TEST(Level, format_debug_debug) {
    logging::Level level = logging::Level::Debug;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Debug, 5 )");
}

// Test formatter with Info level (debug mode)
TEST(Level, format_info_debug) {
    logging::Level level = logging::Level::Info;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Info, 6 )");
}

// Test formatter with Warn level (debug mode)
TEST(Level, format_warn_debug) {
    logging::Level level = logging::Level::Warn;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Warn, 7 )");
}

// Test formatter with Error level (debug mode)
TEST(Level, format_error_debug) {
    logging::Level level = logging::Level::Error;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Error, 8 )");
}

// Test formatter with Fatal level (debug mode)
TEST(Level, format_fatal_debug) {
    logging::Level level = logging::Level::Fatal;
    std::string result = std::format("{:d}", level);
    EXPECT_EQ(result, "logging::Level( Fatal, 9 )");
}

// Test invalid format specifier
TEST(Level, format_invalid_specifier) {
    logging::Level level = logging::Level::Info;
    EXPECT_THROW(std::format("{:x}", level), std::format_error);
}

// Test multiple format specifier characters
TEST(Level, format_multiple_specifiers) {
    logging::Level level = logging::Level::Warn;
    EXPECT_THROW(std::format("{:dd}", level), std::format_error);
}

// Test format in string context
TEST(Level, format_in_string) {
    logging::Level level = logging::Level::Error;
    std::string result = std::format("Current level: {}", level);
    EXPECT_EQ(result, "Current level: Error");
}

// Test format in string context (debug mode)
TEST(Level, format_in_string_debug) {
    logging::Level level = logging::Level::Fatal;
    std::string result = std::format("Current level: {:d}", level);
    EXPECT_EQ(result, "Current level: logging::Level( Fatal, 9 )");
}

// Test multiple levels in same format string
TEST(Level, format_multiple_levels) {
    logging::Level min_level = logging::Level::Debug;
    logging::Level max_level = logging::Level::Fatal;
    std::string result = std::format("Range: {} to {}", min_level, max_level);
    EXPECT_EQ(result, "Range: Debug to Fatal");
}

// Test namespace alias 'log'
TEST(Level, namespace_alias) {
    log::Level level = log::Level::Info;
    std::string result = std::format("{}", level);
    EXPECT_EQ(result, "Info");
}

// Test sizeof Level (should be 1 byte due to u8 underlying type)
TEST(Level, size) {
    EXPECT_EQ(sizeof(logging::Level), sizeof(corelib::u8));
    EXPECT_EQ(sizeof(logging::Level), 1);
}
