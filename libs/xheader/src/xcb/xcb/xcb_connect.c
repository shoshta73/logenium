// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xcb.h>

#ifndef __linux__

#include <stddef.h>

xcb_connection_t *xcb_connect(const char *displayname, int *screenp) { return (xcb_connection_t *)NULL; }

#endif  // __linux__
