#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

LONG_PTR GetWindowLongPtrW(HWND hWnd, int nIndex) { return NULL; }

#endif
