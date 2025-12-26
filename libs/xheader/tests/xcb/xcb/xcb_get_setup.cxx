// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xcb/xcb.h>

TEST(xcb_get_setup, returns_null) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(nullptr, xcb_get_setup(nullptr));
#endif
}
