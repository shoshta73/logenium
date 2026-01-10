// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/windows/application.hxx"

#include <memory>

#include <xheader/windows.h>

#include <debug/assert.hxx>
#include <debug/tracing/macros.hxx>

//! Windows SDK defines min and max macros, which conflict with std::min and std::max
#ifdef min
#undef min
#endif

//! Windows SDK defines min and max macros, which conflict with std::min and std::max
#ifdef max
#undef max
#endif

#include <logging/logging.hxx>

#include "logenium/application.hxx"
#include "logenium/platform/windows/window.hxx"

namespace logenium {

WindowsApplication::WindowsApplication() : Application(ApplicationKind::AK_Windows) {
    ZoneScoped;
    auto handle = GetModuleHandle(nullptr);
    Assert(handle, "Failed to get module handle");
    native_handle = handle;
    log::trace("Native Handle obtained");

    RegisterWindowClass();

    window = std::make_shared<WindowsWindow>();

    state.is_running = true;

    log::debug("WindowsApplication initialized");
}

WindowsApplication::~WindowsApplication() {
    ZoneScoped;
    if (state.is_running) {
        state.is_running = false;
    }

    window.reset();
    UnregisterWindowClass();
    native_handle = nullptr;

    log::debug("WindowsApplication destroyed");
}

void WindowsApplication::Run() {
    ZoneScoped;

    while (state.is_running) {
        MSG msg;
        switch (GetMessage(&msg, nullptr, 0, 0)) {
            case -1:
                [[fallthrough]];
            case 0: {
                state.is_running = false;
                break;
            }
            default: {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
}

bool WindowsApplication::classof(const Application *app) { return app->GetKind() == ApplicationKind::AK_Windows; }

void WindowsApplication::RegisterWindowClass() {
    ZoneScoped;
    auto window_class = WindowsWindow::GetWindowClass();
    Assert(RegisterClassEx(&window_class), "Failed to register window class");
    log::trace("Window Class registered");
}

void WindowsApplication::UnregisterWindowClass() {
    ZoneScoped;
    Assert(UnregisterClass(WindowsWindow::GetWindowClassName(), native_handle), "Failed to unregister window class");
    log::trace("Window Class unregistered");
}

}  // namespace logenium
