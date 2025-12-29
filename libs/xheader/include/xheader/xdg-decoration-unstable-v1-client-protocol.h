// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_INTERNAL_XDG_DECORATION_UNSTABLE_V1_CLIENT_PROTOCOL_H
#define LOGENIUM_XHEADER_INTERNAL_XDG_DECORATION_UNSTABLE_V1_CLIENT_PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <xheader/internal/xdg-decoration-unstable-v1-client-protocol.h>  // IWYU pragma: export

#else

#include <xheader/wayland-util.h>
#include <xheader/xdg-shell-client-protocol.h>

enum zxdg_toplevel_decoration_v1_mode {
    ZXDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE = 1,
    ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE = 2,
};

struct zxdg_decoration_manager_v1 {
    int dummy;
};

struct zxdg_toplevel_decoration_v1 {
    int dummy;
};

extern const struct wl_interface zxdg_decoration_manager_v1_interface;

static inline struct zxdg_toplevel_decoration_v1 *zxdg_decoration_manager_v1_get_toplevel_decoration(
    struct zxdg_decoration_manager_v1 *zxdg_decoration_manager_v1, struct xdg_toplevel *toplevel) {
    return nullptr;
}

static inline void zxdg_toplevel_decoration_v1_destroy(
    struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1) {}

static inline void zxdg_toplevel_decoration_v1_set_mode(struct zxdg_toplevel_decoration_v1 *zxdg_toplevel_decoration_v1,
                                                        uint32_t mode) {}

#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_INTERNAL_XDG_DECORATION_UNSTABLE_V1_CLIENT_PROTOCOL_H
