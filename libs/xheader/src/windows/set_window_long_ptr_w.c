#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

LONG_PTR SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong) { return (LONG_PTR)NULL; }

#endif
