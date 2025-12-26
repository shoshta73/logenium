// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/wayland-client-core.h>

TEST(wl_display_connect, returns_nullptr) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(nullptr, wl_display_connect(nullptr));
#endif
}
