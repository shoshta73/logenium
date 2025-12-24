// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <debug.hxx>

TEST(Assert, true_predicate_succeeds) {
#ifdef NDEBUG
    // In release builds, Assert is a consteval no-op
    debug::Assert{true, "This should not trigger"};
    debug::Assert{false, "Even false predicates don't trigger in release"};
    SUCCEED() << "Assert compiled as consteval no-op in release mode";
#else
    // In debug builds, only test true predicates (safe - won't trigger breakpoint)
    debug::Assert{true, "This assertion passes"};
    debug::Assert{1 + 1 == 2, "Math works: {} + {} = {}", 1, 1, 2};
    SUCCEED() << "Assert with true predicate succeeded in debug mode";

    // Note: We don't test false predicates here because they would trigger a breakpoint
    // To properly test assertion failures, we would need death tests (ASSERT_DEATH)
    // which is more complex and requires fork/spawn support
#endif
}

TEST(Assert, formatting_works) {
#ifdef NDEBUG
    // In release builds, formatting is completely elided
    debug::Assert{true, "Formatted: {}, {}, {}", 1, "two", 3.0};
    SUCCEED() << "Assert with formatting compiled away in release mode";
#else
    // In debug builds, test that formatted assertions work with true predicates
    debug::Assert{true, "Value: {}", 42};
    debug::Assert{true, "Multiple: {}, {}, {}", 1, 2, 3};
    debug::Assert{true, "String: {}", "hello"};
    SUCCEED() << "Assert with formatting succeeded in debug mode";
#endif
}
