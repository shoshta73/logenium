#include <xheader/windows.h>

#ifndef _WIN32

BOOL TranslateMessage(const MSG * /* lpMsg */) { return FALSE; }

#endif
