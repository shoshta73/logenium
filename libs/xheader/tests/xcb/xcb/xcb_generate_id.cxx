// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xcb/xcb.h>

TEST(xcb_generate_id, returns_zero) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(0, xcb_generate_id(nullptr));
#endif
}
