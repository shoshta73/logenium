#include "xheader/windows.h"

#ifndef _WIN32

VOID PostQuitMessage(int nExitCode) { (void)nExitCode; }

#endif
