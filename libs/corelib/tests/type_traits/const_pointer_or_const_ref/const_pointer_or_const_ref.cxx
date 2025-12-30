// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <type_traits>

#include <gtest/gtest.h>

#include <corelib/type_traits/const_pointer_or_const_ref.hxx>

namespace corelib {

TEST(ConstPointerOrConstRefTest, NonPointerType) {
    using Result = const_pointer_or_const_ref_t<int>;
    static_assert(std::is_same_v<Result, const int &>,
                  "const_pointer_or_const_ref should add const reference to non-pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(ConstPointerOrConstRefTest, PointerType) {
    using Result = const_pointer_or_const_ref_t<int *>;
    static_assert(std::is_same_v<Result, int *const>,
                  "const_pointer_or_const_ref should make pointer const for pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(ConstPointerOrConstRefTest, ConstNonPointerType) {
    using Result = const_pointer_or_const_ref_t<const int>;
    static_assert(std::is_same_v<Result, const int &>,
                  "const_pointer_or_const_ref should add reference to const non-pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(ConstPointerOrConstRefTest, ConstPointerType) {
    using Result = const_pointer_or_const_ref_t<int *const>;
    static_assert(std::is_same_v<Result, int *const>, "const_pointer_or_const_ref should preserve const pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(ConstPointerOrConstRefTest, PointerToConst) {
    using Result = const_pointer_or_const_ref_t<const int *>;
    static_assert(std::is_same_v<Result, const int *const>,
                  "const_pointer_or_const_ref should make pointer-to-const const");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(ConstPointerOrConstRefTest, ReferenceType) {
    using Result = const_pointer_or_const_ref_t<int &>;
    static_assert(std::is_same_v<Result, int &>, "const_pointer_or_const_ref should preserve reference types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(ConstPointerOrConstRefTest, DoublePointer) {
    using Result = const_pointer_or_const_ref_t<int **>;
    static_assert(std::is_same_v<Result, int **const>, "const_pointer_or_const_ref should make pointer const");
    SUCCEED() << "Copile time test we always succeed";
}

}  // namespace corelib
