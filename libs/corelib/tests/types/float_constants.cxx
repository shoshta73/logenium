// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <cmath>
#include <limits>

#include <gtest/gtest.h>

#include <corelib/types/float.hxx>

// Test f32 constants
TEST(FloatConstants, f32_constants) {
    EXPECT_EQ(corelib::f32_min, std::numeric_limits<float>::min());
    EXPECT_EQ(corelib::f32_max, std::numeric_limits<float>::max());
}

// Test f64 constants
TEST(FloatConstants, f64_constants) {
    EXPECT_EQ(corelib::f64_min, std::numeric_limits<double>::min());
    EXPECT_EQ(corelib::f64_max, std::numeric_limits<double>::max());
}

// Test that constants are compile-time constexpr
TEST(FloatConstants, constexpr_evaluation) {
    // These tests verify that constants can be used in constexpr context
    constexpr auto test_f32_min = corelib::f32_min;
    constexpr auto test_f32_max = corelib::f32_max;
    constexpr auto test_f64_min = corelib::f64_min;
    constexpr auto test_f64_max = corelib::f64_max;

    // Verify that the constexpr values match the runtime values
    EXPECT_EQ(test_f32_min, corelib::f32_min);
    EXPECT_EQ(test_f32_max, corelib::f32_max);
    EXPECT_EQ(test_f64_min, corelib::f64_min);
    EXPECT_EQ(test_f64_max, corelib::f64_max);
}

// Test that f32_min is positive and smaller than 1
TEST(FloatConstants, f32_min_properties) {
    EXPECT_GT(corelib::f32_min, 0.0f);
    EXPECT_LT(corelib::f32_min, 1.0f);
    EXPECT_FLOAT_EQ(corelib::f32_min, 1.175494e-38f);
}

// Test that f32_max is large and positive
TEST(FloatConstants, f32_max_properties) {
    EXPECT_GT(corelib::f32_max, 0.0f);
    EXPECT_GT(corelib::f32_max, 1.0e38f);
    EXPECT_FLOAT_EQ(corelib::f32_max, 3.402823e+38f);
}

// Test that f64_min is positive and smaller than 1
TEST(FloatConstants, f64_min_properties) {
    EXPECT_GT(corelib::f64_min, 0.0);
    EXPECT_LT(corelib::f64_min, 1.0);
    EXPECT_GT(corelib::f64_min, 1.0e-308);
}

// Test that f64_max is large and positive
TEST(FloatConstants, f64_max_properties) {
    EXPECT_GT(corelib::f64_max, 0.0);
    EXPECT_GT(corelib::f64_max, 1.0e308);
    // f64_max should be the largest representable finite value
    EXPECT_EQ(corelib::f64_max, std::numeric_limits<double>::max());
}

// Test that f64 has greater precision than f32
TEST(FloatConstants, precision_comparison) {
    EXPECT_LT(corelib::f64_min, static_cast<double>(corelib::f32_min));
    EXPECT_GT(corelib::f64_max, static_cast<double>(corelib::f32_max));
}

// Test that constants are normal (not denormalized/subnormal)
TEST(FloatConstants, normal_values) {
    EXPECT_TRUE(std::isnormal(corelib::f32_min));
    EXPECT_TRUE(std::isnormal(corelib::f32_max));
    EXPECT_TRUE(std::isnormal(corelib::f64_min));
    EXPECT_TRUE(std::isnormal(corelib::f64_max));
}

// Test that constants are finite
TEST(FloatConstants, finite_values) {
    EXPECT_TRUE(std::isfinite(corelib::f32_min));
    EXPECT_TRUE(std::isfinite(corelib::f32_max));
    EXPECT_TRUE(std::isfinite(corelib::f64_min));
    EXPECT_TRUE(std::isfinite(corelib::f64_max));
}

// Test that negated max values represent the most negative values
TEST(FloatConstants, negative_ranges) {
    EXPECT_EQ(-corelib::f32_max, std::numeric_limits<float>::lowest());
    EXPECT_EQ(-corelib::f64_max, std::numeric_limits<double>::lowest());
}

// Test that constants can be used in arithmetic
TEST(FloatConstants, arithmetic_operations) {
    // Test that min values can be multiplied and divided
    EXPECT_GT(corelib::f32_min * 2.0f, corelib::f32_min);
    EXPECT_LT(corelib::f32_min / 2.0f, corelib::f32_min);

    // Test that max values maintain their properties in operations
    EXPECT_LT(corelib::f32_max / 2.0f, corelib::f32_max);
    EXPECT_LT(corelib::f64_max / 2.0, corelib::f64_max);
}

// Test approximate value ranges
TEST(FloatConstants, value_ranges) {
    // f32_min should be in the correct order of magnitude
    EXPECT_GT(corelib::f32_min, 1.0e-39f);
    EXPECT_LT(corelib::f32_min, 1.0e-37f);

    // f32_max should be in the correct order of magnitude
    EXPECT_GT(corelib::f32_max, 3.0e+38f);
    EXPECT_EQ(corelib::f32_max, std::numeric_limits<float>::max());

    // f64_min should be in the correct order of magnitude
    EXPECT_GT(corelib::f64_min, 1.0e-309);
    EXPECT_LT(corelib::f64_min, 1.0e-307);

    // f64_max should be in the correct order of magnitude
    EXPECT_GT(corelib::f64_max, 1.0e308);
    EXPECT_EQ(corelib::f64_max, std::numeric_limits<double>::max());
}

// Test type consistency
TEST(FloatConstants, type_consistency) {
    // Verify that constants have the correct types
    static_assert(std::is_same_v<decltype(corelib::f32_min), const float>);
    static_assert(std::is_same_v<decltype(corelib::f32_max), const float>);
    static_assert(std::is_same_v<decltype(corelib::f64_min), const double>);
    static_assert(std::is_same_v<decltype(corelib::f64_max), const double>);
}

// Test that min is NOT the most negative value (common misunderstanding)
TEST(FloatConstants, min_is_not_lowest) {
    // For floating-point types, min() returns the smallest positive normalized value,
    // not the most negative value (which is -max())
    EXPECT_NE(corelib::f32_min, std::numeric_limits<float>::lowest());
    EXPECT_NE(corelib::f64_min, std::numeric_limits<double>::lowest());

    // The most negative values are -max()
    EXPECT_EQ(-corelib::f32_max, std::numeric_limits<float>::lowest());
    EXPECT_EQ(-corelib::f64_max, std::numeric_limits<double>::lowest());
}

// Test relationship with denormalized (subnormal) values
TEST(FloatConstants, denorm_comparison) {
    // min() is larger than denorm_min() (the smallest subnormal value)
    EXPECT_GT(corelib::f32_min, std::numeric_limits<float>::denorm_min());
    EXPECT_GT(corelib::f64_min, std::numeric_limits<double>::denorm_min());
}
