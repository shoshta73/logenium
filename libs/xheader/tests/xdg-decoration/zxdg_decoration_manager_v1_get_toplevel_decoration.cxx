// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xdg-decoration-unstable-v1-client-protocol.h>

TEST(zxdg_decoration_manager_v1_get_toplevel_decoration, returns_nullptr) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(nullptr, zxdg_decoration_manager_v1_get_toplevel_decoration(nullptr, nullptr));
#endif
}
