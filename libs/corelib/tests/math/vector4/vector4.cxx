// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <cmath>

#include <gtest/gtest.h>

#include <corelib/math/vector4.hxx>

using namespace corelib::math;

// Test default constructor
TEST(Vec4, default_constructor) {
    Vec4<float> v;
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, 0.0f);
    EXPECT_EQ(v.w, 0.0f);
}

// Test uniform value constructor
TEST(Vec4, uniform_value_constructor) {
    Vec4<float> v{5.0f};
    EXPECT_EQ(v.x, 5.0f);
    EXPECT_EQ(v.y, 5.0f);
    EXPECT_EQ(v.z, 5.0f);
    EXPECT_EQ(v.w, 5.0f);
}

// Test component constructor
TEST(Vec4, component_constructor) {
    Vec4<float> v{3.0f, 4.0f, 5.0f, 6.0f};
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
    EXPECT_EQ(v.z, 5.0f);
    EXPECT_EQ(v.w, 6.0f);
}

// Test naming schemes
TEST(Vec4, naming_schemes_xyzw) {
    Vec4<float> v{3.0f, 4.0f, 5.0f, 6.0f};
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
    EXPECT_EQ(v.z, 5.0f);
    EXPECT_EQ(v.w, 6.0f);
}

TEST(Vec4, naming_schemes_rgba) {
    Vec4<float> v{0.5f, 0.75f, 1.0f, 0.25f};
    EXPECT_EQ(v.r, 0.5f);
    EXPECT_EQ(v.g, 0.75f);
    EXPECT_EQ(v.b, 1.0f);
    EXPECT_EQ(v.a, 0.25f);

    v.r = 1.0f;
    EXPECT_EQ(v.x, 1.0f);

    v.g = 0.25f;
    EXPECT_EQ(v.y, 0.25f);

    v.b = 0.5f;
    EXPECT_EQ(v.z, 0.5f);

    v.a = 0.75f;
    EXPECT_EQ(v.w, 0.75f);
}

TEST(Vec4, naming_schemes_stpq) {
    Vec4<float> v{0.1f, 0.3f, 0.5f, 0.7f};
    EXPECT_EQ(v.s, 0.1f);
    EXPECT_EQ(v.t, 0.3f);
    EXPECT_EQ(v.p, 0.5f);
    EXPECT_EQ(v.q, 0.7f);

    v.s = 0.2f;
    EXPECT_EQ(v.x, 0.2f);

    v.t = 0.4f;
    EXPECT_EQ(v.y, 0.4f);

    v.p = 0.6f;
    EXPECT_EQ(v.z, 0.6f);

    v.q = 0.8f;
    EXPECT_EQ(v.w, 0.8f);
}

// Test Zero factory
TEST(Vec4, zero_factory) {
    auto v = Vec4<float>::Zero();
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, 0.0f);
    EXPECT_EQ(v.w, 0.0f);
}

// Test One factory
TEST(Vec4, one_factory) {
    auto v = Vec4<float>::One();
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 1.0f);
    EXPECT_EQ(v.z, 1.0f);
    EXPECT_EQ(v.w, 1.0f);
}

// Test addition operator
TEST(Vec4, addition_operator) {
    Vec4<float> a{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4<float> b{5.0f, 6.0f, 7.0f, 8.0f};
    auto result = a + b;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 8.0f);
    EXPECT_EQ(result.z, 10.0f);
    EXPECT_EQ(result.w, 12.0f);
}

// Test subtraction operator
TEST(Vec4, subtraction_operator) {
    Vec4<float> a{10.0f, 9.0f, 8.0f, 7.0f};
    Vec4<float> b{2.0f, 3.0f, 4.0f, 5.0f};
    auto result = a - b;
    EXPECT_EQ(result.x, 8.0f);
    EXPECT_EQ(result.y, 6.0f);
    EXPECT_EQ(result.z, 4.0f);
    EXPECT_EQ(result.w, 2.0f);
}

