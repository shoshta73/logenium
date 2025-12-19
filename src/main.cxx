#include <debug/assert.hxx>

#include "logenium/application.hxx"

int main() {
    using namespace logenium;
    auto app = logenium::Application::Create();
    Assert(app != nullptr, "Application is not created");
    app->Run();
}
