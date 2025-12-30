// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <type_traits>

#include <gtest/gtest.h>

#include <corelib/type_traits/add_lvalue_reference_if_not_pointer.hxx>

namespace corelib {

TEST(AddLValueReferenceIfNotPointerTest, NonPointerType) {
    using Result = add_lvalue_reference_if_not_pointer_t<int>;
    static_assert(std::is_same_v<Result, int &>,
                  "add_lvalue_reference_if_not_pointer should add reference to non-pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddLValueReferenceIfNotPointerTest, PointerType) {
    using Result = add_lvalue_reference_if_not_pointer_t<int *>;
    static_assert(std::is_same_v<Result, int *>, "add_lvalue_reference_if_not_pointer should preserve pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddLValueReferenceIfNotPointerTest, ConstPointerType) {
    using Result = add_lvalue_reference_if_not_pointer_t<const int *>;
    static_assert(std::is_same_v<Result, const int *>,
                  "add_lvalue_reference_if_not_pointer should preserve const pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddLValueReferenceIfNotPointerTest, ConstNonPointerType) {
    using Result = add_lvalue_reference_if_not_pointer_t<const int>;
    static_assert(std::is_same_v<Result, const int &>,
                  "add_lvalue_reference_if_not_pointer should add reference to const non-pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddLValueReferenceIfNotPointerTest, AlreadyReference) {
    using Result = add_lvalue_reference_if_not_pointer_t<int &>;
    static_assert(std::is_same_v<Result, int &>, "add_lvalue_reference_if_not_pointer should preserve reference types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddLValueReferenceIfNotPointerTest, DoublePointer) {
    using Result = add_lvalue_reference_if_not_pointer_t<int **>;
    static_assert(std::is_same_v<Result, int **>,
                  "add_lvalue_reference_if_not_pointer should preserve double pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddLValueReferenceIfNotPointerTest, PointerToConst) {
    using Result = add_lvalue_reference_if_not_pointer_t<const int *>;
    static_assert(std::is_same_v<Result, const int *>,
                  "add_lvalue_reference_if_not_pointer should preserve pointer-to-const types");
    SUCCEED() << "Copile time test we always succeed";
}

}  // namespace corelib
