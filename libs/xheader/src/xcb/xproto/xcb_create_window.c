// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xproto.h>

#ifndef __linux__

xcb_void_cookie_t xcb_create_window(xcb_connection_t *c, uint8_t depth, xcb_window_t wid, xcb_window_t parent,
                                    int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t border_width,
                                    uint16_t _class, xcb_visualid_t visual, uint32_t value_mask,
                                    const void *value_list) {
    return (xcb_void_cookie_t){0};
}

#endif  // __linux__
