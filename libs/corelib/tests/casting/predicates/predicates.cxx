// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/predicates.hxx>

namespace corelib {

TEST(PredicatesTest, IsaFunctor) {
    TestCircle c(10);
    TestRectangle r(5, 10);

    TestShape &s1 = c;
    TestShape &s2 = r;

    auto isa_circle = corelib::IsaPred<TestCircle>;
    EXPECT_TRUE(isa_circle(s1));
    EXPECT_FALSE(isa_circle(s2));
}

TEST(PredicatesTest, IsaAndPresentFunctor) {
    TestCircle c(10);
    TestShape *s1 = &c;
    TestShape *s2 = nullptr;

    auto isa_and_present_circle = corelib::IsaAndPresentPred<TestCircle>;
    EXPECT_TRUE(isa_and_present_circle(s1));
    EXPECT_FALSE(isa_and_present_circle(s2));
}

TEST(PredicatesTest, StaticCastToFunctor) {
    TestCircle c(10);
    TestShape *s = &c;

    auto static_cast_to_circle = corelib::StaticCastTo<TestCircle *>;
    TestCircle *result = static_cast_to_circle(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(PredicatesTest, CastToFunctor) {
    TestCircle c(10);
    TestShape &s = c;

    auto cast_to_circle = corelib::CastTo<TestCircle>;
    TestCircle &result = cast_to_circle(s);
    EXPECT_EQ(result.radius, 10);
}

TEST(PredicatesTest, CastIfPresentToFunctor) {
    TestCircle c(10);
    TestShape *s1 = &c;
    TestShape *s2 = nullptr;

    auto cast_if_present_to_circle = corelib::CastIfPresentTo<TestCircle>;

    TestCircle *result1 = cast_if_present_to_circle(s1);
    ASSERT_NE(result1, nullptr);
    EXPECT_EQ(result1->radius, 10);

    TestCircle *result2 = cast_if_present_to_circle(s2);
    EXPECT_EQ(result2, nullptr);
}

TEST(PredicatesTest, DynCastIfPresentToFunctor) {
    TestCircle c(10);
    TestRectangle r(5, 10);
    TestShape *s1 = &c;
    TestShape *s2 = &r;
    TestShape *s3 = nullptr;

    auto dyn_cast_if_present_to_circle = corelib::DynCastIfPresentTo<TestCircle>;

    TestCircle *result1 = dyn_cast_if_present_to_circle(s1);
    ASSERT_NE(result1, nullptr);
    EXPECT_EQ(result1->radius, 10);

    TestCircle *result2 = dyn_cast_if_present_to_circle(s2);
    EXPECT_EQ(result2, nullptr);

    TestCircle *result3 = dyn_cast_if_present_to_circle(s3);
    EXPECT_EQ(result3, nullptr);
}

TEST(PredicatesTest, DynCastToFunctor) {
    TestCircle c(10);
    TestShape *s = &c;

    auto dyn_cast_to_circle = corelib::DynCastTo<TestCircle>;
    TestCircle *result = dyn_cast_to_circle(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

}  // namespace corelib
