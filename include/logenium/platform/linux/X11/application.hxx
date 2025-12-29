// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_X11_APPLICATION_HXX
#define LOGENIUM_PLATFORM_LINUX_X11_APPLICATION_HXX

#include <xheader/xcb/xcb.h>
#include <xheader/xcb/xproto.h>

#include "logenium/platform/linux/application.hxx"

namespace logenium {

class X11Application : public LinuxApplication {
  public:
    X11Application();
    ~X11Application() override;

    void Run() final;
    [[nodiscard]] xcb_connection_t *GetConnection() const;
    [[nodiscard]] xcb_screen_t *GetScreen() const;

  private:
    xcb_connection_t *connection;
    xcb_screen_t *screen;

    void RegisterWindowClass() final;
    void UnregisterWindowClass() final;
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_X11_APPLICATION_HXX
