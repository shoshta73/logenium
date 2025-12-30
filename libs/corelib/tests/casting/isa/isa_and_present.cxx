// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/isa.hxx>

#include "../common.hxx"

namespace corelib {

TEST(IsaAndPresentTest, PointerPresenceCheck) {
    TestCircle c(10);
    TestShape *s1 = &c;
    TestShape *s2 = nullptr;

    EXPECT_TRUE(corelib::isa_and_present<TestCircle>(s1));
    EXPECT_FALSE(corelib::isa_and_present<TestRectangle>(s1));
    EXPECT_FALSE(corelib::isa_and_present<TestCircle>(s2));
}

TEST(IsaAndPresentTest, UniquePtrPresenceCheck) {
    auto c1 = std::make_unique<TestCircle>(10);
    std::unique_ptr<TestCircle> c2 = nullptr;

    EXPECT_TRUE(corelib::isa_and_present<TestCircle>(c1));
    EXPECT_FALSE(corelib::isa_and_present<TestCircle>(c2));
}

}  // namespace corelib
