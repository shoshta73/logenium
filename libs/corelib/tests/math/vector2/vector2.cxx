// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <cmath>

#include <gtest/gtest.h>

#include <corelib/math/vector2.hxx>

using namespace corelib::math;

// Test default constructor
TEST(Vec2, default_constructor) {
    vec2<float> v;
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
}

// Test uniform value constructor
TEST(Vec2, uniform_value_constructor) {
    vec2<float> v{5.0f};
    EXPECT_EQ(v.x, 5.0f);
    EXPECT_EQ(v.y, 5.0f);
}

// Test component constructor
TEST(Vec2, component_constructor) {
    vec2<float> v{3.0f, 4.0f};
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
}

// Test naming schemes
TEST(Vec2, naming_schemes_xy) {
    vec2<float> v{3.0f, 4.0f};
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
}

TEST(Vec2, naming_schemes_rg) {
    vec2<float> v{0.5f, 0.75f};
    EXPECT_EQ(v.r, 0.5f);
    EXPECT_EQ(v.g, 0.75f);

    v.r = 1.0f;
    EXPECT_EQ(v.x, 1.0f);

    v.g = 0.25f;
    EXPECT_EQ(v.y, 0.25f);
}

TEST(Vec2, naming_schemes_st) {
    vec2<float> v{0.1f, 0.9f};
    EXPECT_EQ(v.s, 0.1f);
    EXPECT_EQ(v.t, 0.9f);

    v.s = 0.3f;
    EXPECT_EQ(v.x, 0.3f);

    v.t = 0.7f;
    EXPECT_EQ(v.y, 0.7f);
}

// Test Zero factory
TEST(Vec2, zero_factory) {
    auto v = vec2<float>::Zero();
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
}

// Test One factory
TEST(Vec2, one_factory) {
    auto v = vec2<float>::One();
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 1.0f);
}

// Test addition operator
TEST(Vec2, addition_operator) {
    vec2<float> a{1.0f, 2.0f};
    vec2<float> b{3.0f, 4.0f};
    auto result = a + b;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 6.0f);
}

// Test subtraction operator
TEST(Vec2, subtraction_operator) {
    vec2<float> a{5.0f, 7.0f};
    vec2<float> b{2.0f, 3.0f};
    auto result = a - b;
    EXPECT_EQ(result.x, 3.0f);
    EXPECT_EQ(result.y, 4.0f);
}

// Test multiplication operator
TEST(Vec2, multiplication_operator) {
    vec2<float> a{2.0f, 3.0f};
    vec2<float> b{4.0f, 5.0f};
    auto result = a * b;
    EXPECT_EQ(result.x, 8.0f);
    EXPECT_EQ(result.y, 15.0f);
}

// Test division operator
TEST(Vec2, division_operator) {
    vec2<float> a{12.0f, 15.0f};
    vec2<float> b{3.0f, 5.0f};
    auto result = a / b;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 3.0f);
}

// Test compound addition
TEST(Vec2, compound_addition) {
    vec2<float> v{5.0f, 7.0f};
    v += vec2<float>{3.0f, 2.0f};
    EXPECT_EQ(v.x, 8.0f);
    EXPECT_EQ(v.y, 9.0f);
}

// Test compound subtraction
TEST(Vec2, compound_subtraction) {
    vec2<float> v{10.0f, 8.0f};
    v -= vec2<float>{3.0f, 2.0f};
    EXPECT_EQ(v.x, 7.0f);
    EXPECT_EQ(v.y, 6.0f);
}

// Test scalar multiplication
TEST(Vec2, scalar_multiplication) {
    vec2<float> v{3.0f, 4.0f};
    auto result = v * 2.0f;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 8.0f);
}

// Test scalar division
TEST(Vec2, scalar_division) {
    vec2<float> v{12.0f, 8.0f};
    auto result = v / 2.0f;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 4.0f);
}

