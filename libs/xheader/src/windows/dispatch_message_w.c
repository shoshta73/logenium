#include <xheader/windows.h>

#ifndef _WIN32

LRESULT DispatchMessageW(const MSG * /* lpMsg */) { return 0; }

#endif
