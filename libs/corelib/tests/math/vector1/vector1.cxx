// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <corelib/math/vector1.hxx>

using namespace corelib::math;

// Test default constructor
TEST(Vec1, default_constructor) {
    vec1<float> v;
    EXPECT_EQ(v.x, 0.0f);
}

// Test value constructor
TEST(Vec1, value_constructor) {
    vec1<float> v{5.0f};
    EXPECT_EQ(v.x, 5.0f);
}

// Test naming schemes
TEST(Vec1, naming_schemes) {
    vec1<float> v{3.0f};
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.r, 3.0f);
    EXPECT_EQ(v.s, 3.0f);

    v.r = 7.0f;
    EXPECT_EQ(v.x, 7.0f);

    v.s = 9.0f;
    EXPECT_EQ(v.x, 9.0f);
}

// Test Zero factory
TEST(Vec1, zero_factory) {
    auto v = vec1<float>::Zero();
    EXPECT_EQ(v.x, 0.0f);
}

// Test One factory
TEST(Vec1, one_factory) {
    auto v = vec1<float>::One();
    EXPECT_EQ(v.x, 1.0f);
}

// Test addition operator
TEST(Vec1, addition_operator) {
    vec1<float> a{3.0f};
    vec1<float> b{4.0f};
    auto result = a + b;
    EXPECT_EQ(result.x, 7.0f);
}

// Test subtraction operator
TEST(Vec1, subtraction_operator) {
    vec1<float> a{10.0f};
    vec1<float> b{4.0f};
    auto result = a - b;
    EXPECT_EQ(result.x, 6.0f);
}

// Test multiplication operator
TEST(Vec1, multiplication_operator) {
    vec1<float> a{3.0f};
    vec1<float> b{4.0f};
    auto result = a * b;
    EXPECT_EQ(result.x, 12.0f);
}

// Test division operator
TEST(Vec1, division_operator) {
    vec1<float> a{12.0f};
    vec1<float> b{3.0f};
    auto result = a / b;
    EXPECT_EQ(result.x, 4.0f);
}

// Test compound addition
TEST(Vec1, compound_addition) {
    vec1<float> v{5.0f};
    v += vec1<float>{3.0f};
    EXPECT_EQ(v.x, 8.0f);
}

// Test compound subtraction
TEST(Vec1, compound_subtraction) {
    vec1<float> v{10.0f};
    v -= vec1<float>{3.0f};
    EXPECT_EQ(v.x, 7.0f);
}

// Test scalar multiplication
TEST(Vec1, scalar_multiplication) {
    vec1<float> v{3.0f};
    auto result = v * 4.0f;
    EXPECT_EQ(result.x, 12.0f);
}

// Test scalar division
TEST(Vec1, scalar_division) {
    vec1<float> v{12.0f};
    auto result = v / 3.0f;
    EXPECT_EQ(result.x, 4.0f);
}

// Test equality operator
TEST(Vec1, equality_operator) {
    vec1<float> a{5.0f};
    vec1<float> b{5.0f};
    vec1<float> c{3.0f};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

// Test inequality operator
TEST(Vec1, inequality_operator) {
    vec1<float> a{5.0f};
    vec1<float> b{3.0f};
    vec1<float> c{5.0f};

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != c);
}

// Test LengthSquared
TEST(Vec1, length_squared) {
    vec1<float> v{4.0f};
    EXPECT_EQ(v.LengthSquared(), 16.0f);
}

// Test Length with positive value
TEST(Vec1, length_positive) {
    vec1<float> v{5.0f};
    EXPECT_EQ(v.Length(), 5.0f);
}

// Test Length with negative value
TEST(Vec1, length_negative) {
    vec1<float> v{-5.0f};
    EXPECT_EQ(v.Length(), 5.0f);
}

// Test Normalize with positive value
TEST(Vec1, normalize_positive) {
    vec1<float> v{5.0f};
    v.Normalize();
    EXPECT_EQ(v.x, 1.0f);
}

// Test Normalize with negative value
TEST(Vec1, normalize_negative) {
    vec1<float> v{-5.0f};
    v.Normalize();
    EXPECT_EQ(v.x, -1.0f);
}

// Test Normalize with zero
TEST(Vec1, normalize_zero) {
    vec1<float> v{0.0f};
    v.Normalize();
    EXPECT_EQ(v.x, 0.0f);
}

// Test Normalized with positive value
TEST(Vec1, normalized_positive) {
    vec1<float> v{5.0f};
    auto result = v.Normalized();
    EXPECT_EQ(result.x, 1.0f);
    EXPECT_EQ(v.x, 5.0f);  // Original unchanged
}

// Test Normalized with negative value
TEST(Vec1, normalized_negative) {
    vec1<float> v{-5.0f};
    auto result = v.Normalized();
    EXPECT_EQ(result.x, -1.0f);
    EXPECT_EQ(v.x, -5.0f);  // Original unchanged
}

// Test Normalized with zero
TEST(Vec1, normalized_zero) {
    vec1<float> v{0.0f};
    auto result = v.Normalized();
    EXPECT_EQ(result.x, 0.0f);
}

// Test Dot product
TEST(Vec1, dot_product) {
    vec1<float> a{3.0f};
    vec1<float> b{4.0f};
    EXPECT_EQ(vec1<float>::Dot(a, b), 12.0f);
}

// Test with integer type
TEST(Vec1, integer_type) {
    vec1<int> v{5};
    EXPECT_EQ(v.x, 5);

    auto result = v + vec1<int>{3};
    EXPECT_EQ(result.x, 8);
}

// Test with double type
TEST(Vec1, double_type) {
    vec1<double> v{3.5};
    EXPECT_DOUBLE_EQ(v.x, 3.5);

    auto result = v * 2.0;
    EXPECT_DOUBLE_EQ(result.x, 7.0);
}

// Test constexpr operations
TEST(Vec1, constexpr_operations) {
    vec1<float> a{3.0f};
    vec1<float> b{4.0f};
    auto sum = a + b;
    auto product = a * 2.0f;
    auto dot = vec1<float>::Dot(a, b);

    EXPECT_EQ(sum.x, 7.0f);
    EXPECT_EQ(product.x, 6.0f);
    EXPECT_EQ(dot, 12.0f);
}

// Test type aliases
TEST(Vec1, type_aliases) {
    vec1 d{3.5};
    EXPECT_DOUBLE_EQ(d.x, 3.5);

    vec1 i{5};
    EXPECT_EQ(i.x, 5);

    vec1 u{10u};
    EXPECT_EQ(u.x, 10u);
}
