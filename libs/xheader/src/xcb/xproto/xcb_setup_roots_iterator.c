// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xproto.h>

#ifndef __linux__

xcb_screen_iterator_t xcb_setup_roots_iterator(const xcb_setup_t *R) { return (xcb_screen_iterator_t){0}; }

#endif  // __linux__
