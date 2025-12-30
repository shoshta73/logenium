// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <memory>
#include <optional>
#include <type_traits>

#include <gtest/gtest.h>

#include <corelib/detail/casting/traits.hxx>

namespace corelib::detail {

TEST(SimplifyTypeTest, BasicType) {
    using Result = SimplifyType<int>::SimpleType;
    static_assert(std::is_same_v<Result, int>, "SimplifyType should preserve basic types");
}

TEST(SimplifyTypeTest, PointerType) {
    using Result = SimplifyType<int *>::SimpleType;
    static_assert(std::is_same_v<Result, int *>, "SimplifyType should preserve pointer types");
}

TEST(SimplifyTypeTest, ConstType) {
    using Result = SimplifyType<const int>::SimpleType;
    static_assert(std::is_same_v<Result, const int>, "SimplifyType should preserve const on non-pointer types");
}

TEST(SimplifyTypeTest, ConstPointerType) {
    using Result = SimplifyType<const int *>::SimpleType;
    static_assert(std::is_same_v<Result, const int *>, "SimplifyType should preserve const pointer types");
}

TEST(SimplifyTypeTest, PointerToConst) {
    using Result = SimplifyType<int *const>::SimpleType;
    static_assert(std::is_same_v<Result, const int *>, "SimplifyType should add const past pointer for const pointer");
}

TEST(SimplifyTypeTest, GetSimplifiedValueBasic) {
    int x = 42;
    int &result = SimplifyType<int>::GetSimplifiedValue(x);
    EXPECT_EQ(result, 42);
}

TEST(SimplifyTypeTest, GetSimplifiedValueConst) {
    const int x = 42;
    const int &result = SimplifyType<const int>::GetSimplifiedValue(x);
    EXPECT_EQ(result, 42);
}

TEST(IsSimpleTypeTest, BasicType) {
    static_assert(IsSimpleType<int>::value, "int should be a simple type");
}

TEST(IsSimpleTypeTest, PointerType) {
    static_assert(IsSimpleType<int *>::value, "int* should be a simple type");
}

TEST(IsSimpleTypeTest, ConstType) {
    static_assert(IsSimpleType<const int>::value, "const int should be a simple type");
}

TEST(IsNullableTest, PointerType) {
    static_assert(IsNullable<int *>, "pointer should be nullable");
}

TEST(IsNullableTest, ConstPointerType) {
    static_assert(IsNullable<const int *>, "const pointer should be nullable");
}

TEST(IsNullableTest, UniquePtrType) {
    static_assert(IsNullable<std::unique_ptr<int>>, "unique_ptr should be nullable");
}

TEST(IsNullableTest, NonNullableType) {
    static_assert(!IsNullable<int>, "int should not be nullable");
}

TEST(IsNullableTest, ReferenceType) {
    static_assert(!IsNullable<int &>, "reference should not be nullable");
}

TEST(ValueIsPresentTest, NonNullableType) {
    int x = 42;
    EXPECT_TRUE(ValueIsPresent<int>::IsPresent(x));
}

TEST(ValueIsPresentTest, NullPointer) {
    int *ptr = nullptr;
    EXPECT_FALSE(ValueIsPresent<int *>::IsPresent(ptr));
}

TEST(ValueIsPresentTest, NonNullPointer) {
    int x = 42;
    int *ptr = &x;
    EXPECT_TRUE(ValueIsPresent<int *>::IsPresent(ptr));
}

TEST(ValueIsPresentTest, EmptyOptional) {
    std::optional<int> opt = std::nullopt;
    EXPECT_FALSE(ValueIsPresent<std::optional<int>>::IsPresent(opt));
}

TEST(ValueIsPresentTest, NonEmptyOptional) {
    std::optional<int> opt = 42;
    EXPECT_TRUE(ValueIsPresent<std::optional<int>>::IsPresent(opt));
}

TEST(ValueIsPresentTest, UnwrapValue) {
    int x = 42;
    int &result = ValueIsPresent<int>::UnwrapValue(x);
    EXPECT_EQ(result, 42);
}

TEST(ValueIsPresentTest, UnwrapPointer) {
    int x = 42;
    int *ptr = &x;
    int *result = ValueIsPresent<int *>::UnwrapValue(ptr);
    EXPECT_EQ(result, ptr);
}

TEST(ValueIsPresentTest, UnwrapOptional) {
    std::optional<int> opt = 42;
    int &result = ValueIsPresent<std::optional<int>>::UnwrapValue(opt);
    EXPECT_EQ(result, 42);
}

TEST(IsPresentFunctionTest, BasicType) {
    int x = 42;
    EXPECT_TRUE(isPresent(x));
}

TEST(IsPresentFunctionTest, NullPointer) {
    int *ptr = nullptr;
    EXPECT_FALSE(isPresent(ptr));
}

TEST(IsPresentFunctionTest, NonNullPointer) {
    int x = 42;
    int *ptr = &x;
    EXPECT_TRUE(isPresent(ptr));
}

TEST(IsPresentFunctionTest, EmptyOptional) {
    std::optional<int> opt = std::nullopt;
    EXPECT_FALSE(isPresent(opt));
}

TEST(IsPresentFunctionTest, NonEmptyOptional) {
    std::optional<int> opt = 42;
    EXPECT_TRUE(isPresent(opt));
}

TEST(UnwrapValueFunctionTest, BasicType) {
    int x = 42;
    int &result = unwrapValue(x);
    EXPECT_EQ(result, 42);
}

TEST(UnwrapValueFunctionTest, Pointer) {
    int x = 42;
    int *ptr = &x;
    int *result = unwrapValue(ptr);
    EXPECT_EQ(result, ptr);
}

TEST(UnwrapValueFunctionTest, Optional) {
    std::optional<int> opt = 42;
    int &result = unwrapValue(opt);
    EXPECT_EQ(result, 42);
}

TEST(SelfTypeTest, VoidDefault) {
    using Result = SelfType<void, int>;
    static_assert(std::is_same_v<Result, int>, "SelfType should return default when OptionalDerived is void");
}

TEST(SelfTypeTest, NonVoidOptional) {
    using Result = SelfType<double, int>;
    static_assert(std::is_same_v<Result, double>, "SelfType should return OptionalDerived when not void");
}

}  // namespace corelib::detail
