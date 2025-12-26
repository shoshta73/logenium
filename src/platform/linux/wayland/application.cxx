// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/linux/wayland/application.hxx"

#include <cstdint>
#include <cstring>
#include <memory>

#include <wayland-client-protocol.h>

#include <xheader/wayland-client-core.h>
#include <xheader/wayland-client-protocol.h>
#include <xheader/xdg-decoration-unstable-v1-client-protocol.h>
#include <xheader/xdg-shell-client-protocol.h>

#include <debug/assert.hxx>

#include "logenium/platform/linux/wayland/window.hxx"

namespace logenium {

WaylandApplication::WaylandApplication() {
    auto wl_display = wl_display_connect(nullptr);
    Assert(wl_display != nullptr, "failed to connect to Wayland display");
    display = wl_display;

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);
    xdg_wm_base_add_listener(xdg_wm_base, &wm_listener, this);

    window = std::make_shared<WaylandWindow>();

    state.is_running = true;
}

WaylandApplication::~WaylandApplication() {
    if (state.is_running) {
        state.is_running = false;
    }
    window.reset();
    wl_display_disconnect(display);
}

void WaylandApplication::Run() {
    while (state.is_running) {
        wl_display_dispatch(display);
    }
}

wl_compositor *WaylandApplication::GetCompositor() { return compositor; }

xdg_wm_base *WaylandApplication::GetXdgWmBase() { return xdg_wm_base; }

wl_shm *WaylandApplication::GetShm() { return shm; }

zxdg_decoration_manager_v1 *WaylandApplication::GetDecorationManager() { return decoration_manager; }

void WaylandApplication::RegisterWindowClass() {}

void WaylandApplication::UnregisterWindowClass() {}

void WaylandApplication::RegistryGlobal(void *data, struct wl_registry *registry, std::uint32_t id,
                                        const char *interface, std::uint32_t version) {
    auto self = static_cast<WaylandApplication *>(data);
    Assert(self != nullptr, "invalid self");

    if (strcmp(interface, "wl_compositor") == 0) {
        self->compositor = static_cast<wl_compositor *>(wl_registry_bind(registry, id, &wl_compositor_interface, 4));
    } else if (std::strcmp(interface, "xdg_wm_base") == 0) {
        self->xdg_wm_base =
            static_cast<struct xdg_wm_base *>(wl_registry_bind(registry, id, &xdg_wm_base_interface, 1));
    } else if (std::strcmp(interface, "wl_shm") == 0) {
        self->shm = static_cast<struct wl_shm *>(wl_registry_bind(registry, id, &wl_shm_interface, 1));
    } else if (std::strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0) {
        self->decoration_manager = static_cast<struct zxdg_decoration_manager_v1 *>(
            wl_registry_bind(registry, id, &zxdg_decoration_manager_v1_interface, 1));
    }
}

void WaylandApplication::RegistryRemove(void *data, struct wl_registry *registry, uint32_t id) {}

void WaylandApplication::XDGPing(void *data, struct xdg_wm_base *wm, uint32_t serial) { xdg_wm_base_pong(wm, serial); }

}  // namespace logenium
