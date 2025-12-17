#include <xheader/windows.h>

#if !defined(_WIN32)

#include <stddef.h>

HMODULE GetModuleHandleW(LPCWSTR /* lpModuleName */) { return NULL; }

#endif  // !defined(_WIN32)
