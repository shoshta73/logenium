#include <xheader/windows.h>

#ifndef _WIN32

BOOL UnregisterClassW(LPCWSTR /* lpClassName */, HINSTANCE /* hInstance */) { return FALSE; }

#endif
