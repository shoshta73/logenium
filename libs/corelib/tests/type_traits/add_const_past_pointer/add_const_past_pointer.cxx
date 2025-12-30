// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <type_traits>

#include <gtest/gtest.h>

#include <corelib/type_traits/add_const_past_pointer.hxx>

namespace corelib {

TEST(AddConstPastPointerTest, NonPointerType) {
    using Result = add_const_past_pointer_t<int>;
    static_assert(std::is_same_v<Result, const int>, "add_const_past_pointer should add const to non-pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddConstPastPointerTest, PointerType) {
    using Result = add_const_past_pointer_t<int *>;
    static_assert(std::is_same_v<Result, const int *>,
                  "add_const_past_pointer should add const past pointer for pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddConstPastPointerTest, ConstNonPointerType) {
    using Result = add_const_past_pointer_t<const int>;
    static_assert(std::is_same_v<Result, const int>,
                  "add_const_past_pointer should preserve const on non-pointer types");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddConstPastPointerTest, ConstPointerType) {
    using Result = add_const_past_pointer_t<int *const>;
    static_assert(std::is_same_v<Result, const int *>, "add_const_past_pointer should add const past pointer");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddConstPastPointerTest, PointerToConst) {
    using Result = add_const_past_pointer_t<const int *>;
    static_assert(std::is_same_v<Result, const int *>,
                  "add_const_past_pointer should preserve const in pointer-to-const");
    SUCCEED() << "Copile time test we always succeed";
}

TEST(AddConstPastPointerTest, DoublePointer) {
    using Result = add_const_past_pointer_t<int **>;
    static_assert(std::is_same_v<Result, int *const *>,
                  "add_const_past_pointer should add const past first pointer level");
    SUCCEED() << "Copile time test we always succeed";
}

}  // namespace corelib
