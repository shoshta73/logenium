// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <cmath>

#include <gtest/gtest.h>

#include <corelib/math/vector3.hxx>

using namespace corelib::math;

// Test default constructor
TEST(Vec3, default_constructor) {
    Vec3<float> v;
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, 0.0f);
}

// Test uniform value constructor
TEST(Vec3, uniform_value_constructor) {
    Vec3<float> v{5.0f};
    EXPECT_EQ(v.x, 5.0f);
    EXPECT_EQ(v.y, 5.0f);
    EXPECT_EQ(v.z, 5.0f);
}

// Test component constructor
TEST(Vec3, component_constructor) {
    Vec3<float> v{3.0f, 4.0f, 5.0f};
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
    EXPECT_EQ(v.z, 5.0f);
}

// Test naming schemes
TEST(Vec3, naming_schemes_xyz) {
    Vec3<float> v{3.0f, 4.0f, 5.0f};
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
    EXPECT_EQ(v.z, 5.0f);
}

TEST(Vec3, naming_schemes_rgb) {
    Vec3<float> v{0.5f, 0.75f, 1.0f};
    EXPECT_EQ(v.r, 0.5f);
    EXPECT_EQ(v.g, 0.75f);
    EXPECT_EQ(v.b, 1.0f);

    v.r = 1.0f;
    EXPECT_EQ(v.x, 1.0f);

    v.g = 0.25f;
    EXPECT_EQ(v.y, 0.25f);

    v.b = 0.5f;
    EXPECT_EQ(v.z, 0.5f);
}

TEST(Vec3, naming_schemes_stp) {
    Vec3<float> v{0.1f, 0.5f, 0.9f};
    EXPECT_EQ(v.s, 0.1f);
    EXPECT_EQ(v.t, 0.5f);
    EXPECT_EQ(v.p, 0.9f);

    v.s = 0.3f;
    EXPECT_EQ(v.x, 0.3f);

    v.t = 0.7f;
    EXPECT_EQ(v.y, 0.7f);

    v.p = 0.2f;
    EXPECT_EQ(v.z, 0.2f);
}

// Test Zero factory
TEST(Vec3, zero_factory) {
    auto v = Vec3<float>::Zero();
    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
    EXPECT_EQ(v.z, 0.0f);
}

// Test One factory
TEST(Vec3, one_factory) {
    auto v = Vec3<float>::One();
    EXPECT_EQ(v.x, 1.0f);
    EXPECT_EQ(v.y, 1.0f);
    EXPECT_EQ(v.z, 1.0f);
}

// Test addition operator
TEST(Vec3, addition_operator) {
    Vec3<float> a{1.0f, 2.0f, 3.0f};
    Vec3<float> b{4.0f, 5.0f, 6.0f};
    auto result = a + b;
    EXPECT_EQ(result.x, 5.0f);
    EXPECT_EQ(result.y, 7.0f);
    EXPECT_EQ(result.z, 9.0f);
}

// Test subtraction operator
TEST(Vec3, subtraction_operator) {
    Vec3<float> a{5.0f, 7.0f, 9.0f};
    Vec3<float> b{2.0f, 3.0f, 4.0f};
    auto result = a - b;
    EXPECT_EQ(result.x, 3.0f);
    EXPECT_EQ(result.y, 4.0f);
    EXPECT_EQ(result.z, 5.0f);
}

// Test multiplication operator
TEST(Vec3, multiplication_operator) {
    Vec3<float> a{2.0f, 3.0f, 4.0f};
    Vec3<float> b{5.0f, 6.0f, 7.0f};
    auto result = a * b;
    EXPECT_EQ(result.x, 10.0f);
    EXPECT_EQ(result.y, 18.0f);
    EXPECT_EQ(result.z, 28.0f);
}

// Test division operator
TEST(Vec3, division_operator) {
    Vec3<float> a{12.0f, 15.0f, 18.0f};
    Vec3<float> b{3.0f, 5.0f, 6.0f};
    auto result = a / b;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 3.0f);
    EXPECT_EQ(result.z, 3.0f);
}

// Test compound addition
TEST(Vec3, compound_addition) {
    Vec3<float> v{5.0f, 7.0f, 9.0f};
    v += Vec3<float>{3.0f, 2.0f, 1.0f};
    EXPECT_EQ(v.x, 8.0f);
    EXPECT_EQ(v.y, 9.0f);
    EXPECT_EQ(v.z, 10.0f);
}

// Test compound subtraction
TEST(Vec3, compound_subtraction) {
    Vec3<float> v{10.0f, 8.0f, 6.0f};
    v -= Vec3<float>{3.0f, 2.0f, 1.0f};
    EXPECT_EQ(v.x, 7.0f);
    EXPECT_EQ(v.y, 6.0f);
    EXPECT_EQ(v.z, 5.0f);
}

