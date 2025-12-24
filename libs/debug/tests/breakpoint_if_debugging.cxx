// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <debug.hxx>

TEST(BreakpointIfDebugging, compiles_and_links) {
#ifdef NDEBUG
    // In release builds, BreakpointIfDebugging() is a consteval no-op
    // We can safely call it - it should compile away completely
    debug::BreakpointIfDebugging();
    SUCCEED() << "BreakpointIfDebugging() compiled as consteval no-op in release mode";
#else
    // In debug builds, BreakpointIfDebugging() only breaks if debugger is attached
    // We can call it safely if no debugger is attached
    if (!debug::IsDebuggerPresent()) {
        debug::BreakpointIfDebugging();
        SUCCEED() << "BreakpointIfDebugging() executed without debugger attached (no-op)";
    } else {
        SUCCEED() << "BreakpointIfDebugging() not called (debugger is attached, would break)";
    }
#endif
}
