// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <gtest/gtest.h>

#include <xheader/xcb/xproto.h>

TEST(xcb_setup_roots_iterator, returns_zero_initialized_iterator) {
#ifdef __linux__
    SUCCEED();
#else
    xcb_setup_t setup{};
    xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(&setup);
    ASSERT_EQ(nullptr, iterator.data);
    ASSERT_EQ(0, iterator.rem);
    ASSERT_EQ(0, iterator.index);
#endif
}
