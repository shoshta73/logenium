// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/application.hxx"

#include <memory>

#include <debug/assert.hxx>

#include "logenium/platform/linux/application.hxx"    // IWYU pragma: keep
#include "logenium/platform/windows/application.hxx"  // IWYU pragma: keep

namespace logenium {

class Application *Application::instance = nullptr;

Application::ApplicationKind Application::GetKind() const { return kind; };

Application::Application(ApplicationKind kind) : kind(kind) {
    Assert(instance == nullptr, "Application is already initialized");
    instance = this;
}

Application::~Application() { instance = nullptr; }

Application::NativeHandle &Application::GetNativeHandle() { return native_handle; }

Application::State &Application::GetState() { return state; }

Application &Application::GetInstance() {
    Assert(instance, "Application is not initialized");
    return *instance;
}

std::unique_ptr<Application> Application::Create() {
#if defined(_WIN32)

    return std::make_unique<WindowsApplication>();

#elif defined(__linux__)

    return LinuxApplication::Create();

#else

    return nullptr;

#endif
}

}  // namespace logenium
