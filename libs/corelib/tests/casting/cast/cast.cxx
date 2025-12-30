// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <type_traits>

#include <gtest/gtest.h>

#include <corelib/casting/cast.hxx>

#include "../common.hxx"

namespace corelib {

TEST(CastTest, BasicReferenceDowncast) {
    TestCircle c(10);
    TestShape &s = c;

    auto &result = corelib::cast<TestCircle>(s);
    EXPECT_EQ(result.radius, 10);
}

TEST(CastTest, PointerDowncast) {
    TestCircle c(10);
    TestShape *s = &c;

    auto *result = corelib::cast<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(CastTest, ConstPointerDowncast) {
    const TestCircle c(10);
    const TestShape *s = &c;

    const auto *result = corelib::cast<TestCircle>(s);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->radius, 10);
}

TEST(CastTest, ConstReferencePreservation) {
    const TestCircle c(10);
    const TestShape &s = c;

    const auto &result = corelib::cast<TestCircle>(s);
    EXPECT_EQ(result.radius, 10);

    static_assert(std::is_const_v<std::remove_reference_t<decltype(result)>>, "Const should be preserved");
}

}  // namespace corelib
