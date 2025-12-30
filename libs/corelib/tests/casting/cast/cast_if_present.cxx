// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <corelib/casting/cast.hxx>

#include "../common.hxx"

namespace corelib {

TEST(CastIfPresentTest, PointerWithValue) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::cast_if_present<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(CastIfPresentTest, NullPointer) {
    TestShape *s = nullptr;

    auto *result = corelib::cast_if_present<TestCircle>(s);
    EXPECT_EQ(result, nullptr);
}

}  // namespace corelib
