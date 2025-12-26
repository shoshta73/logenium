// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xproto.h>

#ifndef __linux__

xcb_void_cookie_t xcb_destroy_window(xcb_connection_t *c, xcb_window_t window) { return (xcb_void_cookie_t){0}; }

#endif  // __linux__
