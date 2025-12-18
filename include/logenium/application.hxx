#ifndef LOGENIUM_APPLICATION_HXX
#define LOGENIUM_APPLICATION_HXX

#include <cstddef>
#include <memory>

#include <xheader/windows.h>

#include "logenium/window.hxx"

namespace logenium {

class Application {
  public:
    struct NativeHandle {
        constexpr NativeHandle() : handle(nullptr) {}
        NativeHandle(void *handle) : handle(handle) {}
        constexpr NativeHandle(std::nullptr_t) : handle(nullptr) {}
        NativeHandle(HMODULE handle) : handle(static_cast<void *>(handle)) {}

        operator void *() const { return handle; }
        operator HMODULE() const { return static_cast<HMODULE>(handle); }

      private:
        void *handle;
    };

    Application();
    virtual ~Application();

    NativeHandle &GetNativeHandle();

    static Application &GetInstance();
    static std::unique_ptr<Application> Create();

  protected:
    NativeHandle native_handle{nullptr};
    std::shared_ptr<Window> window;

    virtual void RegisterWindowClass() = 0;
    virtual void UnregisterWindowClass() = 0;

  private:
    static Application *instance;
};

}  // namespace logenium

#endif  // LOGENIUM_APPLICATION_HXX
