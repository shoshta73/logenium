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
}

WindowsApplication::~WindowsApplication() {
    window.reset();
    UnregisterWindowClass();
    native_handle = nullptr;
}

void WindowsApplication::RegisterWindowClass() {
    auto window_class = WindowsWindow::GetWindowClass();
    Assert(RegisterClassEx(&window_class), "Failed to register window class");
}

void WindowsApplication::UnregisterWindowClass() {
    Assert(UnregisterClass(WindowsWindow::GetWindowClassName(), native_handle), "Failed to unregister window class");
}

}  // namespace logenium
