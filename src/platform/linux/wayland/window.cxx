// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/linux/wayland/window.hxx"

#include <cstdint>
#include <cstring>

#include <xheader/fcntl.h>
#include <xheader/sys/mman.h>
#include <xheader/unistd.h>
#include <xheader/wayland-client-protocol.h>
#include <xheader/wayland-util.h>
#include <xheader/xdg-decoration-unstable-v1-client-protocol.h>
#include <xheader/xdg-shell-client-protocol.h>

#include "debug/tracing/macros.hxx"
#include <debug/assert.hxx>

#include <corelib/casting/cast.hxx>
#include <corelib/casting/isa.hxx>

#include "logenium/application.hxx"
#include "logenium/platform/linux/wayland/application.hxx"
#include "logenium/platform/linux/window.hxx"
#include "logenium/window.hxx"

namespace logenium {

WaylandWindow::WaylandWindow() : LinuxWindow(WindowKind::WK_LinuxWayland) {
    ZoneScoped;
    Assert(isa<WaylandApplication>(Application::GetInstance()), "Application is not WaylandApplication");
    auto &app = cast<WaylandApplication>(Application::GetInstance());

    surface = wl_compositor_create_surface(app.GetCompositor());
    xdg_surface = xdg_wm_base_get_xdg_surface(app.GetXdgWmBase(), surface);
    xdg_surface_add_listener(xdg_surface, &xdg_surface_listener, this);

    xdg_toplevel = xdg_surface_get_toplevel(xdg_surface);
    xdg_toplevel_add_listener(xdg_toplevel, &xdg_toplevel_listener, this);
    xdg_toplevel_set_title(xdg_toplevel, "Logenium");

    if (app.GetDecorationManager() != nullptr) {
        decoration = zxdg_decoration_manager_v1_get_toplevel_decoration(app.GetDecorationManager(), xdg_toplevel);
        zxdg_toplevel_decoration_v1_set_mode(decoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
    }

    wl_surface_commit(surface);
}

WaylandWindow::~WaylandWindow() {
    ZoneScoped;
    if (buffer != nullptr) {
        wl_buffer_destroy(buffer);
    }
    if (decoration != nullptr) {
        zxdg_toplevel_decoration_v1_destroy(decoration);
    }
    xdg_toplevel_destroy(xdg_toplevel);
    xdg_surface_destroy(xdg_surface);
    wl_surface_destroy(surface);
}

bool WaylandWindow::classof(const Window *win) { return win->GetKind() == WindowKind::WK_LinuxWayland; }

void WaylandWindow::XdgSurfaceConfigure(void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    ZoneScoped;
    auto *self = static_cast<WaylandWindow *>(data);
    xdg_surface_ack_configure(xdg_surface, serial);

    if (!self->configured) {
        self->configured = true;
        self->CreateBuffer();
    }
}

void WaylandWindow::XdgToplevelConfigure(void *data, struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height,
                                         struct wl_array *states) {
    ZoneScoped;
    auto *self = static_cast<WaylandWindow *>(data);
    if (width > 0 && height > 0) {
        self->width = width;
        self->height = height;
    }
}

void WaylandWindow::XdgToplevelClose(void *data, struct xdg_toplevel *xdg_toplevel) {
    ZoneScoped;
    auto &app = Application::GetInstance();
    app.GetState().is_running = false;
}

void WaylandWindow::CreateBuffer() {
    ZoneScoped;
    auto &app = cast<WaylandApplication>(Application::GetInstance());

    int stride = width * 4;
    int size = stride * height;

    int fd = memfd_create("wayland-shm", MFD_CLOEXEC);
    Assert(fd >= 0, "failed to create shared memory file descriptor");

    int result = ftruncate(fd, size);
    Assert(result == 0, "failed to truncate shared memory file");

    void *data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    Assert(data != MAP_FAILED, "failed to map shared memory");

    std::memset(data, 0x80, size);

    struct wl_shm_pool *pool = wl_shm_create_pool(app.GetShm(), fd, size);
    buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);

    munmap(data, size);
    close(fd);

    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);
}

}  // namespace logenium