// Test equality operator
TEST(Vec2, equality_operator) {
    vec2<float> a{5.0f, 7.0f};
    vec2<float> b{5.0f, 7.0f};
    vec2<float> c{3.0f, 7.0f};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

// Test inequality operator
TEST(Vec2, inequality_operator) {
    vec2<float> a{5.0f, 7.0f};
    vec2<float> b{3.0f, 4.0f};
    vec2<float> c{5.0f, 7.0f};

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != c);
}

// Test LengthSquared
TEST(Vec2, length_squared) {
    vec2<float> v{3.0f, 4.0f};
    EXPECT_EQ(v.LengthSquared(), 25.0f);
}

// Test Length
TEST(Vec2, length) {
    vec2<float> v{3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v.Length(), 5.0f);
}

// Test Normalize
TEST(Vec2, normalize) {
    vec2<float> v{3.0f, 4.0f};
    v.Normalize();
    EXPECT_FLOAT_EQ(v.x, 0.6f);
    EXPECT_FLOAT_EQ(v.y, 0.8f);
    EXPECT_FLOAT_EQ(v.Length(), 1.0f);
}

// Test Normalized
TEST(Vec2, normalized) {
    vec2<float> v{3.0f, 4.0f};
    auto result = v.Normalized();
    EXPECT_FLOAT_EQ(result.x, 0.6f);
    EXPECT_FLOAT_EQ(result.y, 0.8f);
    EXPECT_FLOAT_EQ(result.Length(), 1.0f);
    // Original unchanged
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
}

// Test Dot product
TEST(Vec2, dot_product) {
    vec2<float> a{1.0f, 2.0f};
    vec2<float> b{3.0f, 4.0f};
    EXPECT_EQ(vec2<float>::Dot(a, b), 11.0f);
}

// Test with integer type
TEST(Vec2, integer_type) {
    vec2<int> v{5, 7};
    EXPECT_EQ(v.x, 5);
    EXPECT_EQ(v.y, 7);

    auto result = v + vec2<int>{3, 2};
    EXPECT_EQ(result.x, 8);
    EXPECT_EQ(result.y, 9);
}

// Test with double type
TEST(Vec2, double_type) {
    vec2<double> v{3.5, 7.5};
    EXPECT_DOUBLE_EQ(v.x, 3.5);
    EXPECT_DOUBLE_EQ(v.y, 7.5);

    auto result = v * 2.0;
    EXPECT_DOUBLE_EQ(result.x, 7.0);
    EXPECT_DOUBLE_EQ(result.y, 15.0);
}

// Test constexpr operations
TEST(Vec2, constexpr_operations) {
    vec2<float> a{1.0f, 2.0f};
    vec2<float> b{3.0f, 4.0f};
    auto sum = a + b;
    auto product = a * 2.0f;
    auto dot = vec2<float>::Dot(a, b);
    auto lengthSq = a.LengthSquared();

    EXPECT_EQ(sum.x, 4.0f);
    EXPECT_EQ(sum.y, 6.0f);
    EXPECT_EQ(product.x, 2.0f);
    EXPECT_EQ(product.y, 4.0f);
    EXPECT_EQ(dot, 11.0f);
    EXPECT_EQ(lengthSq, 5.0f);
}

// Test perpendicular vectors have zero dot product
TEST(Vec2, perpendicular_vectors_dot_product) {
    vec2<float> a{1.0f, 0.0f};
    vec2<float> b{0.0f, 1.0f};
    EXPECT_EQ(vec2<float>::Dot(a, b), 0.0f);
}

// Test parallel vectors
TEST(Vec2, parallel_vectors) {
    vec2<float> a{2.0f, 4.0f};
    vec2<float> b{1.0f, 2.0f};
    auto normalized_a = a.Normalized();
    auto normalized_b = b.Normalized();
    EXPECT_FLOAT_EQ(normalized_a.x, normalized_b.x);
    EXPECT_FLOAT_EQ(normalized_a.y, normalized_b.y);
}
