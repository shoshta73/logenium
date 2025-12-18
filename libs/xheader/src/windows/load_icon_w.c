#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

HICON LoadIconW(HINSTANCE /* hInstance */, LPCWSTR /* lpIconName */) { return NULL; }

#endif
