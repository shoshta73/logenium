// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/isa.hxx>

namespace corelib {

TEST(IsaTest, BasicTypeChecking) {
    TestCircle c(10);
    TestRectangle r(5, 10);
    TestTriangle t(3, 4);

    EXPECT_TRUE(corelib::isa<TestCircle>(c));
    EXPECT_FALSE(corelib::isa<TestRectangle>(c));
    EXPECT_FALSE(corelib::isa<TestTriangle>(c));

    EXPECT_FALSE(corelib::isa<TestCircle>(r));
    EXPECT_TRUE(corelib::isa<TestRectangle>(r));
    EXPECT_FALSE(corelib::isa<TestTriangle>(r));

    EXPECT_FALSE(corelib::isa<TestCircle>(t));
    EXPECT_FALSE(corelib::isa<TestRectangle>(t));
    EXPECT_TRUE(corelib::isa<TestTriangle>(t));
}

TEST(IsaTest, BaseToDeriveredCheck) {
    TestCircle c(10);
    TestShape &s = c;

    EXPECT_TRUE(corelib::isa<TestCircle>(s));
    EXPECT_FALSE(corelib::isa<TestRectangle>(s));
    EXPECT_TRUE(corelib::isa<TestShape>(s));
}

TEST(IsaTest, PointerTypeChecking) {
    TestCircle c(10);
    TestRectangle r(5, 10);

    TestShape *s1 = &c;
    TestShape *s2 = &r;

    EXPECT_TRUE(corelib::isa<TestCircle>(s1));
    EXPECT_FALSE(corelib::isa<TestRectangle>(s1));

    EXPECT_FALSE(corelib::isa<TestCircle>(s2));
    EXPECT_TRUE(corelib::isa<TestRectangle>(s2));
}

TEST(IsaTest, ConstPointerTypeChecking) {
    const TestCircle c(10);
    const TestRectangle r(5, 10);

    const TestShape *s1 = &c;
    const TestShape *s2 = &r;

    EXPECT_TRUE(corelib::isa<TestCircle>(s1));
    EXPECT_FALSE(corelib::isa<TestRectangle>(s1));

    EXPECT_FALSE(corelib::isa<TestCircle>(s2));
    EXPECT_TRUE(corelib::isa<TestRectangle>(s2));
}

TEST(IsaTest, VariadicTypeChecking) {
    TestCircle c(10);
    TestRectangle r(5, 10);
    TestTriangle t(3, 4);

    TestShape &s1 = c;
    TestShape &s2 = r;
    TestShape &s3 = t;

    EXPECT_TRUE((corelib::isa<TestCircle, TestRectangle>(s1)));
    EXPECT_TRUE((corelib::isa<TestCircle, TestRectangle>(s2)));
    EXPECT_FALSE((corelib::isa<TestCircle, TestRectangle>(s3)));

    EXPECT_TRUE((corelib::isa<TestCircle, TestRectangle, TestTriangle>(s1)));
    EXPECT_TRUE((corelib::isa<TestCircle, TestRectangle, TestTriangle>(s2)));
    EXPECT_TRUE((corelib::isa<TestCircle, TestRectangle, TestTriangle>(s3)));
}

TEST(IsaTest, UniquePtrTypeChecking) {
    auto c = std::make_unique<TestCircle>(10);
    auto r = std::make_unique<TestRectangle>(5, 10);

    EXPECT_TRUE(corelib::isa<TestCircle>(c));
    EXPECT_FALSE(corelib::isa<TestRectangle>(c));

    EXPECT_FALSE(corelib::isa<TestCircle>(r));
    EXPECT_TRUE(corelib::isa<TestRectangle>(r));
}

}  // namespace corelib
