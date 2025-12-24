// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_WINDOWS_APPLICATION_HXX
#define LOGENIUM_PLATFORM_WINDOWS_APPLICATION_HXX

#include "logenium/application.hxx"

namespace logenium {

class WindowsApplication : public Application {
  public:
    WindowsApplication();
    ~WindowsApplication();

    void Run() final;

  private:
    void RegisterWindowClass() final;
    void UnregisterWindowClass() final;
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_WINDOWS_APPLICATION_HXX
