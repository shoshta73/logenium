#include "logenium/platform/windows/application.hxx"

#include <cassert>
#include <memory>

#include <xheader/windows.h>

#include "logenium/platform/windows/window.hxx"

namespace logenium {

WindowsApplication::WindowsApplication() {
    auto handle = GetModuleHandle(nullptr);
    assert(handle != nullptr && "Failed to get module handle");
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
    assert(RegisterClassEx(&window_class) != 0 && "Failed to register window class");
}

void WindowsApplication::UnregisterWindowClass() {
    assert(UnregisterClass(WindowsWindow::GetWindowClassName(), native_handle) != 0 &&
           "Failed to unregister window class");
}

}  // namespace logenium
