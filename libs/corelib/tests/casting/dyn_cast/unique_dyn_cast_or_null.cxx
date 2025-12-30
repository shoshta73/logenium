// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/dyn_cast.hxx>

namespace corelib {

TEST(UniqueDynCastOrNullTest, SuccessfulCast) {
    auto s = std::make_unique<TestCircle>(10);

    auto result = corelib::unique_dyn_cast_or_null<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
    EXPECT_EQ(s, nullptr);
}

TEST(UniqueDynCastOrNullTest, FailedCast) {
    auto s = std::make_unique<TestCircle>(10);

    auto result = corelib::unique_dyn_cast_or_null<TestRectangle>(s);
    EXPECT_EQ(result, nullptr);
    ASSERT_NE(s, nullptr);
}

TEST(UniqueDynCastOrNullTest, NullUniquePtr) {
    std::unique_ptr<TestShape> s = nullptr;

    auto result = corelib::unique_dyn_cast_or_null<TestCircle>(s);
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(s, nullptr);
}

TEST(UniqueDynCastOrNullTest, OwnershipTransferOnSuccess) {
    auto s = std::make_unique<TestCircle>(10);
    auto *raw_ptr = static_cast<TestCircle *>(s.get());

    auto result = corelib::unique_dyn_cast_or_null<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result.get(), raw_ptr);
    EXPECT_EQ(s, nullptr);
}

}  // namespace corelib
