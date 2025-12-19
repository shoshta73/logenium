#ifndef LOGENIUM_PLATFORM_WINDOWS_WINDOW_HXX
#define LOGENIUM_PLATFORM_WINDOWS_WINDOW_HXX

#include <xheader/windows.h>

#include "logenium/window.hxx"

namespace logenium {

class WindowsWindow : public Window {
  public:
    WindowsWindow();
    ~WindowsWindow();

    static WNDCLASSEX &GetWindowClass();
    static const char *GetWindowClassName();
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  private:
    LRESULT HandleMessage(HWND pHwnd, UINT pMessage, WPARAM pWParam, LPARAM pLParam);
};

}  // namespace logenium

#endif  // LOGENIUM_PLATFORM_WINDOWS_WINDOW_HXX
