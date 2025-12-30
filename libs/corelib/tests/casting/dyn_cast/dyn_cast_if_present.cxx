// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <gtest/gtest.h>

#include <corelib/casting/dyn_cast.hxx>

namespace corelib {

TEST(DynCastIfPresentTest, SuccessfulPointerCast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::dyn_cast_if_present<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(DynCastIfPresentTest, FailedPointerCast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::dyn_cast_if_present<TestRectangle>(s);
    EXPECT_EQ(result, nullptr);
}

TEST(DynCastIfPresentTest, NullPointer) {
    TestShape *s = nullptr;

    auto *result = corelib::dyn_cast_if_present<TestCircle>(s);
    EXPECT_EQ(result, nullptr);
}

}  // namespace corelib
