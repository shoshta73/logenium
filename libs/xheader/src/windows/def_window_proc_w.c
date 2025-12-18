#include <xheader/windows.h>

#ifndef _WIN32

LRESULT DefWindowProcW(HWND /* hWnd */, UINT /* msg */, WPARAM /* wParam */, LPARAM /* lParam */) { return 0; }

#endif
