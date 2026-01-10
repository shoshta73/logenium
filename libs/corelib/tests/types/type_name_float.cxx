// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/types/float.hxx>
#include <corelib/utility/type_name.hxx>

// Test that floating-point type aliases resolve to correct type names
TEST(TypeNameFloat, basic_types) {
    EXPECT_EQ(corelib::type_name<corelib::f32>(), corelib::type_name<float>());
    EXPECT_EQ(corelib::type_name<corelib::f64>(), corelib::type_name<double>());
}

// Test that f32 is exactly float
TEST(TypeNameFloat, f32_is_float) { EXPECT_EQ(corelib::type_name<corelib::f32>(), "float"); }

// Test that f64 is exactly double
TEST(TypeNameFloat, f64_is_double) { EXPECT_EQ(corelib::type_name<corelib::f64>(), "double"); }

// Test floating-point types with value overload
TEST(TypeNameFloat, value_overload) {
    corelib::f32 f32_val = 3.14f;
    corelib::f64 f64_val = 2.71828;

    EXPECT_EQ(corelib::type_name(f32_val), corelib::type_name<float>());
    EXPECT_EQ(corelib::type_name(f64_val), corelib::type_name<double>());
}

// Test with specific floating-point values
TEST(TypeNameFloat, specific_values) {
    corelib::f32 zero_f32 = 0.0f;
    corelib::f32 negative_f32 = -1.5f;
    corelib::f32 positive_f32 = 42.42f;

    corelib::f64 zero_f64 = 0.0;
    corelib::f64 negative_f64 = -3.14159;
    corelib::f64 positive_f64 = 1.41421;

    EXPECT_EQ(corelib::type_name(zero_f32), "float");
    EXPECT_EQ(corelib::type_name(negative_f32), "float");
    EXPECT_EQ(corelib::type_name(positive_f32), "float");

    EXPECT_EQ(corelib::type_name(zero_f64), "double");
    EXPECT_EQ(corelib::type_name(negative_f64), "double");
    EXPECT_EQ(corelib::type_name(positive_f64), "double");
}

// Test const qualified types
TEST(TypeNameFloat, const_qualified_types) {
    EXPECT_EQ(corelib::type_name<const corelib::f32>(), corelib::type_name<const float>());
    EXPECT_EQ(corelib::type_name<const corelib::f64>(), corelib::type_name<const double>());
}

// Test const values
TEST(TypeNameFloat, const_values) {
    const corelib::f32 const_f32 = 3.14f;
    const corelib::f64 const_f64 = 2.71828;

    EXPECT_EQ(corelib::type_name(const_f32), "const float");
    EXPECT_EQ(corelib::type_name(const_f64), "const double");
}

// Test pointer types
TEST(TypeNameFloat, pointer_types) {
    EXPECT_EQ(corelib::type_name<corelib::f32 *>(), corelib::type_name<float *>());
    EXPECT_EQ(corelib::type_name<corelib::f64 *>(), corelib::type_name<double *>());
    EXPECT_EQ(corelib::type_name<const corelib::f32 *>(), corelib::type_name<const float *>());
    EXPECT_EQ(corelib::type_name<const corelib::f64 *>(), corelib::type_name<const double *>());
}

// Test pointer values
TEST(TypeNameFloat, pointer_values) {
    corelib::f32 f32_val = 1.0f;
    corelib::f64 f64_val = 1.0;

    corelib::f32 *f32_ptr = &f32_val;
    corelib::f64 *f64_ptr = &f64_val;

    EXPECT_EQ(corelib::type_name(f32_ptr), "float *");
    EXPECT_EQ(corelib::type_name(f64_ptr), "double *");
}

// Test reference types
TEST(TypeNameFloat, reference_types) {
    EXPECT_EQ(corelib::type_name<corelib::f32 &>(), corelib::type_name<float &>());
    EXPECT_EQ(corelib::type_name<corelib::f64 &>(), corelib::type_name<double &>());
    EXPECT_EQ(corelib::type_name<const corelib::f32 &>(), corelib::type_name<const float &>());
    EXPECT_EQ(corelib::type_name<const corelib::f64 &>(), corelib::type_name<const double &>());
}

// Test rvalue reference types
TEST(TypeNameFloat, rvalue_reference_types) {
    EXPECT_EQ(corelib::type_name<corelib::f32 &&>(), corelib::type_name<float &&>());
    EXPECT_EQ(corelib::type_name<corelib::f64 &&>(), corelib::type_name<double &&>());
}

// Test constexpr evaluation with floating-point type aliases
TEST(TypeNameFloat, constexpr_evaluation) {
    constexpr auto f32_name = corelib::type_name<corelib::f32>();
    constexpr auto f64_name = corelib::type_name<corelib::f64>();

    static_assert(f32_name == "float", "type_name<f32>() should resolve to float");
    static_assert(f64_name == "double", "type_name<f64>() should resolve to double");

    EXPECT_EQ(f32_name, "float");
    EXPECT_EQ(f64_name, "double");
}

// Test that type names are non-empty
TEST(TypeNameFloat, non_empty_names) {
    EXPECT_FALSE(corelib::type_name<corelib::f32>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::f64>().empty());
}

// Test that type names contain expected substrings
TEST(TypeNameFloat, contains_expected_substrings) {
    auto f32_name = corelib::type_name<corelib::f32>();
    auto f64_name = corelib::type_name<corelib::f64>();

    EXPECT_TRUE(f32_name.find("float") != std::string_view::npos) << "Expected 'float' in: " << f32_name;
    EXPECT_TRUE(f64_name.find("double") != std::string_view::npos) << "Expected 'double' in: " << f64_name;
}
