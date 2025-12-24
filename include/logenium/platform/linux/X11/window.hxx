// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_X11_WINDOW_HXX
#define LOGENIUM_PLATFORM_LINUX_X11_WINDOW_HXX

#include "logenium/platform/linux/window.hxx"

namespace logenium {

class X11Window : public LinuxWindow {
  public:
    X11Window();
    ~X11Window();
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_X11_WINDOW_HXX
