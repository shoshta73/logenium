// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/wayland-util.h>

TEST(wl_interface, struct_compiles) {
#ifdef __linux__
    SUCCEED();
#else
    struct wl_interface iface;
    iface.name = "test";
    iface.version = 1;
    SUCCEED();
#endif
}
