// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/windows.h>

#ifndef _WIN32
#include <stddef.h>

HWND CreateWindowExA(DWORD /* dwExStyle */, LPCSTR /* lpClassName */, LPCSTR /* lpWindowName */, DWORD /* dwStyle */,
                     int /* x */, int /* y */, int /* nWidth */, int /* nHeight */, HWND /* hWndParent */,
                     HMENU /* hMenu */, HINSTANCE /* hInstance */, LPVOID /* lpParam */) {
    return NULL;
}

#endif
