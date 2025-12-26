// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xdg-decoration-unstable-v1-client-protocol.h>

TEST(zxdg_toplevel_decoration_v1_mode, enum_values) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(1, ZXDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE);
    ASSERT_EQ(2, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
#endif
}
