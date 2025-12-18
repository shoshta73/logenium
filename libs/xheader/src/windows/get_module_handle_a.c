#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

HMODULE GetModuleHandleA(LPCSTR /* lpModuleName */) { return NULL; }

#endif  // _WIN32