// Test multiplication operator
TEST(Vec4, multiplication_operator) {
    Vec4<float> a{2.0f, 3.0f, 4.0f, 5.0f};
    Vec4<float> b{6.0f, 7.0f, 8.0f, 9.0f};
    auto result = a * b;
    EXPECT_EQ(result.x, 12.0f);
    EXPECT_EQ(result.y, 21.0f);
    EXPECT_EQ(result.z, 32.0f);
    EXPECT_EQ(result.w, 45.0f);
}

// Test division operator
TEST(Vec4, division_operator) {
    Vec4<float> a{12.0f, 15.0f, 18.0f, 24.0f};
    Vec4<float> b{3.0f, 5.0f, 6.0f, 8.0f};
    auto result = a / b;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 3.0f);
    EXPECT_EQ(result.z, 3.0f);
    EXPECT_EQ(result.w, 3.0f);
}

// Test compound addition
TEST(Vec4, compound_addition) {
    Vec4<float> v{5.0f, 7.0f, 9.0f, 11.0f};
    v += Vec4<float>{3.0f, 2.0f, 1.0f, 4.0f};
    EXPECT_EQ(v.x, 8.0f);
    EXPECT_EQ(v.y, 9.0f);
    EXPECT_EQ(v.z, 10.0f);
    EXPECT_EQ(v.w, 15.0f);
}

// Test compound subtraction
TEST(Vec4, compound_subtraction) {
    Vec4<float> v{10.0f, 8.0f, 6.0f, 12.0f};
    v -= Vec4<float>{3.0f, 2.0f, 1.0f, 4.0f};
    EXPECT_EQ(v.x, 7.0f);
    EXPECT_EQ(v.y, 6.0f);
    EXPECT_EQ(v.z, 5.0f);
    EXPECT_EQ(v.w, 8.0f);
}

// Test scalar multiplication
TEST(Vec4, scalar_multiplication) {
    Vec4<float> v{3.0f, 4.0f, 5.0f, 6.0f};
    auto result = v * 2.0f;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 8.0f);
    EXPECT_EQ(result.z, 10.0f);
    EXPECT_EQ(result.w, 12.0f);
}

// Test scalar division
TEST(Vec4, scalar_division) {
    Vec4<float> v{12.0f, 8.0f, 16.0f, 20.0f};
    auto result = v / 2.0f;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 4.0f);
    EXPECT_EQ(result.z, 8.0f);
    EXPECT_EQ(result.w, 10.0f);
}

// Test equality operator
TEST(Vec4, equality_operator) {
    Vec4<float> a{5.0f, 7.0f, 9.0f, 11.0f};
    Vec4<float> b{5.0f, 7.0f, 9.0f, 11.0f};
    Vec4<float> c{3.0f, 7.0f, 9.0f, 11.0f};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

// Test inequality operator
TEST(Vec4, inequality_operator) {
    Vec4<float> a{5.0f, 7.0f, 9.0f, 11.0f};
    Vec4<float> b{3.0f, 4.0f, 5.0f, 6.0f};
    Vec4<float> c{5.0f, 7.0f, 9.0f, 11.0f};

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != c);
}

// Test LengthSquared
TEST(Vec4, length_squared) {
    Vec4<float> v{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_EQ(v.LengthSquared(), 30.0f);  // 1 + 4 + 9 + 16 = 30
}

// Test Length
TEST(Vec4, length) {
    Vec4<float> v{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v.Length(), std::sqrt(30.0f));
}

// Test Normalize
TEST(Vec4, normalize) {
    Vec4<float> v{1.0f, 2.0f, 3.0f, 4.0f};
    float original_length = v.Length();
    v.Normalize();
    EXPECT_FLOAT_EQ(v.x, 1.0f / original_length);
    EXPECT_FLOAT_EQ(v.y, 2.0f / original_length);
    EXPECT_FLOAT_EQ(v.z, 3.0f / original_length);
    EXPECT_FLOAT_EQ(v.w, 4.0f / original_length);
    EXPECT_FLOAT_EQ(v.Length(), 1.0f);
}

// Test Normalized
TEST(Vec4, normalized) {
    Vec4<float> v{1.0f, 2.0f, 3.0f, 4.0f};
    float original_length = v.Length();
    auto result = v.Normalized();
    EXPECT_FLOAT_EQ(result.x, 1.0f / original_length);
    EXPECT_FLOAT_EQ(result.y, 2.0f / original_length);
    EXPECT_FLOAT_EQ(result.z, 3.0f / original_length);
    EXPECT_FLOAT_EQ(result.w, 4.0f / original_length);
    EXPECT_FLOAT_EQ(result.Length(), 1.0f);
    // Original unchanged
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 2.0f);
    EXPECT_EQ(v.z, 3.0f);
    EXPECT_EQ(v.w, 4.0f);
}

