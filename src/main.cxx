
#include <cassert>

#include "logenium/application.hxx"

int main() {
    auto app = logenium::Application::Create();
    assert(app && "Application is not created");
}
