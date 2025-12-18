#include <xheader/windows.h>

#ifndef _WIN32

BOOL UnregisterClassA(LPCSTR /* lpClassName */, HINSTANCE /* hInstance */) { return FALSE; }

#endif
