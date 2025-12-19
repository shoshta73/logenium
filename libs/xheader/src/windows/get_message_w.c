#include <xheader/windows.h>

#ifndef _WIN32

BOOL GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) { return FALSE; }

#endif
