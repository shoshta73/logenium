// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/math/vector3.hxx>
#include <corelib/utility/type_name.hxx>

using namespace corelib;
using namespace corelib::math;

// Test Vec3 with float type
TEST(TypeNameVector3, Vec3_float) {
    auto name = type_name<vec3<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
}

// Test Vec3 with double type
TEST(TypeNameVector3, Vec3_double) {
    auto name = type_name<vec3<double>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
}

// Test Vec3 with int type
TEST(TypeNameVector3, Vec3_int) {
    auto name = type_name<vec3<int>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
}

// Test Vec3 with value overload
TEST(TypeNameVector3, value_overload) {
    vec3<float> v{5.0f, 7.0f, 9.0f};
    auto name = type_name(v);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
}

// Test const qualified Vec3
TEST(TypeNameVector3, const_qualified) {
    auto name = type_name<const vec3<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
}

// Test Vec3 pointer type
TEST(TypeNameVector3, pointer_type) {
    auto name = type_name<vec3<float> *>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
    EXPECT_NE(name.find("*"), std::string_view::npos);
}

// Test Vec3 reference type
TEST(TypeNameVector3, reference_type) {
    auto name = type_name<vec3<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test const Vec3 reference type
TEST(TypeNameVector3, const_reference_type) {
    auto name = type_name<const vec3<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test constexpr evaluation
TEST(TypeNameVector3, constexpr_evaluation) {
    constexpr auto name = type_name<vec3<float>>();
    EXPECT_FALSE(name.empty());
}

// Test rvalue reference
TEST(TypeNameVector3, rvalue_reference) {
    auto name = type_name<vec3<float> &&>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("Vec3"), std::string_view::npos);
    EXPECT_NE(name.find("&&"), std::string_view::npos);
}

// Test that type names are consistent
TEST(TypeNameVector3, type_consistency) {
    auto name1 = type_name<vec3<float>>();
    auto name2 = type_name<vec3<float>>();
    EXPECT_EQ(name1, name2);
}
