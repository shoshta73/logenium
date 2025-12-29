// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
//  SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_INTERNAL_WAYLAND_CLIENT_PROTOCOL_H
#define LOGENIUM_XHEADER_INTERNAL_WAYLAND_CLIENT_PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <wayland-client-protocol.h>  // IWYU pragma: export

#else

// Windows headers define 'interface' as a macro that expands to 'struct', which conflicts with
// Wayland's use of 'interface' as a parameter name. Undefine it to prevent compilation errors.
#ifdef interface
#undef interface
#endif

#include <xheader/wayland-client-core.h>
#include <xheader/wayland-util.h>

#define WL_DISPLAY_GET_REGISTRY 1
#define WL_REGISTRY_BIND 0

enum wl_shm_format {
    WL_SHM_FORMAT_ARGB8888 = 0,
    WL_SHM_FORMAT_XRGB8888 = 1,
};

struct wl_compositor {
    int dummy;
};

struct wl_registry {
    int dummy;
};

struct wl_shm {
    int dummy;
};

struct wl_surface {
    int dummy;
};

struct wl_buffer {
    int dummy;
};

struct wl_shm_pool {
    int dummy;
};

extern const struct wl_interface wl_registry_interface;
extern const struct wl_interface wl_compositor_interface;
extern const struct wl_interface wl_shm_interface;

struct wl_registry_listener {
    void (*global)(void *data, struct wl_registry *wl_registry, uint32_t name, const char *interface, uint32_t version);
    void (*global_remove)(void *data, struct wl_registry *wl_registry, uint32_t name);
};

static inline struct wl_registry *wl_display_get_registry(struct wl_display *wl_display) {
    struct wl_proxy *registry;

    registry = wl_proxy_marshal_flags((struct wl_proxy *)wl_display, WL_DISPLAY_GET_REGISTRY, &wl_registry_interface,
                                      wl_proxy_get_version((struct wl_proxy *)wl_display), 0, NULL);

    return (struct wl_registry *)registry;
}

static inline int wl_registry_add_listener(struct wl_registry *wl_registry, const struct wl_registry_listener *listener,
                                           void *data) {
    return wl_proxy_add_listener((struct wl_proxy *)wl_registry, (void (**)(void))listener, data);
}

static inline void *wl_registry_bind(struct wl_registry *wl_registry, uint32_t name,
                                     const struct wl_interface *interface, uint32_t version) {
    struct wl_proxy *id;

    id = wl_proxy_marshal_flags((struct wl_proxy *)wl_registry, WL_REGISTRY_BIND, interface, version, 0, name,
                                interface->name, version, NULL);

    return (void *)id;
}

static inline struct wl_surface *wl_compositor_create_surface(struct wl_compositor *wl_compositor) { return nullptr; }

static inline void wl_surface_attach(struct wl_surface *wl_surface, struct wl_buffer *buffer, int32_t x, int32_t y) {}

static inline void wl_surface_commit(struct wl_surface *wl_surface) {}

static inline void wl_surface_destroy(struct wl_surface *wl_surface) {}

static inline void wl_buffer_destroy(struct wl_buffer *wl_buffer) {}

static inline struct wl_shm_pool *wl_shm_create_pool(struct wl_shm *wl_shm, int32_t fd, int32_t size) {
    return nullptr;
}

static inline struct wl_buffer *wl_shm_pool_create_buffer(struct wl_shm_pool *wl_shm_pool, int32_t offset,
                                                          int32_t width, int32_t height, int32_t stride,
                                                          uint32_t format) {
    return nullptr;
}

static inline void wl_shm_pool_destroy(struct wl_shm_pool *wl_shm_pool) {}

#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_INTERNAL_WAYLAND_CLIENT_PROTOCOL_H
