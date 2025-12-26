// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xdg-shell-client-protocol.h>

#ifndef __linux__

const struct wl_interface xdg_wm_base_interface = {
    .name = "xdg_wm_base",
    .version = 1,
    .method_count = 0,
    .methods = NULL,
    .event_count = 0,
    .events = NULL,
};

#endif
