#include "logenium/application.hxx"

#include <memory>

#include <debug/assert.hxx>

#include "logenium/platform/windows/application.hxx"

namespace logenium {

class Application *Application::instance = nullptr;

Application::Application() {
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

#else

    return nullptr;

#endif
}

}  // namespace logenium
