// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/wayland-client-protocol.h>

#ifndef __linux__

#include <xheader/wayland-util.h>

const struct wl_interface wl_registry_interface = {
    .name = "wl_registry",
    .version = 1,
    .method_count = 0,
    .methods = nullptr,
    .event_count = 0,
    .events = nullptr,
};

#endif
