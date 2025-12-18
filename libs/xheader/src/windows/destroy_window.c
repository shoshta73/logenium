#include <xheader/windows.h>

#ifndef _WIN32

BOOL DestroyWindow(HWND /* hWnd */) { return FALSE; }

#endif
