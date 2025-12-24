// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_WAYLAND_APPLICATION_HXX
#define LOGENIUM_PLATFORM_LINUX_WAYLAND_APPLICATION_HXX

#include "logenium/platform/linux/application.hxx"

namespace logenium {

class WaylandApplication : public LinuxApplication {
  public:
    WaylandApplication();
    ~WaylandApplication();

    void Run() final;

  private:
    void RegisterWindowClass() final;
    void UnregisterWindowClass() final;
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_WAYLAND_APPLICATION_HXX
