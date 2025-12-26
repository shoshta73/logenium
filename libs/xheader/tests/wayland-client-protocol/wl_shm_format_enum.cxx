// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/wayland-client-protocol.h>

TEST(wl_shm_format, enum_values) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(0, WL_SHM_FORMAT_ARGB8888);
    ASSERT_EQ(1, WL_SHM_FORMAT_XRGB8888);
#endif
}
