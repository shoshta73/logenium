// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <gtest/gtest.h>

#include <corelib/casting/cast.hxx>

namespace corelib {

TEST(CastOrNullTest, SuccessfulPointerCast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::cast_or_null<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(CastOrNullTest, NullPointer) {
    TestShape *s = nullptr;

    auto *result = corelib::cast_or_null<TestCircle>(s);
    EXPECT_EQ(result, nullptr);
}

TEST(CastOrNullTest, ConstPointer) {
    const TestCircle c(10);
    const TestShape *s = &c;

    const auto *result = corelib::cast_or_null<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(CastOrNullTest, ConstNullPointer) {
    const TestShape *s = nullptr;

    const auto *result = corelib::cast_or_null<TestCircle>(s);
    EXPECT_EQ(result, nullptr);
}

}  // namespace corelib
