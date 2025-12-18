#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

HICON LoadIconA(HINSTANCE /* hInstance */, LPCSTR /* lpIconName */) { return NULL; }

#endif
