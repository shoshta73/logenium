// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/wayland-client-core.h>

#ifndef __linux__

void wl_display_disconnect(struct wl_display *display) {}

#endif
