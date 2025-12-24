// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_LINUX_WAYLAND_WINDOW_HXX
#define LOGENIUM_PLATFORM_LINUX_WAYLAND_WINDOW_HXX

#include "logenium/platform/linux/window.hxx"

namespace logenium {

class WaylandWindow : public LinuxWindow {
  public:
    WaylandWindow();
    ~WaylandWindow();
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_LINUX_WAYLAND_WINDOW_HXX
