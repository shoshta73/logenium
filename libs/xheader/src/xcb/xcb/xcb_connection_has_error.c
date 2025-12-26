// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xcb.h>

#ifndef __linux__

int xcb_connection_has_error(xcb_connection_t *c) { return 0; }

#endif  // __linux__
