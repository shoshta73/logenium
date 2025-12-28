// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/wayland-client-core.h>

#ifndef __linux__

int wl_display_roundtrip(struct wl_display *display) { return -1; }

#endif
