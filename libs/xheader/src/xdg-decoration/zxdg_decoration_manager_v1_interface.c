// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/xdg-decoration-unstable-v1-client-protocol.h>

#ifndef __linux__

#include <xheader/wayland-util.h>

const struct wl_interface zxdg_decoration_manager_v1_interface = {
    .name = "zxdg_decoration_manager_v1",
    .version = 1,
    .method_count = 0,
    .methods = nullptr,
    .event_count = 0,
    .events = nullptr,
};

#endif
