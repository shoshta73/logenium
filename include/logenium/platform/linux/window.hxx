// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_WINDOW_HXX
#define LOGENIUM_PLATFORM_LINUX_WINDOW_HXX

#include "logenium/window.hxx"

namespace logenium {

class LinuxWindow : public Window {
  public:
    ~LinuxWindow() override;

    static bool classof(const Window *win);

  protected:
    LinuxWindow(WindowKind kind);
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_WINDOW_HXX
