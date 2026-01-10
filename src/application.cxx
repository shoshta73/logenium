// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "logenium/application.hxx"

#include <memory>

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

#include "logenium/platform/linux/application.hxx"    // IWYU pragma: keep
#include "logenium/platform/windows/application.hxx"  // IWYU pragma: keep

namespace logenium {

class Application *Application::instance = nullptr;

Application::ApplicationKind Application::GetKind() const { return kind; };

Application::Application(ApplicationKind kind) : kind(kind) {
    ZoneScoped;
    Assert(instance == nullptr, "Application is already initialized");
    instance = this;

    log::trace("Application initialized");
}

Application::~Application() {
    ZoneScoped;
    instance = nullptr;

    log::trace("Application destroyed");
}

Application::NativeHandle &Application::GetNativeHandle() { return native_handle; }

Application::State &Application::GetState() { return state; }

Application &Application::GetInstance() {
    Assert(instance, "Application is not initialized");
    return *instance;
}

std::unique_ptr<Application> Application::Create() {
    ZoneScoped;

#if defined(_WIN32)

    return std::make_unique<WindowsApplication>();

#elif defined(__linux__)

    return LinuxApplication::Create();

#else

    return nullptr;

#endif
}

}  // namespace logenium
