// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../../common.hxx"

#include <memory>

#include <gtest/gtest.h>

#include <corelib/detail/casting/isa.hxx>

namespace corelib::detail {

TEST(IsaImplTest, BasicCheck) {
    TestCircle c(10);
    EXPECT_TRUE((IsaImpl<TestCircle, TestShape>::Check(c)));
    EXPECT_FALSE((IsaImpl<TestRectangle, TestShape>::Check(c)));
}

TEST(IsaImplTest, BaseOfCheck) {
    TestCircle c(10);
    EXPECT_TRUE((IsaImpl<TestShape, TestCircle>::Check(c)));
}

TEST(IsaImplConstTest, BasicCheck) {
    TestCircle c(10);
    EXPECT_TRUE((IsaImplConst<TestCircle, TestShape>::Check(c)));
}

TEST(IsaImplConstTest, ConstCheck) {
    const TestCircle c(10);
    EXPECT_TRUE((IsaImplConst<TestCircle, const TestShape>::Check(c)));
}

TEST(IsaImplConstTest, PointerCheck) {
    TestCircle c(10);
    TestShape *s = &c;
    EXPECT_TRUE((IsaImplConst<TestCircle, TestShape *>::Check(s)));
}

TEST(IsaImplConstTest, ConstPointerCheck) {
    const TestCircle c(10);
    const TestShape *s = &c;
    EXPECT_TRUE((IsaImplConst<TestCircle, const TestShape *>::Check(s)));
}

TEST(IsaImplConstTest, UniquePtrCheck) {
    auto c = std::make_unique<TestCircle>(10);
    EXPECT_TRUE((IsaImplConst<TestCircle, const std::unique_ptr<TestShape>>::Check(
        *reinterpret_cast<const std::unique_ptr<TestShape> *>(&c))));
}

}  // namespace corelib::detail
