// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../../common.hxx"

#include <gtest/gtest.h>

#include <corelib/detail/casting/predicates.hxx>

namespace corelib::detail {

TEST(IsaCheckPredicateTest, BasicCheck) {
    TestCircle c(10);
    TestShape &s = c;

    IsaCheckPredicate<TestCircle> pred{};
    EXPECT_TRUE(pred(s));
}

TEST(IsaCheckPredicateTest, FailedCheck) {
    TestCircle c(10);
    TestShape &s = c;

    IsaCheckPredicate<TestRectangle> pred{};
    EXPECT_FALSE(pred(s));
}

TEST(IsaAndPresentCheckPredicateTest, PresentAndMatch) {
    TestCircle c(10);
    TestShape *s = &c;

    IsaAndPresentCheckPredicate<TestCircle> pred{};
    EXPECT_TRUE(pred(s));
}

TEST(IsaAndPresentCheckPredicateTest, PresentButNoMatch) {
    TestCircle c(10);
    TestShape *s = &c;

    IsaAndPresentCheckPredicate<TestRectangle> pred{};
    EXPECT_FALSE(pred(s));
}

TEST(IsaAndPresentCheckPredicateTest, NullPointer) {
    TestShape *s = nullptr;

    IsaAndPresentCheckPredicate<TestCircle> pred{};
    EXPECT_FALSE(pred(s));
}

TEST(CastFuncTest, BasicCast) {
    TestCircle c(10);
    TestShape &s = c;

    CastFunc<TestCircle> cast_func{};
    TestCircle &result = cast_func(s);
    EXPECT_EQ(result.radius, 10);
}

TEST(DynCastFuncTest, SuccessfulCast) {
    TestCircle c(10);
    TestShape *s = &c;

    DynCastFunc<TestCircle> dyn_cast_func{};
    TestCircle *result = dyn_cast_func(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(DynCastFuncTest, FailedCast) {
    TestCircle c(10);
    TestShape *s = &c;

    DynCastFunc<TestRectangle> dyn_cast_func{};
    TestRectangle *result = dyn_cast_func(s);
    EXPECT_EQ(result, nullptr);
}

}  // namespace corelib::detail
