// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_WAYLAND_APPLICATION_HXX
#define LOGENIUM_PLATFORM_LINUX_WAYLAND_APPLICATION_HXX

#include <cstdint>

#include <xheader/wayland-client-core.h>
#include <xheader/wayland-client-protocol.h>
#include <xheader/wayland-client.h>
#include <xheader/xdg-decoration-unstable-v1-client-protocol.h>
#include <xheader/xdg-shell-client-protocol.h>

#include "logenium/platform/linux/application.hxx"

namespace logenium {

class WaylandApplication : public LinuxApplication {
  public:
    WaylandApplication();
    ~WaylandApplication();

    void Run() final;

    wl_compositor *GetCompositor();
    xdg_wm_base *GetXdgWmBase();
    wl_shm *GetShm();
    zxdg_decoration_manager_v1 *GetDecorationManager();

  private:
    void RegisterWindowClass() final;
    void UnregisterWindowClass() final;

    static void RegistryGlobal(void *data, struct wl_registry *registry, std::uint32_t id, const char *interface,
                               std::uint32_t version);

    static void RegistryRemove(void *data, struct wl_registry *registry, uint32_t id);
    static void XDGPing(void *data, struct xdg_wm_base *wm, uint32_t serial);

    struct wl_display *display = nullptr;
    struct wl_registry *registry = nullptr;
    struct wl_compositor *compositor = nullptr;
    struct xdg_wm_base *xdg_wm_base = nullptr;
    struct wl_shm *shm = nullptr;
    struct zxdg_decoration_manager_v1 *decoration_manager = nullptr;

    const struct wl_registry_listener registry_listener{
        .global = WaylandApplication::RegistryGlobal,
        .global_remove = WaylandApplication::RegistryRemove,
    };

    const struct xdg_wm_base_listener wm_listener = {.ping = WaylandApplication::XDGPing};
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_WAYLAND_APPLICATION_HXX
