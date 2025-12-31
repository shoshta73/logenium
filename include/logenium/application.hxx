// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_APPLICATION_HXX
#define LOGENIUM_APPLICATION_HXX

#include <cstddef>
#include <cstdint>
#include <memory>

#include <xheader/windows.h>

#include "logenium/window.hxx"

namespace logenium {

class Application {
  public:
    enum class ApplicationKind : std::uint8_t {
        AK_Windows,
        AK_Linux,
        AK_LinuxWayland,
        AK_LinuxX11,
    };

    [[nodiscard]] ApplicationKind GetKind() const;

  private:
    const ApplicationKind kind;

  public:
    struct NativeHandle {
        NativeHandle() : handle(nullptr) {}
        NativeHandle(void *handle) : handle(handle) {}
        NativeHandle(std::nullptr_t) : handle(nullptr) {}
        NativeHandle(HMODULE handle) : handle(static_cast<void *>(handle)) {}

        operator void *() const { return handle; }
        operator HMODULE() const { return static_cast<HMODULE>(handle); }

      private:
        void *handle;
    };

    struct State {
        bool is_running{false};
    };

    virtual ~Application();

    virtual void Run() = 0;
    State &GetState();
    NativeHandle &GetNativeHandle();

    static Application &GetInstance();
    static std::unique_ptr<Application> Create();

  protected:
    Application(ApplicationKind kind);

    NativeHandle native_handle{nullptr};
    State state{};
    std::shared_ptr<Window> window;

    virtual void RegisterWindowClass() = 0;
    virtual void UnregisterWindowClass() = 0;

  private:
    static Application *instance;
};

}  // namespace logenium

#endif  // LOGENIUM_APPLICATION_HXX
