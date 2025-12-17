#include <xheader/windows.h>

#if !defined(_WIN32)

#include <stddef.h>

HMODULE GetModuleHandleA(LPCSTR /* lpModuleName */) { return NULL; }

#endif  // !defined(_WIN32)
