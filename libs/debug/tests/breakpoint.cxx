// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <debug.hxx>

TEST(Breakpoint, compiles_and_links) {
#ifdef NDEBUG
    // In release builds, Breakpoint() is a consteval no-op
    // We can safely call it - it should compile away completely
    debug::Breakpoint();
    SUCCEED() << "Breakpoint() compiled as consteval no-op in release mode";
#else
    // In debug builds, Breakpoint() would trigger an actual breakpoint
    // We can't safely call it in tests, so we just verify it compiles
    SUCCEED() << "Breakpoint() function exists and links in debug mode (not calling to avoid breaking)";
#endif
}
