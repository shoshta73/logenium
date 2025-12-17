#ifndef LOGENIUM_APPLICATION_HXX
#define LOGENIUM_APPLICATION_HXX

#include <memory>

namespace logenium {

class Application {
  public:
    Application();
    virtual ~Application();

    static Application &GetInstance();
    static std::unique_ptr<Application> Create();

  private:
    static Application *instance;
};

}  // namespace logenium

#endif  // LOGENIUM_APPLICATION_HXX
