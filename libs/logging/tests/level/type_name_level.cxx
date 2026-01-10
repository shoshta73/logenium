// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <logging/level.hxx>

#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <corelib/utility/type_name.hxx>

// Test type_name with Level enum
TEST(TypeNameLevel, basic_enum) {
    constexpr std::string_view name = corelib::type_name<logging::Level>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
}

// Test type_name with const Level
TEST(TypeNameLevel, const_enum) {
    constexpr std::string_view name = corelib::type_name<const logging::Level>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
}

// Test type_name with Level pointer
TEST(TypeNameLevel, pointer) {
    constexpr std::string_view name = corelib::type_name<logging::Level *>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("*"), std::string_view::npos);
}

// Test type_name with const Level pointer
TEST(TypeNameLevel, const_pointer) {
    constexpr std::string_view name = corelib::type_name<const logging::Level *>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
    EXPECT_NE(name.find("*"), std::string_view::npos);
}

// Test type_name with Level reference
TEST(TypeNameLevel, reference) {
    constexpr std::string_view name = corelib::type_name<logging::Level &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test type_name with const Level reference
TEST(TypeNameLevel, const_reference) {
    constexpr std::string_view name = corelib::type_name<const logging::Level &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test type_name with Level rvalue reference
TEST(TypeNameLevel, rvalue_reference) {
    constexpr std::string_view name = corelib::type_name<logging::Level &&>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("&&"), std::string_view::npos);
}

// Test value overload with Level enum
TEST(TypeNameLevel, value_overload) {
    logging::Level level = logging::Level::Info;
    std::string_view name = corelib::type_name(level);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
}

// Test value overload with const Level
TEST(TypeNameLevel, value_overload_const) {
    const logging::Level level = logging::Level::Debug;
    std::string_view name = corelib::type_name(level);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
}

// Test value overload with Level pointer
TEST(TypeNameLevel, value_overload_pointer) {
    logging::Level level = logging::Level::Warn;
    logging::Level *ptr = &level;
    std::string_view name = corelib::type_name(ptr);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    EXPECT_NE(name.find("*"), std::string_view::npos);
}

// Test value overload with Level reference
TEST(TypeNameLevel, value_overload_reference) {
    logging::Level level = logging::Level::Error;
    logging::Level &ref = level;
    std::string_view name = corelib::type_name(ref);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
    // References collapse in forwarding, so we shouldn't see & in the output
}

// Test constexpr evaluation
TEST(TypeNameLevel, constexpr_evaluation) {
    constexpr std::string_view name1 = corelib::type_name<logging::Level>();
    constexpr std::string_view name2 = corelib::type_name<const logging::Level>();
    constexpr std::string_view name3 = corelib::type_name<logging::Level *>();

    // Verify constexpr values can be used at compile time
    EXPECT_FALSE(name1.empty());
    EXPECT_FALSE(name2.empty());
    EXPECT_FALSE(name3.empty());
}

// Test different enum values produce same type name
TEST(TypeNameLevel, different_values_same_type) {
    logging::Level level1 = logging::Level::Trace;
    logging::Level level2 = logging::Level::Fatal;

    std::string_view name1 = corelib::type_name(level1);
    std::string_view name2 = corelib::type_name(level2);

    // Type name should be the same regardless of value
    EXPECT_EQ(name1, name2);
}

// Test namespace appears in type name
TEST(TypeNameLevel, namespace_in_name) {
    constexpr std::string_view name = corelib::type_name<logging::Level>();
    EXPECT_FALSE(name.empty());
    // Should contain either "logging" or "log" namespace
    bool has_namespace = name.find("logging") != std::string_view::npos || name.find("log") != std::string_view::npos;
    EXPECT_TRUE(has_namespace);
}

// Test type_name is different from formatter output
TEST(TypeNameLevel, different_from_formatter) {
    logging::Level level = logging::Level::Info;

    std::string_view type_name = corelib::type_name(level);
    std::string format_output = std::format("{}", level);

    // type_name shows the type, formatter shows the value
    EXPECT_NE(type_name, format_output);
    EXPECT_NE(type_name.find("Level"), std::string_view::npos);
    EXPECT_EQ(format_output, "Info");
}

// Test with log namespace alias
TEST(TypeNameLevel, namespace_alias) {
    log::Level level = log::Level::Trace3;
    std::string_view name = corelib::type_name(level);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Level"), std::string_view::npos);
}
