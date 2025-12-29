// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_PLATFORM_WINDOWS_WINDOW_HXX
#define LOGENIUM_PLATFORM_WINDOWS_WINDOW_HXX

#include <xheader/windows.h>

#include "logenium/window.hxx"

namespace logenium {

class WindowsWindow : public Window {
  public:
    WindowsWindow();
    ~WindowsWindow() override;

    static WNDCLASSEX &GetWindowClass();
    static const char *GetWindowClassName();
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  private:
    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_WINDOWS_WINDOW_HXX
