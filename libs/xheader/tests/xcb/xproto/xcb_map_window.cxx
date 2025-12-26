// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xcb/xproto.h>

TEST(xcb_map_window, returns_zero_cookie) {
#ifdef __linux__
    SUCCEED();
#else
    xcb_void_cookie_t cookie = xcb_map_window(nullptr, 0);
    ASSERT_EQ(0, cookie.sequence);
#endif
}
