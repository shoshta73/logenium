#include <xheader/windows.h>

#ifndef _WIN32

ATOM RegisterClassExA(const WNDCLASSEXA * /* lpwcx */) { return 0; }

#endif
