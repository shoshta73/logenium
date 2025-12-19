#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

LONG_PTR SetWindowLongPtrA(HWND hWnd, int nIndex, LONG_PTR dwNewLong) { return NULL; }

#endif
