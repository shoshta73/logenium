#include <xheader/windows.h>

#ifndef _WIN32

ATOM RegisterClassExW(const WNDCLASSEXW * /* lpwcx */) { return 0; }

#endif
