#ifndef LOGENIUM_PLATFORM_WINDOWS_APPLICATION_HXX
#define LOGENIUM_PLATFORM_WINDOWS_APPLICATION_HXX

#include "logenium/application.hxx"

namespace logenium {

class WindowsApplication : public Application {
  public:
    WindowsApplication();
    ~WindowsApplication();

    void Run() override;

  private:
    void RegisterWindowClass() override;
    void UnregisterWindowClass() override;
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_WINDOWS_APPLICATION_HXX
