// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/wayland-client-protocol.h>

TEST(wl_compositor_create_surface, returns_nullptr) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(nullptr, wl_compositor_create_surface(nullptr));
#endif
}
