// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xdg-shell-client-protocol.h>

TEST(xdg_wm_base_get_xdg_surface, returns_nullptr) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(nullptr, xdg_wm_base_get_xdg_surface(nullptr, nullptr));
#endif
}
