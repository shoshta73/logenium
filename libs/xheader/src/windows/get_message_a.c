#include <xheader/windows.h>

#ifndef _WIN32

BOOL GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) { return FALSE; }

#endif
