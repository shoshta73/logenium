// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <xheader/windows.h>

#ifndef _WIN32
#include <stddef.h>

HWND CreateWindowExW(DWORD /* dwExStyle */, LPCWSTR /* lpClassName */, LPCWSTR /* lpWindowName */, DWORD /* dwStyle */,
                     int /* x */, int /* y */, int /* nWidth */, int /* nHeight */, HWND /* hWndParent */,
                     HMENU /* hMenu */, HINSTANCE /* hInstance */, LPVOID /* lpParam */) {
    return NULL;
}

#endif
