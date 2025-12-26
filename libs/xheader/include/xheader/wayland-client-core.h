#ifndef LOGENIUM_XHEADER_WAYLAND_CLIENT_CORE_H
#define LOGENIUM_XHEADER_WAYLAND_CLIENT_CORE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <wayland-client-core.h>  // IWYU pragma: export

#else

#include <xheader/wayland-util.h>

struct wl_proxy {
    int dummy;
};

struct wl_display {
    int dummy;
};

struct wl_display *wl_display_connect(const char *name);
int wl_display_roundtrip(struct wl_display *display);
void wl_display_disconnect(struct wl_display *display);
int wl_display_dispatch(struct wl_display *display);

static inline struct wl_proxy *wl_proxy_marshal_flags(struct wl_proxy *proxy, uint32_t opcode,
                                                       const struct wl_interface *interface, uint32_t version,
                                                       uint32_t flags, ...) {
    return NULL;
}

static inline int wl_proxy_add_listener(struct wl_proxy *proxy, void (**implementation)(void), void *data) {
    return 0;
}

static inline uint32_t wl_proxy_get_version(struct wl_proxy *proxy) { return 0; }

#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_WAYLAND_CLIENT_CORE_H
