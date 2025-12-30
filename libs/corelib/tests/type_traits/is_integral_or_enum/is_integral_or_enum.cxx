// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <corelib/type_traits/is_integral_or_enum.hxx>

namespace corelib {

enum TestEnum { Value1, Value2 };
enum class TestEnumClass { Value1, Value2 };

struct TestClass {
    operator int() const { return 0; }
};

TEST(IsIntegralOrEnumTest, IntType) {
    static_assert(is_integral_or_enum_v<int>, "int should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, UnsignedIntType) {
    static_assert(is_integral_or_enum_v<unsigned int>, "unsigned int should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, CharType) {
    static_assert(is_integral_or_enum_v<char>, "char should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, BoolType) {
    static_assert(is_integral_or_enum_v<bool>, "bool should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, LongLongType) {
    static_assert(is_integral_or_enum_v<long long>, "long long should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, EnumType) {
    static_assert(is_integral_or_enum_v<TestEnum>, "enum should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, EnumClassType) {
    static_assert(is_integral_or_enum_v<TestEnumClass>, "enum class should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, PointerType) {
    static_assert(!is_integral_or_enum_v<int *>, "pointer should not be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, FloatType) {
    static_assert(!is_integral_or_enum_v<float>, "float should not be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, DoubleType) {
    static_assert(!is_integral_or_enum_v<double>, "double should not be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, ClassWithConversionOperator) {
    static_assert(!is_integral_or_enum_v<TestClass>, "class with conversion operator should not be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, ReferenceType) {
    static_assert(is_integral_or_enum_v<int &>, "reference to int should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(IsIntegralOrEnumTest, ConstIntType) {
    static_assert(is_integral_or_enum_v<const int>, "const int should be integral or enum");
    SUCCEED() << "Copile time test we always succeed";
}

}  // namespace corelib