// Test Dot product
TEST(Vec4, dot_product) {
    Vec4<float> a{1.0f, 2.0f, 3.0f, 4.0f};
    Vec4<float> b{5.0f, 6.0f, 7.0f, 8.0f};
    EXPECT_EQ(Vec4<float>::Dot(a, b), 70.0f);  // 5 + 12 + 21 + 32 = 70
}

// Test with integer type
TEST(Vec4, integer_type) {
    Vec4<int> v{5, 7, 9, 11};
    EXPECT_EQ(v.x, 5);
    EXPECT_EQ(v.y, 7);
    EXPECT_EQ(v.z, 9);
    EXPECT_EQ(v.w, 11);

    auto result = v + Vec4<int>{3, 2, 1, 4};
    EXPECT_EQ(result.x, 8);
    EXPECT_EQ(result.y, 9);
    EXPECT_EQ(result.z, 10);
    EXPECT_EQ(result.w, 15);
}

// Test with double type
TEST(Vec4, double_type) {
    Vec4<double> v{3.5, 7.5, 9.5, 11.5};
    EXPECT_DOUBLE_EQ(v.x, 3.5);
    EXPECT_DOUBLE_EQ(v.y, 7.5);
    EXPECT_DOUBLE_EQ(v.z, 9.5);
    EXPECT_DOUBLE_EQ(v.w, 11.5);

    auto result = v * 2.0;
    EXPECT_DOUBLE_EQ(result.x, 7.0);
    EXPECT_DOUBLE_EQ(result.y, 15.0);
    EXPECT_DOUBLE_EQ(result.z, 19.0);
    EXPECT_DOUBLE_EQ(result.w, 23.0);
}

// Test constexpr operations
TEST(Vec4, constexpr_operations) {
    constexpr Vec4<float> a{1.0f, 2.0f, 3.0f, 4.0f};
    constexpr Vec4<float> b{5.0f, 6.0f, 7.0f, 8.0f};
    constexpr auto sum = a + b;
    constexpr auto product = a * 2.0f;
    constexpr auto dot = Vec4<float>::Dot(a, b);
    constexpr auto lengthSq = a.LengthSquared();

    EXPECT_EQ(sum.x, 6.0f);
    EXPECT_EQ(sum.y, 8.0f);
    EXPECT_EQ(sum.z, 10.0f);
    EXPECT_EQ(sum.w, 12.0f);
    EXPECT_EQ(product.x, 2.0f);
    EXPECT_EQ(product.y, 4.0f);
    EXPECT_EQ(product.z, 6.0f);
    EXPECT_EQ(product.w, 8.0f);
    EXPECT_EQ(dot, 70.0f);
    EXPECT_EQ(lengthSq, 30.0f);
}

// Test perpendicular vectors have zero dot product
TEST(Vec4, perpendicular_vectors_dot_product) {
    Vec4<float> a{1.0f, 0.0f, 0.0f, 0.0f};
    Vec4<float> b{0.0f, 1.0f, 0.0f, 0.0f};
    EXPECT_EQ(Vec4<float>::Dot(a, b), 0.0f);
}

// Test RGBA color usage
TEST(Vec4, rgba_color_usage) {
    Vec4<float> color{1.0f, 0.5f, 0.25f, 1.0f};
    EXPECT_EQ(color.r, 1.0f);
    EXPECT_EQ(color.g, 0.5f);
    EXPECT_EQ(color.b, 0.25f);
    EXPECT_EQ(color.a, 1.0f);

    color.a = 0.5f;  // Set alpha
    EXPECT_EQ(color.w, 0.5f);
}
