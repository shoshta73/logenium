// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/windows/application.hxx"

#include <memory>

#include <xheader/windows.h>

#include <debug/assert.hxx>

#include "logenium/platform/windows/window.hxx"

namespace logenium {

WindowsApplication::WindowsApplication() {
    auto handle = GetModuleHandle(nullptr);
    Assert(handle, "Failed to get module handle");
    native_handle = handle;

    RegisterWindowClass();

    window = std::make_shared<WindowsWindow>();

    state.is_running = true;
}

WindowsApplication::~WindowsApplication() {
    if (state.is_running) {
        state.is_running = false;
    }

    window.reset();
    UnregisterWindowClass();
    native_handle = nullptr;
}

void WindowsApplication::Run() {
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

void WindowsApplication::RegisterWindowClass() {
    auto window_class = WindowsWindow::GetWindowClass();
    Assert(RegisterClassEx(&window_class), "Failed to register window class");
}

void WindowsApplication::UnregisterWindowClass() {
    Assert(UnregisterClass(WindowsWindow::GetWindowClassName(), native_handle), "Failed to unregister window class");
}

}  // namespace logenium
