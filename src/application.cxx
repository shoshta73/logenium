#include "logenium/application.hxx"

#include <cassert>
#include <memory>

#include "logenium/platform/windows/application.hxx"

namespace logenium {

class Application *Application::instance = nullptr;

Application::Application() {
    assert(instance == nullptr && "Application is already initialized");
    instance = this;
}

Application::~Application() { instance = nullptr; }

Application &Application::GetInstance() {
    assert(instance && "Application is not initialized");
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
