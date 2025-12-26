// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xcb/xcb.h>

TEST(xcb_flush, returns_zero) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(0, xcb_flush(nullptr));
#endif
}
