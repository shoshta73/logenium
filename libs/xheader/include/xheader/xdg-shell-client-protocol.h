// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_INTERNAL_XDG_SHELL_CLIENT_PROTOCOL_H
#define LOGENIUM_XHEADER_INTERNAL_XDG_SHELL_CLIENT_PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <xheader/internal/xdg-shell-client-protocol.h>  // IWYU pragma: export

#else

#include <xheader/wayland-client-core.h>
#include <xheader/wayland-client-protocol.h>

#define XDG_WM_BASE_PONG 3

struct xdg_wm_base {
    int dummy;
};

struct xdg_surface {
    int dummy;
};

struct xdg_toplevel {
    int dummy;
};

extern const struct wl_interface xdg_wm_base_interface;

struct xdg_wm_base_listener {
    void (*ping)(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);
};

struct xdg_surface_listener {
    void (*configure)(void *data, struct xdg_surface *xdg_surface, uint32_t serial);
};

struct xdg_toplevel_listener {
    void (*configure)(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height,
                      struct wl_array *states);
    void (*close)(void *data, struct xdg_toplevel *xdg_toplevel);
};

static inline int xdg_wm_base_add_listener(struct xdg_wm_base *xdg_wm_base, const struct xdg_wm_base_listener *listener,
                                           void *data) {
    return 0;
}

static inline void xdg_wm_base_pong(struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
    wl_proxy_marshal_flags((struct wl_proxy *)xdg_wm_base, XDG_WM_BASE_PONG, nullptr,
                           wl_proxy_get_version((struct wl_proxy *)xdg_wm_base), 0, serial);
}

static inline struct xdg_surface *xdg_wm_base_get_xdg_surface(struct xdg_wm_base *xdg_wm_base,
                                                              struct wl_surface *surface) {
    return nullptr;
}

static inline int xdg_surface_add_listener(struct xdg_surface *xdg_surface, const struct xdg_surface_listener *listener,
                                           void *data) {
    return 0;
}

static inline struct xdg_toplevel *xdg_surface_get_toplevel(struct xdg_surface *xdg_surface) { return nullptr; }

static inline void xdg_surface_ack_configure(struct xdg_surface *xdg_surface, uint32_t serial) {}

static inline void xdg_surface_destroy(struct xdg_surface *xdg_surface) {}

static inline int xdg_toplevel_add_listener(struct xdg_toplevel *xdg_toplevel,
                                            const struct xdg_toplevel_listener *listener, void *data) {
    return 0;
}

static inline void xdg_toplevel_set_title(struct xdg_toplevel *xdg_toplevel, const char *title) {}

static inline void xdg_toplevel_destroy(struct xdg_toplevel *xdg_toplevel) {}

#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_INTERNAL_XDG_SHELL_CLIENT_PROTOCOL_H
