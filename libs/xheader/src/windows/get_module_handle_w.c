#include <xheader/windows.h>

#ifndef _WIN32

#include <stddef.h>

HMODULE GetModuleHandleW(LPCWSTR /* lpModuleName */) { return NULL; }

#endif  // _WIN32
