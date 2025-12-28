// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_XHEADER_WAYLAND_UTIL_H
#define LOGENIUM_XHEADER_WAYLAND_UTIL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__

#include <wayland-util.h>  // IWYU pragma: export

#else

struct wl_message {
    const char *name;
    const char *signature;
    const struct wl_interface **types;
};

struct wl_interface {
    const char *name;
    int version;
    int method_count;
    const struct wl_message *methods;
    int event_count;
    const struct wl_message *events;
};

struct wl_list {
    struct wl_list *prev;
    struct wl_list *next;
};

struct wl_array {
    size_t size;
    size_t alloc;
    void *data;
};

union wl_argument {
    int32_t i;
    uint32_t u;
    int32_t f;
    const char *s;
    struct wl_object *o;
    uint32_t n;
    struct wl_array *a;
    int32_t h;
};

#endif

#ifdef __cplusplus
}
#endif

#endif  // LOGENIUM_XHEADER_WAYLAND_UTIL_H
