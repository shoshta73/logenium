#include <xheader/windows.h>

#ifndef _WIN32

LRESULT DispatchMessageA(const MSG * /* lpMsg */) { return 0; }

#endif
