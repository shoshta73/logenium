#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

LONG_PTR GetWindowLongPtrA(HWND hWnd, int nIndex) { return (LONG_PTR)NULL; }

#endif
