// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/wayland-client-core.h>

TEST(wl_proxy_marshal_flags, returns_nullptr) {
#ifdef __linux__
    SUCCEED();
#else
    ASSERT_EQ(nullptr, wl_proxy_marshal_flags(nullptr, 0, nullptr, 0, 0));
#endif
}
