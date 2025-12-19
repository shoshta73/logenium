#include "logenium/platform/windows/window.hxx"

#include <xheader/windows.h>

#include <debug/assert.hxx>

#include "logenium/application.hxx"

namespace {

constexpr auto kLogeniumWindowClassName = "LogeniumWindowClass";
constexpr auto kWindowName = "Logenium";

}  // namespace

namespace logenium {

WindowsWindow::WindowsWindow() {
    auto handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, GetWindowClassName(), kWindowName, WS_OVERLAPPEDWINDOW, 0, 0,
                                 CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr,
                                 Application::GetInstance().GetNativeHandle(), nullptr);
    Assert(handle, "Failed to create window");
    native_handle = handle;
    ShowWindow(native_handle, SW_SHOW);
    UpdateWindow(native_handle);
}

WindowsWindow::~WindowsWindow() {
    DestroyWindow(native_handle);
    native_handle = nullptr;
}

WNDCLASSEX &WindowsWindow::GetWindowClass() {
    auto application_handle = Application::GetInstance().GetNativeHandle();
    static WNDCLASSEX window_class{
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = DefWindowProc,
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

}  // namespace logenium
