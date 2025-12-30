// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/dyn_cast.hxx>

#include "../common.hxx"

namespace corelib {

TEST(UniqueDynCastTest, SuccessfulCast) {
    auto s = std::make_unique<TestCircle>(10);

    auto result = corelib::unique_dyn_cast<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
    EXPECT_EQ(s, nullptr);
}

TEST(UniqueDynCastTest, FailedCast) {
    auto s = std::make_unique<TestCircle>(10);

    auto result = corelib::unique_dyn_cast<TestRectangle>(s);
    EXPECT_EQ(result, nullptr);
    ASSERT_NE(s, nullptr);
    EXPECT_EQ(static_cast<TestCircle *>(s.get())->radius, 10);
}

TEST(UniqueDynCastTest, OwnershipTransfer) {
    auto s = std::make_unique<TestCircle>(10);
    auto *raw_ptr = static_cast<TestCircle *>(s.get());

    auto result = corelib::unique_dyn_cast<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result.get(), raw_ptr);
    EXPECT_EQ(s, nullptr);
}

}  // namespace corelib
