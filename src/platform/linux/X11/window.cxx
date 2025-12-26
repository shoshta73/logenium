// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/linux/X11/window.hxx"

#include <xheader/xcb/xcb.h>
#include <xheader/xcb/xproto.h>

#include <debug/assert.hxx>

#include "logenium/application.hxx"
#include "logenium/platform/linux/X11/application.hxx"

namespace logenium {

X11Window::X11Window() {
    auto *app = dynamic_cast<X11Application *>(&Application::GetInstance());
    Assert(app != nullptr, "Application is not an X11Application");
    auto connection = app->GetConnection();
    auto screen = app->GetScreen();
    auto window = xcb_generate_id(connection);
    Assert(window != -1, "xcb_generate_id failed");
    native_handle = window;

    xcb_create_window(connection, XCB_COPY_FROM_PARENT, native_handle, screen->root, 0, 0, 1280, 720, 1,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, nullptr);

    xcb_map_window(connection, native_handle);
    xcb_flush(connection);
}

X11Window::~X11Window() {
    auto *app = dynamic_cast<X11Application *>(&Application::GetInstance());
    Assert(app != nullptr, "Application is not an X11Application");
    auto connection = app->GetConnection();
    xcb_destroy_window(connection, native_handle);
    xcb_flush(connection);
}

}  // namespace logenium
