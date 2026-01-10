// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/math/vector1.hxx>
#include <corelib/utility/type_name.hxx>

using namespace corelib;
using namespace corelib::math;

// Test vec1 with float type
TEST(TypeNameVector1, vec1_float) {
    auto name = type_name<vec1<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
}

// Test vec1 with double type
TEST(TypeNameVector1, vec1_double) {
    auto name = type_name<vec1<double>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
}

// Test vec1 with int type
TEST(TypeNameVector1, vec1_int) {
    auto name = type_name<vec1<int>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
}

// Test vec1 with value overload
TEST(TypeNameVector1, value_overload) {
    vec1<float> v{5.0f};
    auto name = type_name(v);
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
}

// Test const qualified vec1
TEST(TypeNameVector1, const_qualified) {
    auto name = type_name<const vec1<float>>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
}

// Test vec1 pointer type
TEST(TypeNameVector1, pointer_type) {
    auto name = type_name<vec1<float> *>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
    EXPECT_NE(name.find("*"), std::string_view::npos);
}

// Test vec1 reference type
TEST(TypeNameVector1, reference_type) {
    auto name = type_name<vec1<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test const vec1 reference type
TEST(TypeNameVector1, const_reference_type) {
    auto name = type_name<const vec1<float> &>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
    EXPECT_NE(name.find("const"), std::string_view::npos);
    EXPECT_NE(name.find("&"), std::string_view::npos);
}

// Test constexpr evaluation
TEST(TypeNameVector1, constexpr_evaluation) {
    constexpr auto name = type_name<vec1<float>>();
    EXPECT_FALSE(name.empty());
}

// Test value overload with different types
TEST(TypeNameVector1, value_overload_double) {
    vec1 v{3.14};
    auto name = type_name(v);
    EXPECT_EQ(name, type_name<vec1<double>>());
}

TEST(TypeNameVector1, value_overload_int) {
    vec1 v{42};
    auto name = type_name(v);
    EXPECT_EQ(name, type_name<vec1<int>>());
}

// Test rvalue reference
TEST(TypeNameVector1, rvalue_reference) {
    auto name = type_name<vec1<float> &&>();
    EXPECT_FALSE(name.empty());
    EXPECT_NE(name.find("vec1"), std::string_view::npos);
    EXPECT_NE(name.find("&&"), std::string_view::npos);
}

// Test that type names are consistent
TEST(TypeNameVector1, type_consistency) {
    auto name1 = type_name<vec1<float>>();
    auto name2 = type_name<vec1<float>>();
    EXPECT_EQ(name1, name2);
}
