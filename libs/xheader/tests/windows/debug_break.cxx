// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/windows.h>

TEST(DebugBreak, returns_void) {
#ifdef _WIN32
    SUCCEED();
#else
    SUCCEED();  // retuns void
#endif
}
