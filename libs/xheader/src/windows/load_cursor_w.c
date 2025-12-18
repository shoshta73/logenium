#include <xheader/windows.h>

#ifndef _WIN32
#include <stddef.h>

HCURSOR LoadCursorW(HINSTANCE /* hInstance */, LPCWSTR /* lpCursorName */) { return NULL; }

#endif
