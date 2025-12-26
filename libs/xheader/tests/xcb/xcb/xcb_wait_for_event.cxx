// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xcb/xcb.h>

TEST(xcb_wait_for_event, returns_null) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(nullptr, xcb_wait_for_event(nullptr));
#endif
}
