// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/wayland-client-protocol.h>

#ifndef __linux__

const struct wl_interface wl_compositor_interface = {
    .name = "wl_compositor",
    .version = 4,
    .method_count = 0,
    .methods = NULL,
    .event_count = 0,
    .events = NULL,
};

#endif
