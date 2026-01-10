// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/platform/windows/window.hxx"

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
#include "logenium/window.hxx"

namespace {

constexpr auto kLogeniumWindowClassName = "LogeniumWindowClass";
constexpr auto kWindowName = "Logenium";

}  // namespace

namespace logenium {

WindowsWindow::WindowsWindow() : Window(WindowKind::WK_Windows) {
    ZoneScoped;
    auto handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, GetWindowClassName(), kWindowName, WS_OVERLAPPEDWINDOW, 0, 0,
                                 CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                                 Application::GetInstance().GetNativeHandle(), this);
    Assert(handle, "Failed to create window");
    native_handle = handle;
    log::trace("Native Window created");
    log::trace("Native handle obtained");

    ShowWindow(native_handle, SW_SHOW);
    UpdateWindow(native_handle);
    log::debug("WindowsWindow created");
}

WindowsWindow::~WindowsWindow() {
    ZoneScoped;
    DestroyWindow(native_handle);
    log::trace("Native Window destroyed");

    native_handle = nullptr;
    log::trace("Native handle released");
    log::debug("WindowsWindow destroyed");
}

WNDCLASSEX &WindowsWindow::GetWindowClass() {
    ZoneScoped;
    auto application_handle = Application::GetInstance().GetNativeHandle();
    static WNDCLASSEX window_class{
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProc,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = application_handle,
        .hIcon = LoadIcon(application_handle, IDI_APPLICATION),
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1),
        .lpszMenuName = nullptr,
        .lpszClassName = GetWindowClassName(),
        .hIconSm = LoadIcon(application_handle, IDI_APPLICATION),

    };
    return window_class;
}

const char *WindowsWindow::GetWindowClassName() { return kLogeniumWindowClassName; }

LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ZoneScoped;
    WindowsWindow *self = nullptr;

    if (msg == WM_NCCREATE) {
        log::trace1("WM_NCCREATE handler reached");
        auto create_struct = reinterpret_cast<CREATESTRUCT *>(lParam);
        self = reinterpret_cast<WindowsWindow *>(create_struct->lpCreateParams);
        Assert(self != nullptr, "Failed to get window pointer");

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        return DefWindowProc(hWnd, msg, wParam, lParam);
    } else {
        self = reinterpret_cast<WindowsWindow *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (self == nullptr) {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        return self->HandleMessage(hWnd, msg, wParam, lParam);
    }
}

bool WindowsWindow::classof(const Window *win) { return win->GetKind() == WindowKind::WK_Windows; }

LRESULT WindowsWindow::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ZoneScoped;
    switch (msg) {
        case WM_CLOSE: {
            log::trace1("WM_CLOSE handler reached");
            Application::GetInstance().GetState().is_running = false;
            PostQuitMessage(0);
            return 0;
        }
        case WM_DESTROY: {
            log::trace1("WM_DESTROY handler reached");
            Application::GetInstance().GetState().is_running = false;
            PostQuitMessage(0);
            return 0;
        }
        default: {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
}

}  // namespace logenium
