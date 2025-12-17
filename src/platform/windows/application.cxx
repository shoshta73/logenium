#include "logenium/platform/windows/application.hxx"

#include <cassert>

#include <xheader/windows.h>

namespace logenium {

WindowsApplication::WindowsApplication() {
    auto handle = GetModuleHandle(nullptr);
    assert(handle != nullptr && "Failed to get module handle");
    native_handle = handle;
}

WindowsApplication::~WindowsApplication() { native_handle = nullptr; }

}  // namespace logenium
