#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

HCURSOR LoadCursorA(HINSTANCE /* hInstance */, LPCSTR /* lpCursorName */) { return NULL; }

#endif
