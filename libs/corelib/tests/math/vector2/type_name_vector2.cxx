// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/math/vector2.hxx>
#include <corelib/utility/type_name.hxx>

using namespace corelib;
using namespace corelib::math;

// Test vec2 with float type
TEST(TypeNameVector2, vec2_float) {
    auto name = type_name<vec2<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
}

// Test vec2 with double type
TEST(TypeNameVector2, vec2_double) {
    auto name = type_name<vec2<double>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
}

// Test vec2 with int type
TEST(TypeNameVector2, vec2_int) {
    auto name = type_name<vec2<int>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
}

// Test vec2 with value overload
TEST(TypeNameVector2, value_overload) {
    vec2<float> v{5.0f, 7.0f};
    auto name = type_name(v);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
}

// Test const qualified vec2
TEST(TypeNameVector2, const_qualified) {
    auto name = type_name<const vec2<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
}

// Test vec2 pointer type
TEST(TypeNameVector2, pointer_type) {
    auto name = type_name<vec2<float> *>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
    EXPECT_NE(name.find("*"), std::string_view::npos);
}

// Test vec2 reference type
TEST(TypeNameVector2, reference_type) {
    auto name = type_name<vec2<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test const vec2 reference type
TEST(TypeNameVector2, const_reference_type) {
    auto name = type_name<const vec2<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test constexpr evaluation
TEST(TypeNameVector2, constexpr_evaluation) {
    constexpr auto name = type_name<vec2<float>>();
    EXPECT_FALSE(name.empty());
}

// Test rvalue reference
TEST(TypeNameVector2, rvalue_reference) {
    auto name = type_name<vec2<float> &&>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec2"), std::string_view::npos);
    EXPECT_NE(name.find("&&"), std::string_view::npos);
}

// Test that type names are consistent
TEST(TypeNameVector2, type_consistency) {
    auto name1 = type_name<vec2<float>>();
    auto name2 = type_name<vec2<float>>();
    EXPECT_EQ(name1, name2);
}
