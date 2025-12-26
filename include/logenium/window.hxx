// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_WINDOW_HXX
#define LOGENIUM_WINDOW_HXX

#include <bit>
#include <cstddef>
#include <cstdint>

#include <xheader/windows.h>
#include <xheader/xcb/xproto.h>

namespace logenium {

class Window {
  public:
    struct NativeHandle {
        constexpr NativeHandle() : handle(nullptr) {}
        NativeHandle(void *handle) : handle(handle) {}
        constexpr NativeHandle(std::nullptr_t) : handle(nullptr) {}
        NativeHandle(HWND handle) : handle(static_cast<void *>(handle)) {}
        NativeHandle(xcb_window_t handle) : handle(std::bit_cast<void *>(static_cast<std::uintptr_t>(handle))) {}

        operator void *() const { return handle; }
        operator HWND() const { return static_cast<HWND>(handle); }
        operator xcb_window_t() const { return static_cast<xcb_window_t>(std::bit_cast<std::uintptr_t>(handle)); }

      private:
        void *handle;
    };

    Window() = default;
    virtual ~Window() = default;

    NativeHandle &GetNativeHandle();

  protected:
    NativeHandle native_handle{nullptr};
};

}  // namespace logenium

#endif  // LOGENIUM_WINDOW_HXX
