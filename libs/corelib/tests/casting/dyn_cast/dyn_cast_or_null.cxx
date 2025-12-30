// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <gtest/gtest.h>

#include <corelib/casting/dyn_cast.hxx>

namespace corelib {

TEST(DynCastOrNullTest, SuccessfulPointerCast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::dyn_cast_or_null<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(DynCastOrNullTest, FailedPointerCast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::dyn_cast_or_null<TestRectangle>(s);
    EXPECT_EQ(result, nullptr);
}

TEST(DynCastOrNullTest, NullPointer) {
    TestShape *s = nullptr;

    auto *result = corelib::dyn_cast_or_null<TestCircle>(s);
    EXPECT_EQ(result, nullptr);
}

TEST(DynCastOrNullTest, ConstPointer) {
    const TestCircle c(10);
    const TestShape *s = &c;

    const auto *result = corelib::dyn_cast_or_null<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

}  // namespace corelib
