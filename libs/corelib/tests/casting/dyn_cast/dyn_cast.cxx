// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/dyn_cast.hxx>

#include "../common.hxx"

namespace corelib {

TEST(DynCastTest, SuccessfulPointerDowncast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::dyn_cast<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(DynCastTest, FailedPointerDowncast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::dyn_cast<TestRectangle>(s);
    EXPECT_EQ(result, nullptr);
}

TEST(DynCastTest, ConstPointerDowncast) {
    const TestCircle c(10);
    const TestShape *s = &c;

    const auto *result = corelib::dyn_cast<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);

    const auto *failed = corelib::dyn_cast<TestRectangle>(s);
    EXPECT_EQ(failed, nullptr);
}

TEST(DynCastTest, UniquePtrDowncast) {
    auto c = std::make_unique<TestCircle>(10);
    std::unique_ptr<TestShape> s(c.release());

    std::unique_ptr<TestCircle> result = corelib::dyn_cast<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
    EXPECT_EQ(s, nullptr);
}

TEST(DynCastTest, UniquePtrFailedDowncast) {
    auto c = std::make_unique<TestCircle>(10);
    std::unique_ptr<TestShape> s(c.release());

    std::unique_ptr<TestRectangle> result = corelib::dyn_cast<TestRectangle>(s);
    EXPECT_EQ(result, nullptr);
    ASSERT_NE(s, nullptr);
}

}  // namespace corelib
