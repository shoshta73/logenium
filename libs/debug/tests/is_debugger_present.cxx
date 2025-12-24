// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <debug.hxx>

TEST(IsDebuggerPresent, returns_boolean) {
#ifdef NDEBUG
    // In release builds, always returns false (consteval)
    ASSERT_EQ(false, debug::IsDebuggerPresent());
#else
    // In debug builds, returns actual debugger state
    // We don't know if a debugger is attached, so just verify it compiles and returns a bool
    bool result = debug::IsDebuggerPresent();
    SUCCEED() << "IsDebuggerPresent() returned: " << (result ? "true" : "false");
#endif
}
