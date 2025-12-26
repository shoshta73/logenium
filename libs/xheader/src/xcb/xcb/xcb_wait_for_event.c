// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xcb.h>

#ifndef __linux__

#include <stddef.h>

xcb_generic_event_t *xcb_wait_for_event(xcb_connection_t *c) { return NULL; }

#endif  // __linux__
