// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <type_traits>

#include <gtest/gtest.h>

#include <corelib/detail/casting/cast.hxx>

#include "../../common.hxx"

namespace corelib::detail {

TEST(CastRettyTest, PointerReturnType) {
    using Result = typename CastRetty<TestCircle, TestShape *>::ret_type;
    static_assert(std::is_same_v<Result, TestCircle *>, "CastRetty should return pointer for pointer input");
}

TEST(CastRettyTest, ReferenceReturnType) {
    using Result = typename CastRetty<TestCircle, TestShape &>::ret_type;
    static_assert(std::is_same_v<Result, TestCircle &>, "CastRetty should return reference for reference input");
}

TEST(CastRettyTest, ConstPointerReturnType) {
    using Result = typename CastRetty<TestCircle, const TestShape *>::ret_type;
    static_assert(std::is_same_v<Result, const TestCircle *>,
                  "CastRetty should preserve const in pointer return type");
}

}  // namespace corelib::detail
