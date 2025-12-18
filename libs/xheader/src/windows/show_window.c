#include <xheader/windows.h>

#ifndef _WIN32

BOOL ShowWindow(HWND /* hWnd */, int /* nCmdShow */) { return FALSE; }

#endif
