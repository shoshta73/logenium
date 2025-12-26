// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xcb.h>

#ifndef __linux__

void xcb_disconnect(xcb_connection_t *c) {}

#endif  // __linux__
