// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <type_traits>

#include <gtest/gtest.h>

#include <corelib/detail/casting/forwarding.hxx>

namespace corelib::detail {

TEST(NullableValueCastFailedTest, ReturnsNullptr) {
    auto result = NullableValueCastFailed<int *>::CastFailed();
    EXPECT_EQ(result, nullptr);
}

struct MockCastInfo {
    static bool check_called;
    static bool cast_called;
    static bool cast_failed_called;

    static void Reset() {
        check_called = false;
        cast_called = false;
        cast_failed_called = false;
    }

    static bool IsPossible(int *) {
        check_called = true;
        return true;
    }

    static int *DoCast(int *ptr) {
        cast_called = true;
        return ptr;
    }

    static int *CastFailed() {
        cast_failed_called = true;
        return nullptr;
    }
};

bool MockCastInfo::check_called = false;
bool MockCastInfo::cast_called = false;
bool MockCastInfo::cast_failed_called = false;

TEST(DefaultDoCastIfPossibleTest, CallsDoCastWhenPossible) {
    MockCastInfo::Reset();
    int x = 42;
    int *ptr = &x;

    using Derived = DefaultDoCastIfPossible<int *, int *, MockCastInfo>;
    auto result = Derived::DoCastIfPossible(ptr);

    EXPECT_TRUE(MockCastInfo::check_called);
    EXPECT_TRUE(MockCastInfo::cast_called);
    EXPECT_FALSE(MockCastInfo::cast_failed_called);
    EXPECT_EQ(result, ptr);
}

}  // namespace corelib::detail
