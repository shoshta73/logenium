// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xcb/xcb.h>

TEST(xcb_disconnect, completes_successfully) {
#ifdef __linux__
    SUCCEED();
#else
    xcb_disconnect(nullptr);
    SUCCEED();
#endif
}
