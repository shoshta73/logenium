// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xcb/xcb.h>

#ifndef __linux__

const struct xcb_setup_t *xcb_get_setup(xcb_connection_t *c) { return (const struct xcb_setup_t *)NULL; }

#endif  // __linux__