// Test scalar multiplication
TEST(Vec3, scalar_multiplication) {
    Vec3<float> v{3.0f, 4.0f, 5.0f};
    auto result = v * 2.0f;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 8.0f);
    EXPECT_EQ(result.z, 10.0f);
}

// Test scalar division
TEST(Vec3, scalar_division) {
    Vec3<float> v{12.0f, 8.0f, 16.0f};
    auto result = v / 2.0f;
    EXPECT_EQ(result.x, 6.0f);
    EXPECT_EQ(result.y, 4.0f);
    EXPECT_EQ(result.z, 8.0f);
}

// Test equality operator
TEST(Vec3, equality_operator) {
    Vec3<float> a{5.0f, 7.0f, 9.0f};
    Vec3<float> b{5.0f, 7.0f, 9.0f};
    Vec3<float> c{3.0f, 7.0f, 9.0f};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

// Test inequality operator
TEST(Vec3, inequality_operator) {
    Vec3<float> a{5.0f, 7.0f, 9.0f};
    Vec3<float> b{3.0f, 4.0f, 5.0f};
    Vec3<float> c{5.0f, 7.0f, 9.0f};

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a != c);
}

// Test LengthSquared
TEST(Vec3, length_squared) {
    Vec3<float> v{2.0f, 3.0f, 6.0f};
    EXPECT_EQ(v.LengthSquared(), 49.0f);  // 4 + 9 + 36 = 49
}

// Test Length
TEST(Vec3, length) {
    Vec3<float> v{2.0f, 3.0f, 6.0f};
    EXPECT_FLOAT_EQ(v.Length(), 7.0f);
}

// Test Normalize
TEST(Vec3, normalize) {
    Vec3<float> v{3.0f, 4.0f, 0.0f};
    v.Normalize();
    EXPECT_FLOAT_EQ(v.x, 0.6f);
    EXPECT_FLOAT_EQ(v.y, 0.8f);
    EXPECT_FLOAT_EQ(v.z, 0.0f);
    EXPECT_FLOAT_EQ(v.Length(), 1.0f);
}

// Test Normalized
TEST(Vec3, normalized) {
    Vec3<float> v{3.0f, 4.0f, 0.0f};
    auto result = v.Normalized();
    EXPECT_FLOAT_EQ(result.x, 0.6f);
    EXPECT_FLOAT_EQ(result.y, 0.8f);
    EXPECT_FLOAT_EQ(result.z, 0.0f);
    EXPECT_FLOAT_EQ(result.Length(), 1.0f);
    // Original unchanged
    EXPECT_EQ(v.x, 3.0f);
    EXPECT_EQ(v.y, 4.0f);
    EXPECT_EQ(v.z, 0.0f);
}

// Test Dot product
TEST(Vec3, dot_product) {
    Vec3<float> a{1.0f, 2.0f, 3.0f};
    Vec3<float> b{4.0f, 5.0f, 6.0f};
    EXPECT_EQ(Vec3<float>::Dot(a, b), 32.0f);  // 4 + 10 + 18 = 32
}

// Test Cross product - standard basis vectors
TEST(Vec3, cross_product_basis_vectors) {
    Vec3<float> x_axis{1.0f, 0.0f, 0.0f};
    Vec3<float> y_axis{0.0f, 1.0f, 0.0f};
    Vec3<float> z_axis{0.0f, 0.0f, 1.0f};

    // x cross y = z
    auto result1 = Vec3<float>::Cross(x_axis, y_axis);
    EXPECT_FLOAT_EQ(result1.x, 0.0f);
    EXPECT_FLOAT_EQ(result1.y, 0.0f);
    EXPECT_FLOAT_EQ(result1.z, 1.0f);

    // y cross z = x
    auto result2 = Vec3<float>::Cross(y_axis, z_axis);
    EXPECT_FLOAT_EQ(result2.x, 1.0f);
    EXPECT_FLOAT_EQ(result2.y, 0.0f);
    EXPECT_FLOAT_EQ(result2.z, 0.0f);

    // z cross x = y
    auto result3 = Vec3<float>::Cross(z_axis, x_axis);
    EXPECT_FLOAT_EQ(result3.x, 0.0f);
    EXPECT_FLOAT_EQ(result3.y, 1.0f);
    EXPECT_FLOAT_EQ(result3.z, 0.0f);
}

