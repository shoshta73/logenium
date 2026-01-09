// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/linux/X11/application.hxx"

#include <cstdlib>
#include <memory>

#include <xheader/xcb/xcb.h>
#include <xheader/xcb/xproto.h>

#include "debug/tracing/macros.hxx"
#include <debug/assert.hxx>

#include "logenium/application.hxx"
#include "logenium/platform/linux/X11/window.hxx"
#include "logenium/platform/linux/application.hxx"

namespace logenium {

X11Application::X11Application() : LinuxApplication(ApplicationKind::AK_LinuxX11) {
    ZoneScoped;
    connection = xcb_connect(nullptr, nullptr);
    auto err = xcb_connection_has_error(connection);
    Assert(err == 0, "xcb_connect failed with error {}", err);

    const struct xcb_setup_t *setup = xcb_get_setup(connection);
    screen = xcb_setup_roots_iterator(setup).data;

    window = std::make_shared<X11Window>();

    state.is_running = true;
}

X11Application::~X11Application() {
    ZoneScoped;
    if (state.is_running) {
        state.is_running = false;
    }
    window.reset();
    xcb_disconnect(connection);
}

void X11Application::Run() {
    ZoneScoped;
    while (state.is_running) {
        xcb_generic_event_t *event = xcb_wait_for_event(connection);

        if (!event) {
            continue;
        }

        std::free(event);
    }
}

xcb_connection_t *X11Application::GetConnection() const { return connection; }

xcb_screen_t *X11Application::GetScreen() const { return screen; }

bool X11Application::classof(const Application *app) { return app->GetKind() == ApplicationKind::AK_LinuxX11; }

void X11Application::RegisterWindowClass() {}

void X11Application::UnregisterWindowClass() {}

}  // namespace logenium
