// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xcb.h>

#ifndef __linux__

uint32_t xcb_generate_id(xcb_connection_t *c) { return 0; }

#endif  // __linux__