// Test Cross product - anti-commutativity
TEST(Vec3, cross_product_anti_commutative) {
    Vec3<float> a{1.0f, 2.0f, 3.0f};
    Vec3<float> b{4.0f, 5.0f, 6.0f};

    auto cross_ab = Vec3<float>::Cross(a, b);
    auto cross_ba = Vec3<float>::Cross(b, a);

    EXPECT_FLOAT_EQ(cross_ab.x, -cross_ba.x);
    EXPECT_FLOAT_EQ(cross_ab.y, -cross_ba.y);
    EXPECT_FLOAT_EQ(cross_ab.z, -cross_ba.z);
}

// Test Cross product - perpendicularity
TEST(Vec3, cross_product_perpendicular) {
    Vec3<float> a{1.0f, 2.0f, 3.0f};
    Vec3<float> b{4.0f, 5.0f, 6.0f};
    auto cross = Vec3<float>::Cross(a, b);

    // Cross product should be perpendicular to both vectors
    EXPECT_FLOAT_EQ(Vec3<float>::Dot(cross, a), 0.0f);
    EXPECT_FLOAT_EQ(Vec3<float>::Dot(cross, b), 0.0f);
}

// Test Cross product - parallel vectors
TEST(Vec3, cross_product_parallel_vectors) {
    Vec3<float> a{2.0f, 4.0f, 6.0f};
    Vec3<float> b{1.0f, 2.0f, 3.0f};  // Parallel to a
    auto cross = Vec3<float>::Cross(a, b);

    // Cross product of parallel vectors is zero
    EXPECT_FLOAT_EQ(cross.x, 0.0f);
    EXPECT_FLOAT_EQ(cross.y, 0.0f);
    EXPECT_FLOAT_EQ(cross.z, 0.0f);
}

// Test Cross product - specific values
TEST(Vec3, cross_product_specific_values) {
    Vec3<float> a{1.0f, 2.0f, 3.0f};
    Vec3<float> b{4.0f, 5.0f, 6.0f};
    auto cross = Vec3<float>::Cross(a, b);

    EXPECT_FLOAT_EQ(cross.x, -3.0f);  // 2*6 - 3*5 = 12 - 15 = -3
    EXPECT_FLOAT_EQ(cross.y, 6.0f);   // 3*4 - 1*6 = 12 - 6 = 6
    EXPECT_FLOAT_EQ(cross.z, -3.0f);  // 1*5 - 2*4 = 5 - 8 = -3
}

// Test with integer type
TEST(Vec3, integer_type) {
    Vec3<int> v{5, 7, 9};
    EXPECT_EQ(v.x, 5);
    EXPECT_EQ(v.y, 7);
    EXPECT_EQ(v.z, 9);

    auto result = v + Vec3<int>{3, 2, 1};
    EXPECT_EQ(result.x, 8);
    EXPECT_EQ(result.y, 9);
    EXPECT_EQ(result.z, 10);
}

// Test with double type
TEST(Vec3, double_type) {
    Vec3<double> v{3.5, 7.5, 9.5};
    EXPECT_DOUBLE_EQ(v.x, 3.5);
    EXPECT_DOUBLE_EQ(v.y, 7.5);
    EXPECT_DOUBLE_EQ(v.z, 9.5);

    auto result = v * 2.0;
    EXPECT_DOUBLE_EQ(result.x, 7.0);
    EXPECT_DOUBLE_EQ(result.y, 15.0);
    EXPECT_DOUBLE_EQ(result.z, 19.0);
}

// Test constexpr operations
TEST(Vec3, constexpr_operations) {
    constexpr Vec3<float> a{1.0f, 2.0f, 3.0f};
    constexpr Vec3<float> b{4.0f, 5.0f, 6.0f};
    constexpr auto sum = a + b;
    constexpr auto product = a * 2.0f;
    constexpr auto dot = Vec3<float>::Dot(a, b);
    constexpr auto cross = Vec3<float>::Cross(a, b);
    constexpr auto lengthSq = a.LengthSquared();

    EXPECT_EQ(sum.x, 5.0f);
    EXPECT_EQ(sum.y, 7.0f);
    EXPECT_EQ(sum.z, 9.0f);
    EXPECT_EQ(product.x, 2.0f);
    EXPECT_EQ(product.y, 4.0f);
    EXPECT_EQ(product.z, 6.0f);
    EXPECT_EQ(dot, 32.0f);
    EXPECT_EQ(cross.x, -3.0f);
    EXPECT_EQ(cross.y, 6.0f);
    EXPECT_EQ(cross.z, -3.0f);
    EXPECT_EQ(lengthSq, 14.0f);
}

// Test perpendicular vectors have zero dot product
TEST(Vec3, perpendicular_vectors_dot_product) {
    Vec3<float> a{1.0f, 0.0f, 0.0f};
    Vec3<float> b{0.0f, 1.0f, 0.0f};
    EXPECT_EQ(Vec3<float>::Dot(a, b), 0.0f);
}
