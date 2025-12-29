// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_WAYLAND_WINDOW_HXX
#define LOGENIUM_PLATFORM_LINUX_WAYLAND_WINDOW_HXX

#include <cstdint>

#include <xheader/wayland-client-protocol.h>
#include <xheader/wayland-client.h>
#include <xheader/wayland-util.h>
#include <xheader/xdg-decoration-unstable-v1-client-protocol.h>
#include <xheader/xdg-shell-client-protocol.h>

#include "logenium/platform/linux/window.hxx"

namespace logenium {

class WaylandWindow : public LinuxWindow {
  public:
    WaylandWindow();
    ~WaylandWindow() override;

  private:
    static void XdgSurfaceConfigure(void *data, struct xdg_surface *xdg_surface, uint32_t serial);
    static void XdgToplevelConfigure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height,
                                     struct wl_array *states);
    static void XdgToplevelClose(void *data, struct xdg_toplevel *xdg_toplevel);

    void CreateBuffer();

    struct wl_surface *surface = nullptr;
    struct xdg_surface *xdg_surface = nullptr;
    struct xdg_toplevel *xdg_toplevel = nullptr;
    struct wl_buffer *buffer = nullptr;
    struct zxdg_toplevel_decoration_v1 *decoration = nullptr;

    int width = 1280;
    int height = 720;
    bool configured = false;

    const struct xdg_surface_listener xdg_surface_listener = {
        .configure = WaylandWindow::XdgSurfaceConfigure,
    };

    const struct xdg_toplevel_listener xdg_toplevel_listener = {
        .configure = WaylandWindow::XdgToplevelConfigure,
        .close = WaylandWindow::XdgToplevelClose,
    };
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_WAYLAND_WINDOW_HXX
