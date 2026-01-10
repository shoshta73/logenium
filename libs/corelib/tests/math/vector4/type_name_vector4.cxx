// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/math/vector4.hxx>
#include <corelib/utility/type_name.hxx>

using namespace corelib;
using namespace corelib::math;

// Test Vec4 with float type
TEST(TypeNameVector4, Vec4_float) {
    auto name = type_name<vec4<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
}

// Test Vec4 with double type
TEST(TypeNameVector4, Vec4_double) {
    auto name = type_name<vec4<double>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
}

// Test Vec4 with int type
TEST(TypeNameVector4, Vec4_int) {
    auto name = type_name<vec4<int>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
}

// Test Vec4 with value overload
TEST(TypeNameVector4, value_overload) {
    vec4<float> v{5.0f, 7.0f, 9.0f, 11.0f};
    auto name = type_name(v);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
}

// Test const qualified Vec4
TEST(TypeNameVector4, const_qualified) {
    auto name = type_name<const vec4<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
}

// Test Vec4 pointer type
TEST(TypeNameVector4, pointer_type) {
    auto name = type_name<vec4<float> *>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
    EXPECT_NE(name.find("*"), std::string_view::npos);
}

// Test Vec4 reference type
TEST(TypeNameVector4, reference_type) {
    auto name = type_name<vec4<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test const Vec4 reference type
TEST(TypeNameVector4, const_reference_type) {
    auto name = type_name<const vec4<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test constexpr evaluation
TEST(TypeNameVector4, constexpr_evaluation) {
    constexpr auto name = type_name<vec4<float>>();
    EXPECT_FALSE(name.empty());
}

// Test rvalue reference
TEST(TypeNameVector4, rvalue_reference) {
    auto name = type_name<vec4<float> &&>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec4"), std::string_view::npos);
    EXPECT_NE(name.find("&&"), std::string_view::npos);
}

// Test that type names are consistent
TEST(TypeNameVector4, type_consistency) {
    auto name1 = type_name<vec4<float>>();
    auto name2 = type_name<vec4<float>>();
    EXPECT_EQ(name1, name2);
}
