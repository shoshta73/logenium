// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_X11_APPLICATION_HXX
#define LOGENIUM_PLATFORM_LINUX_X11_APPLICATION_HXX

#include "logenium/platform/linux/application.hxx"

namespace logenium {

class X11Application : public LinuxApplication {
  public:
    X11Application();
    ~X11Application();

    void Run() final;

  private:
    void RegisterWindowClass() final;
    void UnregisterWindowClass() final;
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_X11_APPLICATION_HXX
