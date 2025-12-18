#include <xheader/windows.h>

#ifndef _WIN32
#include <stddef.h>

HWND CreateWindowExA(DWORD /* dwExStyle */, LPCSTR /* lpClassName */, LPCSTR /* lpWindowName */, DWORD /* dwStyle */,
                     int /* x */, int /* y */, int /* nWidth */, int /* nHeight */, HWND /* hWndParent */,
                     HMENU /* hMenu */, HINSTANCE /* hInstance */, LPVOID /* lpParam */) {
    return NULL;
}

#endif
