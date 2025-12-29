// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/wayland-util.h>

TEST(wl_array, struct_compiles) {
#ifdef __linux__
    SUCCEED();
#else
    struct wl_array arr{};
    arr.size = 0;
    arr.alloc = 0;
    arr.data = nullptr;
    SUCCEED();
#endif
}
