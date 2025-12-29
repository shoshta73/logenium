// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/wayland-client-core.h>

#ifndef __linux__

struct wl_display *wl_display_connect(const char *name) { return nullptr; }

#endif
