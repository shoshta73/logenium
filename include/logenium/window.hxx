// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_WINDOW_HXX
#define LOGENIUM_WINDOW_HXX

#include <bit>
#include <cstddef>
#include <cstdint>

#include <xheader/windows.h>
#include <xheader/xcb/xproto.h>

//! Windows SDK defines min and max macros, which conflict with std::min and std::max
#ifdef min
#undef min
#endif

//! Windows SDK defines min and max macros, which conflict with std::min and std::max
#ifdef max
#undef max
#endif

#include <corelib/math/vector2.hxx>
#include <corelib/types/int.hxx>

namespace logenium {

using math::vec2;

class Window {
  public:
    enum class WindowKind : std::uint8_t {
        WK_Windows,
        WK_Linux,
        WK_LinuxWayland,
        WK_LinuxX11,
    };

    [[nodiscard]] WindowKind GetKind() const;

  private:
    const WindowKind kind;

  public:
    struct NativeHandle {
        NativeHandle() : handle(nullptr) {}
        NativeHandle(void *handle) : handle(handle) {}
        NativeHandle(std::nullptr_t) : handle(nullptr) {}
        NativeHandle(HWND handle) : handle(static_cast<void *>(handle)) {}
        NativeHandle(xcb_window_t handle) : handle(std::bit_cast<void *>(static_cast<std::uintptr_t>(handle))) {}

        operator void *() const { return handle; }
        operator HWND() const { return static_cast<HWND>(handle); }
        operator xcb_window_t() const { return static_cast<xcb_window_t>(std::bit_cast<std::uintptr_t>(handle)); }

      private:
        void *handle;
    };

    struct State {
        vec2<i32> dimensions{0, 0};
        vec2<i32> framebuffer_dimensions{0, 0};
    };

    virtual ~Window();

    NativeHandle &GetNativeHandle();
    State &GetState();

  protected:
    Window(WindowKind kind);

    NativeHandle native_handle{nullptr};
    State state{};
};

}  // namespace logenium

#endif  // LOGENIUM_WINDOW_HXX
